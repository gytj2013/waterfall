#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(20); // Limit the speed of our program to 20 frames per second

	// We still want to draw on a black background, so we need to draw
	// the background before we do anything with the brush
	ofBackground(226, 226, 226); //sets the background color to gray
	ofSetLineWidth(4); // set line width to 4

	black.set(0, 0, 0); //black�� r,g,b �� ����
	red.set(255, 0, 0); //red�� r,g,b �� ����

	draw_flag = 0;  //�ʱ� draw_flag 0���� reset
	load_flag = 0; //�ʱ� load_flag 0���� reset
	draw_stream_flag = 0; // ���� �ȳ����� 0���� �ʱ�ȭ, 1�̸� �� ��� ���
	change_gravity = 0; // �߷� �������� �ʱ�ȭ (��:0, ��:1, ��:2, ��:3)

	tap = 0; // �� ������ �� �ʱ�ȭ(default�� : 0, �ǿ��� ��)
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetLineWidth(3);//���� 3
	if (draw_flag) { // draw_flag�� 1�� set�Ǹ� ����

		/* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
		 Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
		 */

		 //draw line
		drawLines();
		//draw dot
		drawDots();

		if (draw_stream_flag) { //�� ��θ� ����ҰŸ�
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
	if (key == 'q') { // q ������ �����Ҵ� �����ϰ� ���α׷� �����ϱ�
		// Reset flags
		draw_flag = 0; // draw ���� ���ϱ�

		// Free the dynamically allocated memory exits.

		for (int i = 0; i < num_of_line; i++) { //�� ���� ��Ҵ� ������ �迭 �޸� ����
			delete line_data[i];
		}
		delete line_data;

		for (int i = 0; i < num_of_dot; i++) {  //�� ���� ��Ҵ� ������ �迭 �޸� ����
			delete dot_data[i];
		}
		delete dot_data;

		delete water_type; //���� ���� ���� �迭 �޸� ����

		cout << "Dynamically allocated memory has been freed." << endl; //�����Ҵ� �����ٰ� �ܼ�â�� ���

		_Exit(0); //���μ��� �����ϱ�
	}
	if (key == 'd') { // d ������ draw() �����ϱ�
		if (!load_flag) return; //������ load���� �ʾ����� �׳� return�ϱ�

		/* COMSIL1-TODO 2: This is draw control part.
		You should draw only after when the key 'd' has been pressed.
		*/

		draw_flag = 1; //d�� ������ draw_flag 1�� set
	}
	if (key == 's') { //s ������ �� ������ �ϱ�
		// 2nd week portion.
		if (!load_flag) return; //������ load���� �ʾ����� �׳� return�ϱ�
		initializeWaterLines(); //�� ��� �ʱ�ȭ
		draw_stream_flag = 1; // �� ��� ����ϰ� 1���� set
	}
	if (key == 'e') { //e ������ �� �ܰ� �ϱ�
		// 2nd week portion.
		if (!load_flag) return; //������ load���� �ʾ����� �׳� return�ϱ�
		initializeWaterLines(); //�� ��� �ʱ�ȭ
		draw_stream_flag = 0; // ���� �ȳ����� 0���� �ʱ�ȭ
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'l') { //���� ������ ���� load�ϱ�
		// Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

		// Check whether the user opened a file
		if (openFileResult.bSuccess) {
			ofLogVerbose("User selected a file"); // ���� �о������ ���� ����ٰ� �ܼ�â�� ���� ���

			// We have a file, so let's check it and process it
			processOpenFileSelection(openFileResult);
			load_flag = 1; // ������ load������ 1�� set
		}
	}

	/* COMSIL1-TODO 4: This is selection dot control part.
	 You can select dot in which water starts to flow by left, right direction key (<- , ->).
	 */

	if (key == OF_KEY_RIGHT) { // >�� �������� 
		if (++tap >= num_of_dot) // �� �̻� ���ʿ� ���� ������
			tap = 0; // �� ���������� �̵�
		cout << "Selcted Dot Coordinate is (" << dot_data[tap][0] << "," << dot_data[tap][1] << ")" << endl; //������ ���� ��ġ ���
	}
	if (key == OF_KEY_LEFT) { // <�� ��������
		if (--tap < 0) // �� �̻� �����ʿ� ���� ������ 
			tap = num_of_dot - 1; // �� ���������� �̵�
		cout << "Selcted Dot Coordinate is (" << dot_data[tap][0] << "," << dot_data[tap][1] << ")" << endl; //������ ���� ��ġ ���
	}
	if (key == 'g') { // g������ �߷� ��ġ �ٲٱ�
		change_gravity++; // �߷� ��ġ ���� �������� 1 ����
		if (change_gravity == 4) // �ѹ��� �������� �ٽ� ����
			change_gravity = 0; //0���� �ʱ�ȭ
		if (change_gravity == 0) // change_gravity ������ 0�̸�
			cout << "gravity is in the south" << endl; // �߷��� ���ʿ� �ִٰ� �޽��� ���
		else if (change_gravity == 1) // change_gravity ������ 1�̸�
			cout << "gravity is in the east" << endl; // �߷��� ���ʿ� �ִٰ� �޽��� ���
		else if (change_gravity == 2) // change_gravity ������ 2�̸�
			cout << "gravity is in the north" << endl; // �߷��� ���ʿ� �ִٰ� �޽��� ���
		else // change_gravity ������ 3�̸�
			cout << "gravity is in the west" << endl; // �߷��� ���ʿ� �ִٰ� �޽��� ���
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

	if (!file.exists()) cout << "Target file does not exists." << endl; //�� ã�Ҵٰ� �޽��� ���
	else cout << "We found the target file." << endl; // ã�Ҵٰ� �޽��� ���

	ofBuffer buffer(file);

	/* This variable is for indicating which type of input is being received.
	 IF input_type == 0, then work of getting line input is in progress.
	 IF input_type == 1, then work of getting dot input is in progress.
	 */
	int input_type = 0;
	int count = 0; // ���� ���� ����

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

		if (words.size() == 1) { // ���� �ϳ��� ������ �� �Ǵ� ���� ����
			if (input_type == 0) { // Input for the number of lines.
				num_of_line = atoi(words[0].c_str()); // �� �� txt ���Ͽ��� �о����
				cout << "The number of line is: " << num_of_line << endl; // �� �� �ܼ� â�� ���

				line_data = new int* [num_of_line]; // �����Ҵ�
				for (int i = 0; i < num_of_line; i++) { //�� ����ŭ
					line_data[i] = new int[4]; //ofDrawLine(x1,y1,x2,y2):line from (x1,y2) to (x1,y2) �̴ϱ� x1,x2,y1,y2 ���� ���� �迭 �Ҵ�
				}
			}
			else { // Input for the number of dots.
				num_of_dot = atoi(words[0].c_str()); // �� ���� txt ���Ͽ��� �о����
				cout << "The number of dot is: " << num_of_dot << endl; // �� ���� �ܼ� â�� ���

				dot_data = new int* [num_of_dot]; // �����Ҵ�
				for (int i = 0; i < num_of_dot; i++) { //�� ������ŭ
					dot_data[i] = new int[2]; // ofDrawCircle(x1,y1,10):centered at (x1,y1), radius of 10 �̴ϱ� x1,y1 ���� ���� �迭 �Ҵ�
				}
			}
		}
		else if (words.size() >= 2) { // x, y ���� �κ��̸�
			int x1, y1, x2, y2; // ��ǥ ����
			if (input_type == 0) { // Input for actual information of lines
				x1 = atoi(words[0].c_str()); // x1 txt ���Ͽ��� �о����
				y1 = atoi(words[1].c_str()); // y1 txt ���Ͽ��� �о����
				x2 = atoi(words[2].c_str()); // x2 txt ���Ͽ��� �о����
				y2 = atoi(words[3].c_str()); // y2 txt ���Ͽ��� �о����

				if (x1 < 0 || x2 > ofGetWidth()) return; //ofGetWidth:���α׷� â�� �ʺ� ũ��
				if (y1 <= 0 || y2 <= 0 || y1 > ofGetHeight() || y2 > ofGetHeight()) return; // ���α׷� â�� ���� ũ��, y���� 0���� ũ��

				line_data[count][0] = x1; //x1 ���� �迭�� �����ϱ�
				line_data[count][1] = y1; //y1 ���� �迭�� �����ϱ�
				line_data[count][2] = x2; //x2 ���� �迭�� �����ϱ�
				line_data[count][3] = y2; //y2 ���� �迭�� �����ϱ�
			}
			else { // Input for actual information of dots.
				x1 = atoi(words[0].c_str()); // x1 txt ���Ͽ��� �о����
				y1 = atoi(words[1].c_str()); // y1 txt ���Ͽ��� �о����

				if (x1 < 0 || x1 > ofGetWidth()) return; //x1�� ���α׷� â�� �ʺ񺸴� ���ų� �۰� 0���� ũ�ų� ���ƾ� ��.
				if (y1 <= 0 || y1 > ofGetHeight()) return; //y1�� x�ຸ�ٴ� ���ƾ� �ϰ� ���α׷� â�� ���� ũ�⺸�ٴ� �۾ƾ� ��.

				dot_data[count][0] = x1; //x1 ���� �迭�� �����ϱ�
				dot_data[count][1] = y1; //y1 ���� �迭�� �����ϱ�
			}

			count++; // ���� ����

			if (count == num_of_line) { // ���� ������ŭ ������ �޾Ƶ鿴���� ������ ���� ���� �޾Ƶ��� ����
				input_type = 1; //dot input �ޱ�
				count = 0; //count �ʱ�ȭ
			}
		} // End of else if.
	} // End of for-loop (Read file line by line).

	setWaterType(); // �� ���� ����

	initializeWaterLines(); //�� ��� �ʱ�ȭ
}

void ofApp::initializeWaterLines() {
	// initialize
	if (stream.empty()) { //��ο� ���� ������ ���ٸ�
		stream.assign(1, water(num_of_line)); //��� �Ҵ�
	}
	else { //��ο� ���� ������ �����ϸ�
		for (unsigned int i = 0; i < stream.size(); i++) { //��� ��θ�
			stream[i].draw_path_flag = 0; // draw_path_flag 0���� reset
			stream[i].stream_data[i][0] = stream[i].stream_data[i][1] = -1; // �� ��� ������ ���� �迭 �ʱ�ȭ
		}
	}
	stream[0].stream_data[0][0] = dot_data[tap][0]; //���� ������ ���� x ��ǥ ����
	stream[0].stream_data[0][1] = dot_data[tap][1]; //���� ������ ���� y ��ǥ ����
}

void ofApp::drawLines() {
	for (int i = 0; i < num_of_line; i++) { // �� �� ��ŭ
		ofSetColor(red); // ������
		ofDrawLine(line_data[i][0], line_data[i][1], line_data[i][2], line_data[i][3]); //draw line between two points (line_data[i][0], line_data[i][1]), (line_data[i][2], line_data[i][3])
	}
}

void ofApp::drawDots() {
	for (int i = 0; i < num_of_dot; i++) { // ���� ���� ��ŭ
		if (i == tap) { // ���� ������ ���̸�
			ofSetColor(red); // ���������� ����
		}
		else { //�� ������ ���� �ƴϸ�
			ofSetColor(black); //���������� ����
		}
		ofDrawCircle(dot_data[i][0], dot_data[i][1], 10); //draw a circle, centered at (dot_data[i][0], dot_data[i][1]), with radius 10
	}
}

void ofApp::setWaterType() {
	water_type = new int [num_of_dot]; // �����Ҵ�
	for (int i = 0; i < num_of_dot; i++) // ���� ���� ��ŭ
		water_type[i] = ofRandom(0, 3); //0,1,2�� �����ϰ�
}

void ofApp::drawStream() {
	ofSetLineWidth(5); // set line width to 5
	for (unsigned int i = 0; i < stream.size(); i++) { // ���� ����� ������ŭ(������ ����)
		if (!stream[i].draw_path_flag) //���� ��ΰ� ���Ǿ����� ������
			stream[i].calculationPath(line_data, num_of_line, change_gravity); // ���� ��� ��ǥ�� ����Ѵ�.
		stream[i].draw(water_type[tap], dot_data[tap][0], dot_data[tap][1]); //���� ��θ� �׸���.
	}
}