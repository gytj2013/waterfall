#pragma once

#include "ofMain.h"
#include "waterObj.h"
#include <vector>

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* WaterFall-related member variables Regions */

	ofColor red; //빨간색 rgb값 저장할 변수
	ofColor black; //검은색 rgb값 저장할 변수

	// flag variables
	int draw_flag; // draw()를 실행할지 말지 체크해주는 flag
	int load_flag; // 파일이 로드 됐는지 안 됐는지 체크해주는 flag
	int draw_stream_flag; // 물 경로를 출력해줄지 말지 체크해주는 flag
	int change_gravity; // 중력 위치 바꾸기

	// Line segment and dot related variables
	int num_of_line, num_of_dot; // 줄 수, 점 수

	int** line_data; // 줄의 정보를 담을 이차원 배열
	int** dot_data; // 점의 정보를 담을 이차원 배열
	int* water_type; // 물의 종류 알려주는 배열 (0 : 중성, 1 : 산성, 2 : 연기성)

	int tap; //물이 나오는 점

	vector<water> stream;

	/* WaterFall-related member functions */

	void processOpenFileSelection(ofFileDialogResult openFileResult);//알맞은 파일 선택해서 안에 있는 내용 배열에 할당
	void initializeWaterLines(); //물 경로 초기화
	void drawLines(); // 선 그리는 함수
	void drawDots(); // 물 나오는 점 그리는 함수
	void drawStream(); // 물 경로 그리는 함수
	void setWaterType(); // 물 종류 지정해주는 함수
};