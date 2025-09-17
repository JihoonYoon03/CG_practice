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
GLvoid MouseMotion(int x, int y);

class Rect {
	GLfloat x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	GLfloat r, g, b;

	GLfloat delX = 0, delY = 0;
public:
	Rect() {
		randRectPos(x1, y1, x2, y2);
		randColor(r, g, b);
	}

	void draw() {
		glColor3f(r, g, b);
		glRectf(x1 + delX, y1 + delY, x2 + delX, y2 + delY);
	}

	bool gotClick(int mx, int my) {
		return isMouseIn(x1, y1, x2, y2, mx, my);
	}

	void drag(GLfloat mx, GLfloat my) {
		delX = mx;
		delY = my;
	}

	void stopDrag() {
		x1 += delX;
		x2 += delX;
		y1 += delY;
		y2 += delY;
		delX = 0;
		delY = 0;
	}
};;

std::vector<Rect> rects;
GLfloat lastX = 0, lastY = 0;
bool dragging = false;
int dragIndex = -1;

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
	glutMotionFunc(MouseMotion); // 마우스 움직임 콜백 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.0f, 0.4f, 0.4f, 1.0f);
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
	case 'a':
	{
		int randScale = rand() % 10 + 1;
		if (randScale > 30 - rects.size())
			randScale = 30 - rects.size();

		for (int i = 0; i < randScale; i++) {
			rects.push_back(Rect());
		}
		glutPostRedisplay();
		break;
	}
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (dragging == false) {
				for (auto& rect : rects) {
					if (rect.gotClick(x, y)) {
						std::cout << "Click!" << std::endl;
						dragging = true;
						dragIndex = &rect - &rects[0];
						mPosToGL(x, y, lastX, lastY);	// 현재 마우스 위치 저장

						// break를 넣지 않아야 맨 위의 사각형을 드래그 할 수 있음
					}
				}

				glutPostRedisplay();
			}
		}
		else if (state == GLUT_UP) {
			if (dragging == true) {
				dragging = false;
				rects[dragIndex].stopDrag();
				dragIndex = -1;

				glutPostRedisplay();
			}
		}
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	}
}

GLvoid MouseMotion(int x, int y) {
	if (dragging == true) {
		GLfloat mx = 0, my = 0;
		mPosToGL(x, y, mx, my);
		rects[dragIndex].drag(mx - lastX, my - lastY);	// 처음 클릭 시 마우스 위치와의 차이만큼 del값을 넘겨줌
		std::cout << "Drag!" << std::endl;
		glutPostRedisplay();
	}
}