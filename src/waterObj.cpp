#include "waterObj.h"

#define MIN(x,y) (((x)<(y))? (x):(y)) //�� �߿� ������
#define MAX(x,y) (((x)>(y))? (x):(y)) // �� �߿� ū��

water::water(int num_of_line)
{
	num_of_path = num_of_line * 2 + 2; // �� ��� ���� = �ټ� x 2 + 2

	if (stream_data) { 
		stream_data = new int* [num_of_path]; // �� ��� ���� ������ �迭 �����Ҵ�
		for (int i = 0; i < num_of_path; i++) {
			stream_data[i] = new int[2]; // �� ��� ���� ������ �迭 �����Ҵ�
			stream_data[i][0] = stream_data[i][1] = -1; // �� ��� ���� �ʱ�ȭ
		}
	}
	draw_path_flag = 0; // �� ��� ��� �̿Ϸ�� ����
}

water::~water()
{
	if (!stream_data) {
		for (int i = 0; i < num_of_path; i++) { //�� ��� ���� ������ �迭 ����
			delete stream_data[i];
		}
		delete stream_data; 
	}
}

void water::draw(int water_type, int tap_x, int tap_y)
{
	if (draw_path_flag) { //�� ��� ����ҰŸ�

		float xOffset = ofRandom(-20, 20); // x��ǥ �� �����ϰ�
		float yOffset = ofRandom(-20, 10); // y��ǥ �� �����ϰ�
		float radius = ofRandom(5, 10); // ������ �� �����ϰ�

		for (int i = 0; ; i++) {
			if (stream_data[i][0] < ofGetWidth() || stream_data[i][1] < ofGetHeight()) // �� ��� ������ �����ȿ� ������
			{
				ofSetColor(0,0,255); // �� �Ķ�������
				ofDrawLine(stream_data[i][0] , stream_data[i][1], stream_data[i + 1][0] , stream_data[i + 1][1]); // ���� ��θ� �׷��ش�.	
				ofSetColor(0, 0, 255, ofRandom(50,200)); //������ random���� 
				ofDrawCircle(stream_data[i + 1][0] + xOffset, stream_data[i + 1][1] + yOffset, radius); // Ƣ�� ����� �׸���
			}

			if (stream_data[i + 1][1] == ofGetHeight()) break; // ���� �ٴڿ� ������� break
			else if (stream_data[i + 1][0] == ofGetWidth()) break; //���� �߷¹��� �ٴڿ� ������� break
			else if (stream_data[i + 1][1] == 0) break; //���� �߷¹��� �ٴڿ� ������� break
			else if (stream_data[i + 1][0] == 0) break; //���� �߷¹��� �ٴڿ� ������� break
			else continue; //�ƴϸ� ���
		}

		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) { //���� ���콺 ��ư�� ������
			ofSetRectMode(OF_RECTMODE_CENTER); //�������� �簢�� �߽����� ����
			if ((ofGetMouseX()-25 < tap_x) && (ofGetMouseX() + 25 > tap_x) && (ofGetMouseY() - 25 < tap_y) && (ofGetMouseY() + 25 > tap_y)) { // �� ������ ���� ������
				if (water_type == 0) ofSetColor(255); // �߼��̸� �������
				else if (water_type == 1) ofSetColor(219, 68, 85); // �꼺�̸� ����������
				else if (water_type == 2) ofSetColor(75, 137, 220); // ���⼺�̸� Ǫ��������
				ofDrawRectangle(ofGetMouseX(), ofGetMouseY(), 50, 50); // Draw a 50 x 50 rect centered over the mouse
			}
		}
	}
}

