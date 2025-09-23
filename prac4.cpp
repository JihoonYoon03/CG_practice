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
	ColorRGB color;
	GLfloat OriginX, OriginY;	// 사각형 시작 위치
	GLfloat defWidth = 0.1f, defHeight = 0.1f;

	// 위는 고정값, 아래는 변하는 값
	rtPos pos = { 0 };
	GLfloat pinPosX = 0, pinPosY = 0;
	GLfloat delX = 0, delY = 0, r, g, b, width = 0.05f, height = 0.05f, angle = 45;
	bool isRotate = false;
public:

	Rect(GLfloat mx, GLfloat my) : OriginX(mx), OriginY(my) {
		std::cout << "OriginX: " << OriginX << ", OriginY: " << OriginY << std::endl;
		pos.x1 = OriginX - defWidth / 2; pos.y1 = OriginY + defHeight / 2;
		pos.x2 = OriginX + defWidth / 2; pos.y2 = OriginY - defHeight / 2;
		randColor(color);
		r = color.r, g = color.g, b = color.b;
	}

	void draw() {
		glColor3f(color.r, color.g, color.b);
		glRectf(pos.x1 - width, pos.y1 + height, pos.x2 + width, pos.y2 - height);
	}

	void resetPos() {
		pos.x1 = OriginX - defWidth / 2; pos.y1 = OriginY + defHeight / 2;
		pos.x2 = OriginX + defWidth / 2; pos.y2 = OriginY - defHeight / 2;
	}

	void setDeltaDiag() {
		delX = rand() % 2 == 0 ? 0.01f : -0.01f;
		delY = rand() % 2 == 0 ? 0.01f : -0.01f;
	}

	void setDeltaZigzag() {
		delX = rand() % 2 == 0 ? 0.01f : -0.01f;
		delY = -1;
	}

	void moveDiag(bool Zigzag) {
		bool atEdge = false;
		GLfloat edgeX = 1.0f - width, edgeY = 1.0f - height;

		pos.x1 += delX; pos.x2 += delX;
		if (pos.x1 < -edgeX) {
			pos.x1 = -edgeX; pos.x2 = pos.x1 + defWidth;
			delX = -delX;
			atEdge = true;
		}
		else if (pos.x2 > edgeX) {
			pos.x2 = edgeX; pos.x1 = pos.x2 - defWidth;
			delX = -delX;
			atEdge = true;
		}

		// 지그재그
		if (atEdge && Zigzag) {
			pos.y1 += height * delY; pos.y2 += height * delY;
			if (pos.y1 > edgeY) {
				pos.y1 = edgeY; pos.y2 = pos.y1 - defHeight;
				delY = -delY;
			}
			else if (pos.y2 < -edgeY) {
				pos.y2 = -edgeY; pos.y1 = pos.y2 + defHeight;
				delY = -delY;
			}
		}

		// 대각선
		else if (!Zigzag) {
			pos.y1 += delY; pos.y2 += delY;
			if (pos.y1 > edgeY) {
				pos.y1 = edgeY; pos.y2 = pos.y1 - defHeight;
				delY = -delY;
			}
			else if (pos.y2 < -edgeY) {
				pos.y2 = -edgeY; pos.y1 = pos.y2 + defHeight;
				delY = -delY;
			}
		}
	}

	void rollWH() {
		width = rand() / static_cast<float>(RAND_MAX) * 0.2f + 0.05f;
		height = rand() / static_cast<float>(RAND_MAX) * 0.2f + 0.05f;
	}

	void resetWH() {
		width = 0.05f; height = 0.05f;
	}

	void rollColor() {
		randColor(color);
	}

	void resetColor() {
		color.r = r, color.g = g, color.b = b;
	}

	bool checkRotate() {
		return isRotate;
	}

	void startRotate() {
		isRotate = true;
		pinPosX = pos.x1 + defWidth / 2;
		pinPosY = pos.y1 - defHeight / 2;
	}

	void endRotate() {
		if (isRotate) {
			isRotate = false;
			angle = 45;
			pos.x1 = pinPosX - defWidth / 2;
			pos.y1 = pinPosY + defHeight / 2;
			pos.x2 = pinPosX + defWidth / 2;
			pos.y2 = pinPosY - defHeight / 2;
		}
	}

	void rotating() {
		if (isRotate) {
			std::cout << "pinPosX: " << pinPosX << ", pinPosY: " << pinPosY << std::endl;
			pos.x1 = pinPosX - defWidth / 2 * cos(angle / 180 * 3.14);
			pos.y1 = pinPosY + defHeight / 2 * sin(angle / 180 * 3.14);
			pos.x2 = pinPosX + defWidth / 2  * cos(angle / 180 * 3.14);
			pos.y2 = pinPosY - defHeight / 2 * sin(angle / 180 * 3.14);
			if (angle >= 405) {
				angle = 45;
				isRotate = false;
			}
			else {
				angle += 5;
			}
		}
	}

	rtPos returnPos() {
		return pos;
	}
};;

