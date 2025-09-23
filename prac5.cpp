#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>

#include "tools.h"

constexpr auto defSize = 0.05f;
auto eraserSize = 0.05f;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid MouseMotion(int x, int y);

class Rect {
protected:
	rtPos pos = { 0 };
	ColorRGB color = { 0.0f, 0.0f, 0.0f };
public:
	Rect() {	// 기본 랜덤 사각형 생성
		randSquarePos(pos, 0.05f);
		randColor(color);
	}

	Rect(GLfloat mx, GLfloat my) : pos(pos) {	// 마우스 좌표에 사각형 생성
		pos.x1 = mx - defSize / 2; pos.y1 = my + defSize / 2;
		pos.x2 = mx + defSize / 2; pos.y2 = my - defSize / 2;
		randColor(color);
	}

	void draw() {
		glColor3f(color.r, color.g, color.b);
		glRectf(pos.x1, pos.y1, pos.x2, pos.y2);
	}

	bool checkCollide(Rect& other) {
		if (pos.x1 > other.pos.x2 || pos.x2 < other.pos.x1 ||
			pos.y1 < other.pos.y2 || pos.y2 > other.pos.y1)
			return false;
		else
			return true;
	}

	rtPos returnPos() {
		return pos;
	}

	ColorRGB returnColor() {
		return color;
	}
};

class Eraser : public Rect {
public:
	Eraser(GLfloat mx, GLfloat my) : Rect(mx, my) {
		pos.x1 -= eraserSize / 2; pos.y1 += eraserSize / 2;
		pos.x2 += eraserSize / 2; pos.y2 -= eraserSize / 2;
		color = { 0.0f, 0.0f, 0.0f };
	}

	void drag(GLfloat mx, GLfloat my) {
		GLfloat width = (pos.x2 - pos.x1) / 2;
		GLfloat height = (pos.y1 - pos.y2) / 2;

		pos.x1 = mx - width; pos.y1 = my + width;
		pos.x2 = mx + height; pos.y2 = my - height;
	}

	void erasing(ColorRGB target) {
		pos.x1 -= 0.005f; pos.y1 += 0.005f;
		pos.x2 += 0.005f; pos.y2 -= 0.005f;
		color = target;
	}
};


std::vector<Rect> rects;
std::vector<Eraser> eraser;
GLfloat lastX = 0, lastY = 0;
bool dragging = false;

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

	for (int i = 0; i < 40; i++) {
		rects.push_back(Rect());
	}

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 이벤트 콜백 함수 지정
	glutMouseFunc(Mouse); // 마우스 이벤트 콜백 함수 지정
	glutMotionFunc(MouseMotion); // 마우스 움직임 콜백 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& rect : rects) {
		rect.draw();
	}

	for (auto& er : eraser) {
		er.draw();
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		rects.clear();
		eraserSize = 0.05f;
		dragging = false;
		for (int i = 0; i < 40; i++) {
			rects.push_back(Rect());
		}
		glutPostRedisplay();
		break;
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (dragging == false) {
				dragging = true;
				if (eraser.size() < 1) {
					GLfloat mx = 0, my = 0;
					mPosToGL(x, y, mx, my);
					eraser.push_back(Eraser(mx, my));
				}
				glutPostRedisplay();
			}
		}
		else if (state == GLUT_UP) {
			if (dragging == true) {
				dragging = false;
				eraser.clear();
				glutPostRedisplay();
			}
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN && rects.size() < 40) {
			if (0.05f - rects.size() * 0.005f > 0.01f)
				eraserSize = 0.05f - rects.size() * 0.005f;
			std::cout << "Eraser size: " << eraserSize << std::endl;
			GLfloat xGL, yGL;						 
			mPosToGL(x, y, xGL, yGL);
			rects.push_back(Rect(xGL, yGL));
		}
		break;
	}
}

GLvoid MouseMotion(int x, int y) {
	if (dragging == true) {
		GLfloat mx = 0, my = 0;
		mPosToGL(x, y, mx, my);
		eraser[0].drag(mx, my);
		for (auto it = rects.begin(); it != rects.end(); ) {
			if (eraser[0].checkCollide(*it)) {
				std::cout << "Collide!" << std::endl;
				eraser[0].erasing((*it).returnColor());
				it = rects.erase(it);
			}
			else {
				it++;
			}
		}
		glutPostRedisplay();
	}
}