void water::calculationPath(int ** line_data, int num_of_line, int change_gravity)
{ //�� ��θ� ������ش�.
	int tap_x = stream_data[0][0], tap_y = stream_data[0][1]; // ���� �����°��� ��ǥ�� ���� �����Ѵ�.
	int nearest_i; //���� ����� ���� �ε��� ����

	for (int i = 1; ; i++) {
		nearest_i = findNearestLine(line_data, num_of_line, tap_x, tap_y, change_gravity); //���������� ���� ����� ���� ã�´�.
		if (change_gravity == 0) {
			if (nearest_i == -1) { //�ٴڿ� ������
				stream_data[i][0] = tap_x; // ���� x ��ǥ�� ����
				stream_data[i][1] = ofGetHeight(); //y��ǥ�� �ٴ� ���̿� ����.
				break;
			}

			stream_data[i][0] = tap_x; // ���� x ��ǥ�� ����
			stream_data[i][1] = tap_y - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //y��ǥ�� ���� y��ǥ���� ���� ����� �������� �Ÿ���ŭ�� �� ���̴�.
			i++;

			if (line_data[nearest_i][1] > line_data[nearest_i][3]) { //���� ����� ���� �������� ���� �帣��
				tap_x = line_data[nearest_i][0]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][1]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
			else {
				tap_x = line_data[nearest_i][2]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][3]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
		}
		else if (change_gravity == 1) {
			if (nearest_i == -1) { //�ٴڿ� ������
				stream_data[i][1] = tap_y; // ���� y ��ǥ�� ����
				stream_data[i][0] = ofGetWidth(); //x��ǥ�� â�� �ʺ�� ����.
				break;
			}
			stream_data[i][1] = tap_y; // ���� y ��ǥ�� ����
			stream_data[i][0] = tap_x - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //x��ǥ�� ���� x��ǥ���� ���� ����� �������� �Ÿ���ŭ�� �� ���̴�.
			i++;
			if (line_data[nearest_i][0] > line_data[nearest_i][2]) { //���� ����� ���� ���� �帣�� ����
				tap_x = line_data[nearest_i][0]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][1]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
			else {
				tap_x = line_data[nearest_i][2]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][3]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
		}
		else if (change_gravity == 2) {
			if (nearest_i == -1) { //�ٴڿ� ������
				stream_data[i][0] = tap_x; // ���� x ��ǥ�� ����
				stream_data[i][1] = 0; //y��ǥ�� õ�� ���̿� ����.
				break;
			}
			stream_data[i][0] = tap_x; // ���� x ��ǥ�� ����
			stream_data[i][1] = tap_y - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //y��ǥ�� ���� y��ǥ���� ���� ����� �������� �Ÿ���ŭ�� �� ���̴�.
			i++;
			if (line_data[nearest_i][1] < line_data[nearest_i][3]) { //���� ����� ���� ���� �帣�� ����
				tap_x = line_data[nearest_i][0]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][1]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
			else {
				tap_x = line_data[nearest_i][2]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][3]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
		}
		else {
			if (nearest_i == -1) { //�ٴڿ� ������
				stream_data[i][1] = tap_y; // ���� y ��ǥ�� ����
				stream_data[i][0] = 0; //x��ǥ�� ������ ���� ����.
				break;
			}
			stream_data[i][1] = tap_y; // ���� y ��ǥ�� ����
			stream_data[i][0] = tap_x - distance(line_data[nearest_i], tap_x, tap_y, change_gravity); //x��ǥ�� ���� x��ǥ���� ���� ����� �������� �Ÿ���ŭ�� �� ���̴�.
			i++;
			if (line_data[nearest_i][0] < line_data[nearest_i][2]) { //���� ����� ���� ���� �帣�� ����
				tap_x = line_data[nearest_i][0]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][1]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
			else {
				tap_x = line_data[nearest_i][2]; //���� �帣�� �� x��ǥ�� �����Ѵ�.
				tap_y = line_data[nearest_i][3]; //���� �帣�� �� y��ǥ�� �����Ѵ�.
			}
		}

		stream_data[i][0] = tap_x; //���� �帣�� �� x��ǥ�� �����Ѵ�.
		stream_data[i][1] = tap_y; //���� �帣�� �� y��ǥ�� �����Ѵ�.
	}
	draw_path_flag = 1; //�� ��ΰ� ���Ǿ���.
}

int water::distance(int * line_data, int tap_x, int tap_y, int change_gravity) //���� �������� �Ÿ��� ����Ѵ�.
{
	float slope = (float(line_data[3] - line_data[1]) / float(line_data[2] - line_data[0])); //���⸦ ����Ѵ�.
	
	float end_y = (slope *(tap_x - line_data[0])) + line_data[1]; // ������ �������� �������� �� �ٷ� ��� �κ��� y��ǥ

	float end_x = ((tap_y - line_data[1]) / slope) + line_data[0]; // ������ �������� �������� �� �ٷ� ��� �κ��� x��ǥ
	
	if (change_gravity == 0 || change_gravity == 2)
		return (int)(tap_y - end_y); // ���� ���� ������ �Ÿ��� return�Ѵ�.
	else if(change_gravity==1 || change_gravity == 3)
		return (int)(tap_x - end_x); // ���� ���� ������ �Ÿ��� return�Ѵ�.

}

