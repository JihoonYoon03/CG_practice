#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>

#include "tools.h"

constexpr auto winWidth = 800;
constexpr auto winHeight = 600;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

class Board;

class Rect {
	friend class Board;

	rtPos pos = { 0 };
	ColorRGB color = { 0.0f, 0.0f, 0.0f };
public:
	Rect(rtPos p, ColorRGB c) : pos(p), color(c) {}

	void draw() {
		glColor3f(color.r, color.g, color.b);
		glRectf(pos.x1, pos.y1, pos.x2, pos.y2);
	}
};

Rect dataSet[10] = {
	// 머리 부분 (오른쪽 끝)
	{{-0.3f, 0.8f, -0.1f, 0.6f}, {0.2f, 0.8f, 0.3f}},

	// 머리
	{{-0.5f, 0.8f, -0.3f, 0.6f}, {0.9f, 0.1f, 0.7f}},

	// 목
	{{-0.5f, 0.6f, -0.3f, 0.4f}, {0.1f, 0.6f, 0.9f}},  
	{{-0.5f, 0.4f, -0.3f, 0.2f}, {0.3f, 0.9f, 0.2f}},  

	// 몸통
	{{-0.9f, 0.2f, -0.3f, 0.0f}, {0.7f, 0.4f, 0.1f}},  
	{{-0.9f, 0.0f, -0.5f, -0.2f}, {0.8f, 0.2f, 0.9f}}, 

	// 앞다리
	{{-0.5f, 0.0f, -0.3f, -0.4f}, {0.9f, 0.8f, 0.1f}}, 
	{{-0.5f, -0.4f, -0.3f, -0.8f}, {0.4f, 0.1f, 0.8f}},

	// 뒷다리
	{{-0.9f, -0.2f, -0.7f, -0.4f}, {0.6f, 0.3f, 0.5f}},
	{{-0.9f, -0.4f, -0.7f, -0.8f}, {0.2f, 0.5f, 0.7f}} 
};


class Board {
	std::vector<Rect> rects;

public:
	Board() {
		for(int i = 0; i < 10; i++) {
			rects.push_back(dataSet[i]);
		}
	}

	void draw() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (auto& rect : rects) {
			rect.draw();
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void reroll() {
		rects.clear();
		for (int i = 0; i < 10; i++) {
			rects.push_back(dataSet[i]);
		}
	}
};

Board board;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	srand((unsigned int)time(NULL));
	board.reroll();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse); // 마우스 이벤트 콜백 함수 지정

	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	board.draw();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
		
		}
		break;
	}
}