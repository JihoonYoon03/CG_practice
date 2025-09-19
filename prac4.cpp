#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>

#include "tools.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);

class Rect {
	rtPos pos = { 0 };
	GLfloat r, g, b;
	GLfloat OriginX, OriginY;	// 사각형 시작 위치

	GLfloat delX = 0, delY = 0;
public:

	Rect(GLfloat mx, GLfloat my) : OriginX(mx), OriginY(my) {
		pos.x1 = OriginX - 0.05f; pos.y1 = OriginY + 0.05f;
		pos.x2 = OriginX + 0.05f; pos.y2 = OriginY - 0.05f;
		randColor(r, g, b);
	}

	void draw() {
		glColor3f(r, g, b);
		glRectf(pos.x1, pos.y1, pos.x2, pos.y2);
	}

	void resetPos() {
		pos.x1 = OriginX - 0.05f; pos.y1 = OriginY + 0.05f;
		pos.x2 = OriginX + 0.05f; pos.y2 = OriginY - 0.05f;
	}

	rtPos returnPos() {
		return pos;
	}
};;

std::vector<Rect> rects;
GLfloat lastX = 0, lastY = 0;
bool playAnim = false;
int clickIndex = -1;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 이벤트 콜백 함수 지정
	glutMouseFunc(Mouse); // 마우스 이벤트 콜백 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& rect : rects) {
		rect.draw();
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'm':
		for(auto& rect : rects) {
			std::cout << "reset Rect pos\n";
			rect.resetPos();
		}
		break;
	case 's':
		playAnim = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		return;
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && rects.size() < 5) {
			GLfloat xGL, yGL;
			mPosToGL(x, y, xGL, yGL);
			rects.push_back(Rect(xGL, yGL));
		}
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	}
}