#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>

#include "tools.h"

class Rect;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
void TimerFunction(int value);
void randomParticles(Rect& rect);

class Rect {
protected:
	rtPos pos = { 0 };
	ColorRGB color = { 0.0f, 0.0f, 0.0f };
public:
	Rect() {	// 기본 랜덤 사각형 생성
		randSquarePos(pos, (rand() / static_cast<GLfloat>(RAND_MAX) + 0.4f) * 0.2);
		randColor(color);
	}

	bool clicked(int mx, int my) {
		return isMouseIn(pos, mx, my);
	}

	void draw() {
		glColor3f(color.r, color.g, color.b);
		glRectf(pos.x1, pos.y1, pos.x2, pos.y2);
	}

	rtPos returnPos() {
		return pos;
	}

	ColorRGB returnColor() {
		return color;
	}
};

class Particle {
	rtPos pos = { 0 };
	ColorRGB color;

	GLfloat scaleX, scaleY, dx, dy, speed = 0.01f;
public:
	// 위치, 크기, 방향벡터, 색깔 입력
	Particle(GLfloat Xpos, GLfloat Ypos, GLfloat scaleX, GLfloat scaleY, GLfloat dx, GLfloat dy, ColorRGB colorIn) : scaleX(scaleX), scaleY(scaleY), dx(dx), dy(dy), color(colorIn) {
		pos.x1 = Xpos - scaleX / 2.0f;
		pos.y1 = Ypos + scaleY / 2.0f;
		pos.x2 = Xpos + scaleX / 2.0f;
		pos.y2 = Ypos - scaleY / 2.0f;
		GLfloat len = sqrt(dx * dx + dy * dy);
		this->dx = dx / len;
		this->dy = dy / len;
	}

	bool move() {
		pos.x1 += dx * speed;
		pos.x2 += dx * speed;
		pos.y1 += dy * speed;
		pos.y2 += dy * speed;

		pos.x1 += scaleX * 0.01f;
		pos.x2 -= scaleY * 0.01f;
		pos.y1 -= scaleX * 0.01f;
		pos.y2 += scaleY * 0.01f;

		if (pos.x2 - pos.x1 <= 0 || pos.y1 - pos.y2 <= 0) {
			// 삭제 신호
			return true;
		}

		return false;
	}

	void draw() {
		glColor3f(color.r, color.g, color.b);
		glRectf(pos.x1, pos.y1, pos.x2, pos.y2);
	}
};


std::vector<Rect> rects;
std::vector<Particle> particles;

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

	srand((unsigned int)time(NULL));

	int cap = 5 + rand() % 6;

	for (int i = 0; i < cap; i++)
		rects.push_back(Rect());


	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse); // 마우스 이벤트 콜백 함수 지정
	glutTimerFunc(10, TimerFunction, 1);

	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& rect : rects) {
		rect.draw();
	}

	for (auto & part : particles) {
		part.draw();
	}

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
			for (auto rect = rects.begin(); rect != rects.end(); ) {
				if (rect->clicked(x, y)) {
					std::cout << "Left Click!" << std::endl;
					randomParticles(*rect);
					rects.erase(rect);
					break;
				}
				else
					rect++;
			}
		}
		break;
	}
}

void TimerFunction(int value) {
	for (auto part = particles.begin(); part != particles.end(); ) {
		if (part->move()) {
			std::cout << "particle erase" << std::endl;
			part = particles.erase(part);
		}
		else {
			std::cout << "particle draw" << std::endl;
			part->draw();
			part++;
		}
	}

	glutPostRedisplay();
	if (value) {
		std::cout << "timer" << std::endl;
		glutTimerFunc(10, TimerFunction, 1);
	}
}


void randomParticles(Rect& rect) {
	rtPos pos = rect.returnPos();
	ColorRGB color = rect.returnColor();
	GLfloat centerX = (pos.x1 + pos.x2) / 2.0f;
	GLfloat centerY = (pos.y1 + pos.y2) / 2.0f;
	GLfloat scaleX = (pos.x2 - pos.x1) / 2.0f;
	GLfloat scaleY = (pos.y1 - pos.y2) / 2.0f;

	std::cout << "pos: " << centerX << ", " << centerY << std::endl;

	switch (rand() % 4) {
	case 0:
		// 상하좌우 4방향

		for (int i = 0; i < 4; i++)
			particles.push_back(Particle(centerX, centerY, scaleX, scaleY, (i == 0) - (i == 1), (i == 2) - (i == 3), color));	// 1, 0 / -1, 0 / 0, 1 / 0, -1

		break;
	case 1:
		// 대각선 4방향

		for (int i = 0; i < 4; i++)
			particles.push_back(Particle(centerX, centerY, scaleX, scaleY, (i < 2 ? 1 : -1), (i % 3 == 0 ? 1 : -1), color)); // 1, 1 / 1, -1 / -1, -1 / -1, 1

		break;
	case 2:
	{
		// 다같이 한뱡향

		GLfloat dirX = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
		GLfloat dirY = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
		for (int i = 0; i < 4; i++) {
			GLfloat offset = (rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f) * 0.05f;
			particles.push_back(Particle(centerX + offset, centerY + offset, scaleX, scaleY, dirX, dirY, color));
		}

		break;
	}
	case 3:
		// 8방향

		for (int i = 0; i < 4; i++)
			particles.push_back(Particle(centerX, centerY, scaleX, scaleY, (i == 0) - (i == 1), (i == 2) - (i == 3), color)); // 1, 0 / -1, 0 / 0, 1 / 0, -1

		for (int i = 0; i < 4; i++)
			particles.push_back(Particle(centerX, centerY, scaleX, scaleY, (i < 2 ? 1 : -1), (i % 3 == 0 ? 1 : -1), color)); // 1, 1 / 1, -1 / -1, -1 / -1, 1

		break;
	}
}