int water::findNearestLine(int** line_data, int num_of_line, int tap_x, int tap_y, int change_gravity) // ���� ����� ���� ã���ش�.
{
	int nearest_i = -1; //�� ���� �ٺ��� Ž�� ����

	if (change_gravity == 0) {
		int nearest_y = ofGetHeight(); // �ʱ� ����� y��ǥ�� �ٴڳ��̷� ����
		for (int i = 0; i < num_of_line; i++) { //��� �� Ž��
			if (line_data[i][1] < tap_y || line_data[i][3] < tap_y) continue; //���� ���� y��ǥ�� ������ǥ���� ���� �ִٸ� �������� continue
			if (MIN(line_data[i][0], line_data[i][2]) > tap_x || MAX(line_data[i][0], line_data[i][2]) < tap_x) continue; //���� �������� �������� �� �� ��� ���̸� �������� continue

			int min_y = MIN(line_data[i][1], line_data[i][3]); //���� y��ǥ �� ���ʿ� �ִ� �� min_y�� ����

			if (nearest_y > min_y) { // ������ ã�Ƴ��� ����� y��ǥ���� ���� ã���� �� ���� �ִٸ�
				nearest_y = min_y; //���� ã�Ƴ��� y��ǥ�� update
				nearest_i = i; //���� ã�Ƴ��� �ٷ� update
			}
		}
	}
	else if (change_gravity == 1) {
		int nearest_x = ofGetWidth(); // �ʱ� ����� x��ǥ�� â�� �ʺ�� ����
		for (int i = 0; i < num_of_line; i++) { //��� �� Ž��
			if (line_data[i][0] < tap_x || line_data[i][2] < tap_x) continue; //���� ���� x��ǥ�� ������ǥ���� ���ʿ� �ִٸ� �������� continue
			if (MIN(line_data[i][1], line_data[i][3]) > tap_y || MAX(line_data[i][1], line_data[i][3]) < tap_y) continue; //���� �������� �������� �� �� ��� ���̸� �������� continue

			int min_x = MIN(line_data[i][0], line_data[i][2]); //���� x��ǥ �� �����ʿ� �ִ� �� min_x�� ����

			if (nearest_x > min_x) { // ������ ã�Ƴ��� ����� x��ǥ���� ���� ã���� �� ���ʿ� �ִٸ�
				nearest_x = min_x; //���� ã�Ƴ��� x��ǥ�� update
				nearest_i = i; //���� ã�Ƴ��� �ٷ� update
			}
		}
	}
	else if (change_gravity == 2) {
		int nearest_y = 0; // �ʱ� ����� y��ǥ�� õ����̷� ����
		for (int i = 0; i < num_of_line; i++) { //��� �� Ž��
			if (line_data[i][1] > tap_y || line_data[i][3] > tap_y) continue; //���� ���� y��ǥ�� ������ǥ���� �Ʒ��� �ִٸ� �������� continue
			if (MIN(line_data[i][0], line_data[i][2]) > tap_x || MAX(line_data[i][0], line_data[i][2]) < tap_x) continue; //���� ���� �������� �� �� ��� ���̸� �������� continue

			int max_y = MAX(line_data[i][1], line_data[i][3]); //���� y��ǥ �� ���ʿ� �ִ� �� max_y�� ����

			if (nearest_y < max_y) { // ������ ã�Ƴ��� ����� y��ǥ���� ���� ã���� �� �Ʒ��� �ִٸ�
				nearest_y = max_y; //���� ã�Ƴ��� y��ǥ�� update
				nearest_i = i; //���� ã�Ƴ��� �ٷ� update
			}
		}
	}
	else {
		int nearest_x = 0; // �ʱ� ����� x��ǥ�� ���� ������ ����
		for (int i = 0; i < num_of_line; i++) { //��� �� Ž��
			if (line_data[i][0] > tap_x || line_data[i][2] > tap_x) continue; //���� ���� x��ǥ�� ������ǥ���� �����ʿ� �ִٸ� �������� continue
			if (MIN(line_data[i][1], line_data[i][3]) > tap_y || MAX(line_data[i][1], line_data[i][3]) < tap_y) continue; //���� �������� �������� �� �� ��� ���̸� �������� continue

			int max_x = MAX(line_data[i][0], line_data[i][2]); //���� x��ǥ �� �����ʿ� �ִ� �� max_x�� ����

			if (nearest_x < max_x) { // ������ ã�Ƴ��� ����� x��ǥ���� ���� ã���� �� �����ʿ� �ִٸ�
				nearest_x = max_x; //���� ã�Ƴ��� x��ǥ�� update
				nearest_i = i; //���� ã�Ƴ��� �ٷ� update
			}
		}
	}
	return nearest_i; //ã�� ���� ����� ���� �ε��� ��ȯ
}