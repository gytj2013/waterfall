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

	ofColor red; //������ rgb�� ������ ����
	ofColor black; //������ rgb�� ������ ����

	// flag variables
	int draw_flag; // draw()�� �������� ���� üũ���ִ� flag
	int load_flag; // ������ �ε� �ƴ��� �� �ƴ��� üũ���ִ� flag
	int draw_stream_flag; // �� ��θ� ��������� ���� üũ���ִ� flag
	int change_gravity; // �߷� ��ġ �ٲٱ�

	// Line segment and dot related variables
	int num_of_line, num_of_dot; // �� ��, �� ��

	int** line_data; // ���� ������ ���� ������ �迭
	int** dot_data; // ���� ������ ���� ������ �迭
	int* water_type; // ���� ���� �˷��ִ� �迭 (0 : �߼�, 1 : �꼺, 2 : ���⼺)

	int tap; //���� ������ ��

	vector<water> stream;

	/* WaterFall-related member functions */

	void processOpenFileSelection(ofFileDialogResult openFileResult);//�˸��� ���� �����ؼ� �ȿ� �ִ� ���� �迭�� �Ҵ�
	void initializeWaterLines(); //�� ��� �ʱ�ȭ
	void drawLines(); // �� �׸��� �Լ�
	void drawDots(); // �� ������ �� �׸��� �Լ�
	void drawStream(); // �� ��� �׸��� �Լ�
	void setWaterType(); // �� ���� �������ִ� �Լ�
};