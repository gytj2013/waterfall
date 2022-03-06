#include "waterObj.h"

#define MIN(x,y) (((x)<(y))? (x):(y)) //둘 중에 작은거
#define MAX(x,y) (((x)>(y))? (x):(y)) // 둘 중에 큰거

water::water(int num_of_line)
{
	num_of_path = num_of_line * 2 + 2; // 물 경로 개수 = 줄수 x 2 + 2

	if (stream_data) { 
		stream_data = new int* [num_of_path]; // 물 경로 정보 저장할 배열 동적할당
		for (int i = 0; i < num_of_path; i++) {
			stream_data[i] = new int[2]; // 물 경로 정보 저장할 배열 동적할당
			stream_data[i][0] = stream_data[i][1] = -1; // 물 경로 정보 초기화
		}
	}
	draw_path_flag = 0; // 물 경로 계산 미완료된 상태
}

water::~water()
{
	if (!stream_data) {
		for (int i = 0; i < num_of_path; i++) { //물 경로 정보 저장할 배열 해제
			delete stream_data[i];
		}
		delete stream_data; 
	}
}

void water::draw(int water_type, int tap_x, int tap_y)
{
	if (draw_path_flag) { //물 경로 계산할거면

		float xOffset = ofRandom(-20, 20); // x좌표 값 랜덤하게
		float yOffset = ofRandom(-20, 10); // y좌표 값 랜덤하게
		float radius = ofRandom(5, 10); // 반지름 값 랜덤하게

		for (int i = 0; ; i++) {
			if (stream_data[i][0] < ofGetWidth() || stream_data[i][1] < ofGetHeight()) // 물 경로 정보가 범위안에 들어오면
			{
				ofSetColor(0,0,255); // 물 파란색으로
				ofDrawLine(stream_data[i][0] , stream_data[i][1], stream_data[i + 1][0] , stream_data[i + 1][1]); // 물의 경로를 그려준다.	
				ofSetColor(0, 0, 255, ofRandom(50,200)); //투명값을 random으로 
				ofDrawCircle(stream_data[i + 1][0] + xOffset, stream_data[i + 1][1] + yOffset, radius); // 튀긴 물방울 그리기
			}

			if (stream_data[i + 1][1] == ofGetHeight()) break; // 물이 바닥에 닿았으면 break
			else if (stream_data[i + 1][0] == ofGetWidth()) break; //물이 중력방향 바닥에 닿았으면 break
			else if (stream_data[i + 1][1] == 0) break; //물이 중력방향 바닥에 닿았으면 break
			else if (stream_data[i + 1][0] == 0) break; //물이 중력방향 바닥에 닿았으면 break
			else continue; //아니면 계속
		}

		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) { //왼쪽 마우스 버튼이 눌리면
			ofSetRectMode(OF_RECTMODE_CENTER); //시작점을 사각형 중심으로 설정
			if ((ofGetMouseX()-25 < tap_x) && (ofGetMouseX() + 25 > tap_x) && (ofGetMouseY() - 25 < tap_y) && (ofGetMouseY() + 25 > tap_y)) { // 물 나오는 점을 누르면
				if (water_type == 0) ofSetColor(255); // 중성이면 흰색으로
				else if (water_type == 1) ofSetColor(219, 68, 85); // 산성이면 붉은색으로
				else if (water_type == 2) ofSetColor(75, 137, 220); // 염기성이면 푸른색으로
				ofDrawRectangle(ofGetMouseX(), ofGetMouseY(), 50, 50); // Draw a 50 x 50 rect centered over the mouse
			}
		}
	}
}

