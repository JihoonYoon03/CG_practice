#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
bool timerToggle = false;

GLclampf red = 1.0f, green = 1.0f, blue = 1.0f; // 배경색 변수

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(time(NULL));

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
	glutKeyboardFunc(Keyboard); // 키보드 입력 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(red, green, blue, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		red = 0.0f;
		green = 1.0f;
		blue = 1.0f;
		break;
	case 'm':
		red = 1.0f;
		green = 0.0f;
		blue = 1.0f;
		break;
	case 'y':
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
		break;
	case 'a':
		red = rand() % 100 / 100.0f;
		green = rand() % 100 / 100.0f;
		blue = rand() % 100 / 100.0f;
		std::cout << red << green << blue << std::endl;
		break;
	case 'w':
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
		break;
	case 'k':
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
		break;
	case 't':
		timerToggle = true;
		glutTimerFunc(100, TimerFunction, 1);
		break;
	case 's':
		timerToggle = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		return;
	}
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	red = rand() % 100 / 100.0f;
	green = rand() % 100 / 100.0f;
	blue = rand() % 100 / 100.0f;
	glutPostRedisplay();
	if (timerToggle)
		glutTimerFunc(100, TimerFunction, 1);
	else
		return;
}