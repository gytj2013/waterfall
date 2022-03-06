#pragma once
#include "ofMain.h"

class water {
	public:
		water(int num_of_line); // �� ��� ���� ������ �迭 �����Ҵ�
		~water(); // �� ��� ���� ������ �迭 �޸� ���� 

		int draw_path_flag; // ���� ��θ� ����Ұ���
	
		int ** stream_data; //���� ��� ���� ������ ������ �迭
		int num_of_path; //���� ����� ������ ����

		void draw(int water_type, int tap_x, int tap_y); //���� ��� �׷��ֱ�

		void calculationPath(int** line_data, int num_of_line, int change_gravity); //���� ��� ��ǥ ���
		int distance(int* line_data, int tap_x, int tap_y, int change_gravity); // ���� ���������� �Ÿ�
		int findNearestLine(int** line_data, int num_of_line, int tap_x, int tap_y, int change_gravity); // ���� ������ �߷¹������� ���� �������� �� ���� ���� ������ �� ã��
};