void water::calculationPath(int ** line_data, int num_of_line, int change_gravity)
{ //물 경로를 계산해준다.
	int tap_x = stream_data[0][0], tap_y = stream_data[0][1]; // 물이 나오는곳의 좌표를 각각 저장한다.
	int nearest_i; //가장 가까운 줄의 인덱스 변수

	for (int i = 1; ; i++) {
		nearest_i = findNearestLine(line_data, num_of_line, tap_x, tap_y, change_gravity); //현재점에서 가장 가까운 줄을 찾는다.
		if (change_gravity == 0) {
			if (nearest_i == -1) { //바닥에 닿으면
				stream_data[i][0] = tap_x; // 현재 x 좌표와 같고
				stream_data[i][1] = ofGetHeight(); //y좌표는 바닥 높이와 같다.
				break;
			}

			stream_data[i][0] = tap_x; // 현재 x 좌표와 같고
			stream_data[i][1] = tap_y - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //y좌표는 현재 y좌표에서 가장 가까운 선까지의 거리만큼을 뺀 값이다.
			i++;

			if (line_data[nearest_i][1] > line_data[nearest_i][3]) { //가장 가까운 선의 왼쪽으로 물이 흐르면
				tap_x = line_data[nearest_i][0]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][1]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
			else {
				tap_x = line_data[nearest_i][2]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][3]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
		}
		else if (change_gravity == 1) {
			if (nearest_i == -1) { //바닥에 닿으면
				stream_data[i][1] = tap_y; // 현재 y 좌표와 같고
				stream_data[i][0] = ofGetWidth(); //x좌표는 창의 너비와 같다.
				break;
			}
			stream_data[i][1] = tap_y; // 현재 y 좌표와 같고
			stream_data[i][0] = tap_x - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //x좌표는 현재 x좌표에서 가장 가까운 선까지의 거리만큼을 뺀 값이다.
			i++;
			if (line_data[nearest_i][0] > line_data[nearest_i][2]) { //가장 가까운 선의 물이 흐르는 방향
				tap_x = line_data[nearest_i][0]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][1]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
			else {
				tap_x = line_data[nearest_i][2]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][3]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
		}
		else if (change_gravity == 2) {
			if (nearest_i == -1) { //바닥에 닿으면
				stream_data[i][0] = tap_x; // 현재 x 좌표와 같고
				stream_data[i][1] = 0; //y좌표는 천장 높이와 같다.
				break;
			}
			stream_data[i][0] = tap_x; // 현재 x 좌표와 같고
			stream_data[i][1] = tap_y - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //y좌표는 현재 y좌표에서 가장 가까운 선까지의 거리만큼을 뺀 값이다.
			i++;
			if (line_data[nearest_i][1] < line_data[nearest_i][3]) { //가장 가까운 선의 물이 흐르는 방향
				tap_x = line_data[nearest_i][0]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][1]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
			else {
				tap_x = line_data[nearest_i][2]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][3]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
		}
		else {
			if (nearest_i == -1) { //바닥에 닿으면
				stream_data[i][1] = tap_y; // 현재 y 좌표와 같고
				stream_data[i][0] = 0; //x좌표는 왼쪽의 끝와 같다.
				break;
			}
			stream_data[i][1] = tap_y; // 현재 y 좌표와 같고
			stream_data[i][0] = tap_x - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //x좌표는 현재 x좌표에서 가장 가까운 선까지의 거리만큼을 뺀 값이다.
			i++;
			if (line_data[nearest_i][0] < line_data[nearest_i][2]) { //가장 가까운 선의 물이 흐르는 방향
				tap_x = line_data[nearest_i][0]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][1]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
			else {
				tap_x = line_data[nearest_i][2]; //물이 흐르는 쪽 x좌표를 저장한다.
				tap_y = line_data[nearest_i][3]; //물이 흐르는 쪽 y좌표를 저장한다.
			}
		}

		stream_data[i][0] = tap_x; //물이 흐르는 쪽 x좌표를 저장한다.
		stream_data[i][1] = tap_y; //물이 흐르는 쪽 y좌표를 저장한다.
	}
	draw_path_flag = 1; //물 경로가 계산되었다.
}

int water::distance(int * line_data, int tap_x, int tap_y, int change_gravity) //점과 직선간의 거리를 계산한다.
{
	float slope = (float(line_data[3] - line_data[1]) / float(line_data[2] - line_data[0])); //기울기를 계산한다.
	
	float end_y = (slope *(tap_x - line_data[0])) + line_data[1]; // 점에서 수직으로 떨어졌을 때 바로 닿는 부분의 y좌표

	float end_x = ((tap_y - line_data[1]) / slope) + line_data[0]; // 점에서 수직으로 떨어졌을 때 바로 닿는 부분의 x좌표
	
	if (change_gravity == 0 || change_gravity == 2)
		return (int)(tap_y - end_y); // 점과 직선 사이의 거리를 return한다.
	else if(change_gravity==1 || change_gravity == 3)
		return (int)(tap_x - end_x); // 점과 직선 사이의 거리를 return한다.

}