std::vector<Rect> rects;
GLfloat lastX = 0, lastY = 0;
bool playAnim = false, rollSize = false, rollColor = false, animDiag = false, isAnimZigzag = false;
int animHeadIndex = -1, animEndIndex = -1, tick = 0;

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
			isAnimZigzag = false;
			for (auto& rect : rects) {
				rect.setDeltaDiag();
			}
			if (!playAnim) {
				playAnim = true;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			playAnim = false;
		}
		break;
	case '2':
		if (!isAnimZigzag) {
			isAnimZigzag = true;
			for (auto& rect : rects) {
				rect.setDeltaZigzag();
			}
			if (!playAnim) {
				playAnim = true;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			playAnim = false;
			isAnimZigzag = false;
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
			for (auto& rect : rects) {
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
	case '5':
		if (animHeadIndex == -1) {
			// rotate는 다른 애니메이션 플래그에 영향 X, 다만 해당 애니메이션이 끝나거나 도중에 중단될 때 까진 다른 애니메이션 실행 X
			animHeadIndex = rand() % 5;
			animEndIndex = 4;
			if (animEndIndex == animHeadIndex) animEndIndex = 3;

			rects[animHeadIndex].startRotate();

			if (!playAnim) {
				playAnim = true;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			animHeadIndex = -1, animEndIndex = -1;
			for (auto& rect : rects) {
				rect.endRotate();
			}
		}
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
		isAnimZigzag = false;
		animHeadIndex = -1, animEndIndex = -1;
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
		if (animHeadIndex != -1) {
			tick = (tick + 1) % 10;
			rects[animHeadIndex].rotating();	// head 먼저 애니메이션 시작

			for (auto& rect : rects) {
				if (&rect != &rects[animHeadIndex]) {
					rect.rotating();	// isRotate 플래그가 켜진 사각형만 애니메이션
				}
			}

			if (animEndIndex != -1) {
				if (tick == 0) {
					for (auto& rect : rects) {
						if (rect.checkRotate() == false && &rect != &rects[animHeadIndex]) {
							// 마지막 사각형이면 animEndIndex 초기화
							if (&rect - &rects[0] == animEndIndex) {
								animEndIndex = -1;
							}

							rect.startRotate();
							break;
						}
					}
				}
			}
			else {
				bool checkEnd = true;
				for (auto& rect : rects) {
					if (rect.checkRotate()) {
						checkEnd = false;
						break;
					}
				}
				if (checkEnd) {
					animHeadIndex = -1;
				}
			}
		}

		for (auto& rect : rects) {
			if (rect.checkRotate() == false) {
				rect.moveDiag(isAnimZigzag);
			}
		}

		glutTimerFunc(10, TimerFunction, 1);
	}
	glutPostRedisplay();
}
