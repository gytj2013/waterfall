#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(20); // Limit the speed of our program to 20 frames per second

	// We still want to draw on a black background, so we need to draw
	// the background before we do anything with the brush
	ofBackground(226, 226, 226); //sets the background color to gray
	ofSetLineWidth(4); // set line width to 4

	black.set(0, 0, 0); //black의 r,g,b 값 설정
	red.set(255, 0, 0); //red의 r,g,b 값 설정

	draw_flag = 0;  //초기 draw_flag 0으로 reset
	load_flag = 0; //초기 load_flag 0으로 reset
	draw_stream_flag = 0; // 물이 안나오게 0으로 초기화, 1이면 물 경로 출력
	change_gravity = 0; // 중력 남쪽으로 초기화 (남:0, 서:1, 북:2, 동:3)

	tap = 0; // 물 나오는 곳 초기화(default값 : 0, 맨왼쪽 점)
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetLineWidth(3);//굵기 3
	if (draw_flag) { // draw_flag가 1로 set되면 실행

		/* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
		 Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
		 */

		 //draw line
		drawLines();
		//draw dot
		drawDots();

		if (draw_stream_flag) { //물 경로를 출력할거면
			drawStream(); // draw stream
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'v') {
		// HACK: only needed on windows, when using ofSetAutoBackground(false)
		glReadBuffer(GL_FRONT);
		ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
	}
	if (key == 'q') { // q 누르면 동적할당 해제하고 프로그램 종료하기
		// Reset flags
		draw_flag = 0; // draw 실행 안하기

		// Free the dynamically allocated memory exits.

		for (int i = 0; i < num_of_line; i++) { //줄 정보 담았던 이차원 배열 메모리 해제
			delete line_data[i];
		}
		delete line_data;

		for (int i = 0; i < num_of_dot; i++) {  //점 정보 담았던 이차원 배열 메모리 해제
			delete dot_data[i];
		}
		delete dot_data;

		delete water_type; //물의 종류 정보 배열 메모리 해제

		cout << "Dynamically allocated memory has been freed." << endl; //동적할당 끝났다고 콘솔창에 출력

		_Exit(0); //프로세스 종료하기
	}
	if (key == 'd') { // d 누르면 draw() 실행하기
		if (!load_flag) return; //파일이 load되지 않았으면 그냥 return하기

		/* COMSIL1-TODO 2: This is draw control part.
		You should draw only after when the key 'd' has been pressed.
		*/

		draw_flag = 1; //d가 눌리면 draw_flag 1로 set
	}
	if (key == 's') { //s 누르면 물 나오게 하기
		// 2nd week portion.
		if (!load_flag) return; //파일이 load되지 않았으면 그냥 return하기
		initializeWaterLines(); //물 경로 초기화
		draw_stream_flag = 1; // 물 경로 출력하게 1으로 set
	}
	if (key == 'e') { //e 누르면 물 멎게 하기
		// 2nd week portion.
		if (!load_flag) return; //파일이 load되지 않았으면 그냥 return하기
		initializeWaterLines(); //물 경로 초기화
		draw_stream_flag = 0; // 물이 안나오게 0으로 초기화
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'l') { //ㅣ이 눌리면 파일 load하기
		// Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

		// Check whether the user opened a file
		if (openFileResult.bSuccess) {
			ofLogVerbose("User selected a file"); // 파일 읽어왔으면 파일 골랐다고 콘솔창에 문구 출력

			// We have a file, so let's check it and process it
			processOpenFileSelection(openFileResult);
			load_flag = 1; // 파일이 load됐으면 1로 set
		}
	}

	/* COMSIL1-TODO 4: This is selection dot control part.
	 You can select dot in which water starts to flow by left, right direction key (<- , ->).
	 */

	if (key == OF_KEY_RIGHT) { // >를 눌렀으면 
		if (++tap >= num_of_dot) // 더 이상 왼쪽에 점이 없으면
			tap = 0; // 맨 오른쪽으로 이동
		cout << "Selcted Dot Coordinate is (" << dot_data[tap][0] << "," << dot_data[tap][1] << ")" << endl; //선택한 점의 위치 출력
	}
	if (key == OF_KEY_LEFT) { // <를 눌렀으면
		if (--tap < 0) // 더 이상 오른쪽에 점이 없으면 
			tap = num_of_dot - 1; // 맨 오른쪽으로 이동
		cout << "Selcted Dot Coordinate is (" << dot_data[tap][0] << "," << dot_data[tap][1] << ")" << endl; //선택한 점의 위치 출력
	}
	if (key == 'g') { // g누르면 중력 위치 바꾸기
		change_gravity++; // 중력 위치 변수 다음으로 1 증가
		if (change_gravity == 4) // 한바퀴 돌았으면 다시 남쪽
			change_gravity = 0; //0으로 초기화
		if (change_gravity == 0) // change_gravity 변수가 0이면
			cout << "gravity is in the south" << endl; // 중력은 남쪽에 있다고 메시지 출력
		else if (change_gravity == 1) // change_gravity 변수가 1이면
			cout << "gravity is in the east" << endl; // 중력은 동쪽에 있다고 메시지 출력
		else if (change_gravity == 2) // change_gravity 변수가 2이면
			cout << "gravity is in the north" << endl; // 중력은 북쪽에 있다고 메시지 출력
		else // change_gravity 변수가 3이면
			cout << "gravity is in the west" << endl; // 중력은 서쪽에 있다고 메시지 출력
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	//Path to the comma delimited file
	//string fileName = "input.txt";

	string fileName = openFileResult.getName();
	ofFile file(fileName);

	if (!file.exists()) cout << "Target file does not exists." << endl; //못 찾았다고 메시지 출력
	else cout << "We found the target file." << endl; // 찾았다고 메시지 출력

	ofBuffer buffer(file);

	/* This variable is for indicating which type of input is being received.
	 IF input_type == 0, then work of getting line input is in progress.
	 IF input_type == 1, then work of getting dot input is in progress.
	 */
	int input_type = 0;
	int count = 0; // 개수 세는 변수

	/* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
	 You must maintain those information. But, currently below code is not complete.
	 Also, note that all of coordinate should not be out of screen size.
	 However, all of coordinate do not always turn out to be the case.
	 So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
	*/

	// Read file line by line
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
		string line = *it;

		// Split line into strings
		vector<string> words = ofSplitString(line, " ");

		if (words.size() == 1) { // 수가 하나만 있으면 줄 또는 점의 개수
			if (input_type == 0) { // Input for the number of lines.
				num_of_line = atoi(words[0].c_str()); // 줄 수 txt 파일에서 읽어오기
				cout << "The number of line is: " << num_of_line << endl; // 줄 수 콘솔 창에 출력

				line_data = new int* [num_of_line]; // 동적할당
				for (int i = 0; i < num_of_line; i++) { //줄 수만큼
					line_data[i] = new int[4]; //ofDrawLine(x1,y1,x2,y2):line from (x1,y2) to (x1,y2) 이니까 x1,x2,y1,y2 정보 담을 배열 할당
				}
			}
			else { // Input for the number of dots.
				num_of_dot = atoi(words[0].c_str()); // 점 개수 txt 파일에서 읽어오기
				cout << "The number of dot is: " << num_of_dot << endl; // 점 개수 콘솔 창에 출력

				dot_data = new int* [num_of_dot]; // 동적할당
				for (int i = 0; i < num_of_dot; i++) { //점 개수만큼
					dot_data[i] = new int[2]; // ofDrawCircle(x1,y1,10):centered at (x1,y1), radius of 10 이니까 x1,y1 정보 담을 배열 할당
				}
			}
		}
		else if (words.size() >= 2) { // x, y 정보 부분이면
			int x1, y1, x2, y2; // 좌표 변수
			if (input_type == 0) { // Input for actual information of lines
				x1 = atoi(words[0].c_str()); // x1 txt 파일에서 읽어오기
				y1 = atoi(words[1].c_str()); // y1 txt 파일에서 읽어오기
				x2 = atoi(words[2].c_str()); // x2 txt 파일에서 읽어오기
				y2 = atoi(words[3].c_str()); // y2 txt 파일에서 읽어오기

				if (x1 < 0 || x2 > ofGetWidth()) return; //ofGetWidth:프로그램 창의 너비 크기
				if (y1 <= 0 || y2 <= 0 || y1 > ofGetHeight() || y2 > ofGetHeight()) return; // 프로그램 창의 높이 크기, y축은 0보다 크다

				line_data[count][0] = x1; //x1 정보 배열에 저장하기
				line_data[count][1] = y1; //y1 정보 배열에 저장하기
				line_data[count][2] = x2; //x2 정보 배열에 저장하기
				line_data[count][3] = y2; //y2 정보 배열에 저장하기
			}
			else { // Input for actual information of dots.
				x1 = atoi(words[0].c_str()); // x1 txt 파일에서 읽어오기
				y1 = atoi(words[1].c_str()); // y1 txt 파일에서 읽어오기

				if (x1 < 0 || x1 > ofGetWidth()) return; //x1이 프로그램 창의 너비보단 같거나 작고 0보단 크거나 같아야 함.
				if (y1 <= 0 || y1 > ofGetHeight()) return; //y1이 x축보다는 높아야 하고 프로그램 창의 높이 크기보다는 작아야 함.

				dot_data[count][0] = x1; //x1 정보 배열에 저장하기
				dot_data[count][1] = y1; //y1 정보 배열에 저장하기
			}

			count++; // 개수 증가

			if (count == num_of_line) { // 줄의 개수만큼 정보를 받아들였으면 이제는 점의 정보 받아들일 차례
				input_type = 1; //dot input 받기
				count = 0; //count 초기화
			}
		} // End of else if.
	} // End of for-loop (Read file line by line).

	setWaterType(); // 물 종류 지정

	initializeWaterLines(); //물 경로 초기화
}

