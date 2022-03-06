#pragma once
#include "ofMain.h"

class water {
	public:
		water(int num_of_line); // 물 경로 정보 저장할 배열 동적할당
		~water(); // 물 경로 정보 저장한 배열 메모리 해제 

		int draw_path_flag; // 물의 경로를 계산할건지
	
		int ** stream_data; //물의 경로 정보 저장할 이차원 배열
		int num_of_path; //물의 경로의 직선의 개수

		void draw(int water_type, int tap_x, int tap_y); //물의 경로 그려주기

		void calculationPath(int** line_data, int num_of_line, int change_gravity); //물의 경로 좌표 계산
		int distance(int* line_data, int tap_x, int tap_y, int change_gravity); // 점과 직선사이의 거리
		int findNearestLine(int** line_data, int num_of_line, int tap_x, int tap_y, int change_gravity); // 현재 점에서 중력방향으로 물이 떨어졌을 때 가장 먼저 만나는 선 찾기
};