#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

// void glRectf(Glfloat x1, Glfloat y1, Glfloat x2, Glfloat y2);
// void glColor3f (Glfloat r, Glfloat g, Glfloat b);

struct Point {
	GLfloat x;
	GLfloat y;
};;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void drawRect(bool isSmall, Point p1, Point p2);

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
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.0f, 0.4f, 0.4f, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.

	drawRect(false, {-1.0f, 1.0f}, {0.0f, 0.0f});

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void drawRect(bool isSmall, Point p1, Point p2) {
	if (!isSmall) {
		glRectf(p1.x, p1.y, p2.x, p2.y);
	}
	else {
		glRectf(p1.x - 0.1f, p1.y - 0.1f, p2.x + 0.1f, p2.y + 0.1f);
	}
}