void ofApp::initializeWaterLines() {
	// initialize
	if (stream.empty()) { //경로에 대한 정보가 없다면
		stream.assign(1, water(num_of_line)); //경로 할당
	}
	else { //경로에 대한 정보가 존재하면
		for (unsigned int i = 0; i < stream.size(); i++) { //모든 경로를
			stream[i].draw_path_flag = 0; // draw_path_flag 0으로 reset
			stream[i].stream_data[i][0] = stream[i].stream_data[i][1] = -1; // 물 경로 정보를 담은 배열 초기화
		}
	}
	stream[0].stream_data[0][0] = dot_data[tap][0]; //물이 나오는 곳의 x 좌표 저장
	stream[0].stream_data[0][1] = dot_data[tap][1]; //물이 나오는 곳의 y 좌표 저장
}

void ofApp::drawLines() {
	for (int i = 0; i < num_of_line; i++) { // 줄 수 만큼
		ofSetColor(red); // 빨간색
		ofDrawLine(line_data[i][0], line_data[i][1], line_data[i][2], line_data[i][3]); //draw line between two points (line_data[i][0], line_data[i][1]), (line_data[i][2], line_data[i][3])
	}
}

void ofApp::drawDots() {
	for (int i = 0; i < num_of_dot; i++) { // 점의 개수 만큼
		if (i == tap) { // 물이 나오는 곳이면
			ofSetColor(red); // 빨간색으로 설정
		}
		else { //물 나오는 곳이 아니면
			ofSetColor(black); //검정색으로 설정
		}
		ofDrawCircle(dot_data[i][0], dot_data[i][1], 10); //draw a circle, centered at (dot_data[i][0], dot_data[i][1]), with radius 10
	}
}

void ofApp::setWaterType() {
	water_type = new int [num_of_dot]; // 동적할당
	for (int i = 0; i < num_of_dot; i++) // 점의 개수 만큼
		water_type[i] = ofRandom(0, 3); //0,1,2중 랜덤하게
}

void ofApp::drawStream() {
	ofSetLineWidth(5); // set line width to 5
	for (unsigned int i = 0; i < stream.size(); i++) { // 물의 경로의 개수만큼(직선의 개수)
		if (!stream[i].draw_path_flag) //물의 경로가 계산되어있지 않으면
			stream[i].calculationPath(line_data, num_of_line, change_gravity); // 물의 경로 좌표를 계산한다.
		stream[i].draw(water_type[tap], dot_data[tap][0], dot_data[tap][1]); //물의 경로를 그린다.
	}
}