int water::findNearestLine(int** line_data, int num_of_line, int tap_x, int tap_y, int change_gravity) // 가장 가까운 선을 찾아준다.
{
	int nearest_i = -1; //맨 왼쪽 줄부터 탐색 시작

	if (change_gravity == 0) {
		int nearest_y = ofGetHeight(); // 초기 가까운 y좌표를 바닥높이로 설정
		for (int i = 0; i < num_of_line; i++) { //모든 줄 탐색
			if (line_data[i][1] < tap_y || line_data[i][3] < tap_y) continue; //선의 양쪽 y좌표가 현재좌표보다 위에 있다면 다음으로 continue
			if (MIN(line_data[i][0], line_data[i][2]) > tap_x || MAX(line_data[i][0], line_data[i][2]) < tap_x) continue; //물이 수직으로 떨어졌을 때 안 닿는 선이면 다음으로 continue

			int min_y = MIN(line_data[i][1], line_data[i][3]); //양쪽 y좌표 중 위쪽에 있는 걸 min_y에 저장

			if (nearest_y > min_y) { // 기존에 찾아놓은 가까운 y좌표보다 현재 찾은게 더 위에 있다면
				nearest_y = min_y; //현재 찾아놓은 y좌표로 update
				nearest_i = i; //현재 찾아놓은 줄로 update
			}
		}
	}
	else if (change_gravity == 1) {
		int nearest_x = ofGetWidth(); // 초기 가까운 x좌표를 창의 너비로 설정
		for (int i = 0; i < num_of_line; i++) { //모든 줄 탐색
			if (line_data[i][0] < tap_x || line_data[i][2] < tap_x) continue; //선의 양쪽 x좌표가 현재좌표보다 왼쪽에 있다면 다음으로 continue
			if (MIN(line_data[i][1], line_data[i][3]) > tap_y || MAX(line_data[i][1], line_data[i][3]) < tap_y) continue; //물이 수평으로 떨어졌을 때 안 닿는 선이면 다음으로 continue

			int min_x = MIN(line_data[i][0], line_data[i][2]); //양쪽 x좌표 중 오른쪽에 있는 걸 min_x에 저장

			if (nearest_x > min_x) { // 기존에 찾아놓은 가까운 x좌표보다 현재 찾은게 더 왼쪽에 있다면
				nearest_x = min_x; //현재 찾아놓은 x좌표로 update
				nearest_i = i; //현재 찾아놓은 줄로 update
			}
		}
	}
	else if (change_gravity == 2) {
		int nearest_y = 0; // 초기 가까운 y좌표를 천장높이로 설정
		for (int i = 0; i < num_of_line; i++) { //모든 줄 탐색
			if (line_data[i][1] > tap_y || line_data[i][3] > tap_y) continue; //선의 양쪽 y좌표가 현재좌표보다 아래에 있다면 다음으로 continue
			if (MIN(line_data[i][0], line_data[i][2]) > tap_x || MAX(line_data[i][0], line_data[i][2]) < tap_x) continue; //물이 위로 떨어졌을 때 안 닿는 선이면 다음으로 continue

			int max_y = MAX(line_data[i][1], line_data[i][3]); //양쪽 y좌표 중 왼쪽에 있는 걸 max_y에 저장

			if (nearest_y < max_y) { // 기존에 찾아놓은 가까운 y좌표보다 현재 찾은게 더 아래에 있다면
				nearest_y = max_y; //현재 찾아놓은 y좌표로 update
				nearest_i = i; //현재 찾아놓은 줄로 update
			}
		}
	}
	else {
		int nearest_x = 0; // 초기 가까운 x좌표를 왼쪽 면으로 설정
		for (int i = 0; i < num_of_line; i++) { //모든 줄 탐색
			if (line_data[i][0] > tap_x || line_data[i][2] > tap_x) continue; //선의 양쪽 x좌표가 현재좌표보다 오른쪽에 있다면 다음으로 continue
			if (MIN(line_data[i][1], line_data[i][3]) > tap_y || MAX(line_data[i][1], line_data[i][3]) < tap_y) continue; //물이 수평으로 떨어졌을 때 안 닿는 선이면 다음으로 continue

			int max_x = MAX(line_data[i][0], line_data[i][2]); //양쪽 x좌표 중 오른쪽에 있는 걸 max_x에 저장

			if (nearest_x < max_x) { // 기존에 찾아놓은 가까운 x좌표보다 현재 찾은게 더 오른쪽에 있다면
				nearest_x = max_x; //현재 찾아놓은 x좌표로 update
				nearest_i = i; //현재 찾아놓은 줄로 update
			}
		}
	}
	return nearest_i; //찾은 가장 가까운 줄의 인덱스 반환
}