#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

// void glRectf(Glfloat x1, Glfloat y1, Glfloat x2, Glfloat y2);
// void glColor3f (Glfloat r, Glfloat g, Glfloat b);

struct Rect {
	GLfloat x1, y1, x2, y2;
	GLfloat r, g, b;
};;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
bool isMouseIn(Rect& target, int x, int y);
void randColor(GLfloat& r, GLfloat& g, GLfloat& b);

GLfloat bgColor[3] = { 0.0f, 0.4f, 0.4f };

Rect rects[4] =
{
	{-0.9f, 0.9f, -0.1f, 0.1f, 0.4f, 0.4f, 0.4f},
	{ 0.1f, 0.9f, 0.9f, 0.1f, 0.1f, 0.2f, 0.3f },
	{ -0.9f, -0.1f, -0.1f, -0.9f, 0.7f, 0.2f, 0.3f },
	{ 0.1f, -0.1f, 0.9f, -0.9f, 0.4f, 0.8f, 0.2f },
};

void drawRect(Rect rect) {
	glColor3f(rect.r, rect.g, rect.b);
	glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
}

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
	glutMouseFunc(Mouse); // 마우스 이벤트 콜백 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	for (int i = 0; i < 4; i++)
		drawRect(rects[i]);

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
			if (isMouseIn(rects[0], x, y));
			else if (isMouseIn(rects[1], x, y));
			else if (isMouseIn(rects[2], x, y));
			else if (isMouseIn(rects[3], x, y));
			else randColor(bgColor[0], bgColor[1], bgColor[2]);

			std::cout << "Left Button Down (" << x << ", " << y << ")\n";
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			std::cout << "Right Button Down (" << x << ", " << y << ")\n";
		}
		break;
	}
}

bool isMouseIn(Rect& target, int x, int y)
{
	GLfloat xGL = (x / 400.0f) - 1.0f;
	GLfloat yGL = 1.0f - (y / 300.0f);

	if (xGL > target.x1 && xGL < target.x2 && yGL < target.y1 && yGL > target.y2) {
		randColor(target.r, target.g, target.b);
		return true;
	}
	else return false;
}

void randColor(GLfloat& r, GLfloat& g, GLfloat& b) {
	r = rand() % 100 / 100.0f;
	g = rand() % 100 / 100.0f;
	b = rand() % 100 / 100.0f;
}