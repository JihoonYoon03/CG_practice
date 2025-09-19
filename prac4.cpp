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
void TimerFunction(int value);

class Rect {
	GLfloat rO, gO, bO;
	GLfloat OriginX, OriginY;	// 사각형 시작 위치

	// 위는 고정값, 아래는 변하는 값
	rtPos pos = { 0 };
	GLfloat delX = 0, delY = 0, r, g, b, width = 0.1f, height = 0.1f;
public:

	Rect(GLfloat mx, GLfloat my) : OriginX(mx), OriginY(my) {
		pos.x1 = OriginX - width / 2; pos.y1 = OriginY + height / 2;
		pos.x2 = OriginX + width / 2; pos.y2 = OriginY - height / 2;
		randColor(rO, gO, bO);
		r = rO, g = gO, b = bO;
	}

	void draw() {
		glColor3f(rO, gO, bO);
		glRectf(pos.x1, pos.y1, pos.x2, pos.y2);
	}

	void resetPos() {
		pos.x1 = OriginX - width / 2; pos.y1 = OriginY + height / 2;
		pos.x2 = OriginX + width / 2; pos.y2 = OriginY - height / 2;
	}

	void setDeltaDiag() {
		delX = rand() % 2 == 0 ? 0.01f : -0.01f;
		delY = rand() % 2 == 0 ? 0.01f : -0.01f;
	}

	void setDeltaZigzag() {
		delX = rand() % 2 == 0 ? 0.01f : -0.01f;
		delY = -1;
	}

	void moveDiag() {
		pos.x1 += delX; pos.x2 += delX;
		if (pos.x1 < -1.0f) {
			pos.x1 = -1.0f; pos.x2 = pos.x1 + width;
			delX = -delX;
		}
		else if (pos.x2 > 1.0f) {
			pos.x2 = 1.0f; pos.x1 = pos.x2 - width;
			delX = -delX;
		}

		pos.y1 += delY; pos.y2 += delY;
		if (pos.y2 < -1.0f) {
			pos.y2 = -1.0f; pos.y1 = pos.y2 + height;
			delY = -delY;
		}
		else if (pos.y1 > 1.0f) {
			pos.y1 = 1.0f; pos.y2 = pos.y1 - height;
			delY = -delY;
		}
	}

	void moveZigzag() {
		bool atEdge = false;
		pos.x1 += delX; pos.x2 += delX;
		if (pos.x1 < -1.0f) {
			pos.x1 = -1.0f; pos.x2 = pos.x1 + width;
			delX = -delX;
			atEdge = true;
		}
		else if (pos.x2 > 1.0f) {
			pos.x2 = 1.0f; pos.x1 = pos.x2 - width;
			delX = -delX;
			atEdge = true;
		}
		if (atEdge) {
			pos.y1 += height * delY; pos.y2 += height * delY;
			if (pos.y1 > 1.0f) {
				pos.y1 = 1.0f; pos.y2 = pos.y1 - height;
				delY = -delY;
			}
			else if (pos.y2 < -1.0f) {
				pos.y2 = -1.0f; pos.y1 = pos.y2 + height;
				delY = -delY;
			}
		}
	}

	void rollWH() {
		width = rand() / static_cast<float>(RAND_MAX) * 0.3f + 0.05f;
		height = rand() / static_cast<float>(RAND_MAX) * 0.3f + 0.05f;
	}

	void resetWH() {
		width = 0.1f; height = 0.1f;
	}

	void rollColor() {
		rO = rand() / static_cast<float>(RAND_MAX);
		gO = rand() / static_cast<float>(RAND_MAX);
		bO = rand() / static_cast<float>(RAND_MAX);
	}

	void resetColor() {
		rO = r, gO = g, bO = b;
	}

	rtPos returnPos() {
		return pos;
	}
};;

std::vector<Rect> rects;
GLfloat lastX = 0, lastY = 0;
bool playAnim = false, rollSize = false, rollColor = false, animDiag = false, animZigzag = false, timerOn = false;
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
	case '1':
		if (!animDiag) {
			playAnim = true;
			animDiag = true;
			animZigzag = false;
			for (auto& rect : rects) {
				rect.setDeltaDiag();
			}
			if (!timerOn) {
				timerOn = true;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			playAnim = false;
			animDiag = false;
		}
		break;
	case '2':
		if (!animZigzag) {
			playAnim = true;
			animZigzag = true;
			animDiag = false;
			for (auto& rect : rects) {
				rect.setDeltaZigzag();
			}
			if (!timerOn) {
				timerOn = true;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			playAnim = false;
			animZigzag = false;
		}
		break;
	case '3':
		if (!rollSize) {
			rollSize = true;
			for (auto& rect : rects) {
				rect.rollWH();
			}
		}
		else {
			rollSize = false;
			for(auto& rect : rects) {
				rect.resetWH();
			}
		}
		glutPostRedisplay();
		break;
	case '4':
		if (!rollColor) {
			rollColor = true;
			for (auto& rect : rects) {
				rect.rollColor();
			}
		}
		else {
			rollColor = false;
			for (auto& rect : rects) {
				rect.resetColor();
			}
		}
		glutPostRedisplay();
		break;
	case 'm':
		for (auto& rect : rects) {
			rect.resetPos();
		}
		glutPostRedisplay();
		break;
	case 'r':
		rects.clear();
		glutPostRedisplay();
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

void TimerFunction(int value) {
	if (playAnim) {
		if (animDiag)
			for (auto& rect : rects) {
				rect.moveDiag();
			}
		else if (animZigzag)
			for (auto& rect : rects) {
				rect.moveZigzag();
			}
		glutTimerFunc(10, TimerFunction, 1);
	}
	else {
		timerOn = false;
	}
	glutPostRedisplay();
}