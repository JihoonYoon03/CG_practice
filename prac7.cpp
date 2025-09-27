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
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

class Board;

class Rect {
	friend class Board;
	int index = -1;

protected:
	rtPos pos = { 0 };
	ColorRGB color = { 0.0f, 0.0f, 0.0f };
public:
	Rect(rtPos p, ColorRGB c) : pos(p), color(c) {}

	virtual void draw() {
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


class Puzzle : public Rect {
	friend class Board;

	GLfloat delX = 0, delY = 0;

public:
	Puzzle(rtPos p, ColorRGB c) : Rect(p, c) {}

	void draw() override {
		glColor3f(color.r, color.g, color.b);
		glRectf(pos.x1 + delX, pos.y1 + delY, pos.x2 + delX, pos.y2 + delY);
	}
	
	void drag(GLfloat mx, GLfloat my) {
		delX = mx;
		delY = my;
	}

	void stopDrag(bool isIn, rtPos newPos = { 0 }) {
		if (isIn) {
			pos.x1 = newPos.x1;
			pos.x2 = newPos.x2;
			pos.y1 = newPos.y1;
			pos.y2 = newPos.y2;
		}
		delX = 0;
		delY = 0;
	}

	rtPos returnPos() {
		return pos;
	}
};

class Board {
	std::vector<Rect> rects;
	std::vector<Puzzle> puzzles;

	int selectedPuzzleIdx = -1, correct = 0;

public:
	Board() {
		this->reroll();
	}

	void draw() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (auto& rect : rects) {
			rect.draw();
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0.2f, 0.2f, 0.2f);
		glRectf(-0.01f, 1.0f, 0.01f, -1.0f);

		for (auto& puzzle : puzzles) {
			puzzle.draw();
		}
	}

	void reroll() {
		rects.clear();
		puzzles.clear();
		for (int i = 0; i < 10; i++) {
			rects.push_back(dataSet[i]);
			rects[i].index = i;
		}
		

		// 퍼즐은 오른쪽에 약간 랜덤하게 배치
		for (int i = 0; i < 10; i++) {
			GLfloat offsetX = rand() / static_cast<GLfloat>(RAND_MAX) * 0.25f;
			GLfloat offsetY = (rand() / static_cast<GLfloat>(RAND_MAX) - 0.5f) * 0.5f;

			rtPos adjustedPos = dataSet[i].pos;
			adjustedPos.x1 += offsetX + 1.0f;
			adjustedPos.x2 += offsetX + 1.0f;
			adjustedPos.y2 += offsetY;
			adjustedPos.y1 += offsetY;

			puzzles.push_back({ adjustedPos, dataSet[i].color });
			puzzles[i].index = i;
		}

		selectedPuzzleIdx = -1;
		correct = 0;
	}

	bool clickPuzzle(int mx, int my) {
		bool correctClick = false;
		for (auto& puzzle : puzzles) {
			if (isMouseIn(puzzle.pos, mx, my)) {
				// 드래그 시작
				selectedPuzzleIdx = &puzzle - &puzzles[0];
				correctClick = true;
			}
		}

		return correctClick;
	}

	bool dragPuzzle(GLfloat mx, GLfloat my) {
		if (selectedPuzzleIdx != -1) {
			puzzles[selectedPuzzleIdx].drag(mx, my);
			return true;
		}
		return false;
	}

	void unclickPuzzle() {
		if (selectedPuzzleIdx != -1) {
			if (rects[selectedPuzzleIdx].pos.x1 + 0.05f > puzzles[selectedPuzzleIdx].pos.x1 + puzzles[selectedPuzzleIdx].delX &&
				rects[selectedPuzzleIdx].pos.x2 - 0.05f < puzzles[selectedPuzzleIdx].pos.x2 + puzzles[selectedPuzzleIdx].delX &&
				rects[selectedPuzzleIdx].pos.y1 - 0.05f < puzzles[selectedPuzzleIdx].pos.y1 + puzzles[selectedPuzzleIdx].delY &&
				rects[selectedPuzzleIdx].pos.y2 + 0.05f > puzzles[selectedPuzzleIdx].pos.y2 + puzzles[selectedPuzzleIdx].delY) {
				std::cout << "Correct!" << std::endl;
				puzzles[selectedPuzzleIdx].stopDrag(true, rects[selectedPuzzleIdx].pos);
				correct++;
			}
			else puzzles[selectedPuzzleIdx].stopDrag(false);
			selectedPuzzleIdx = -1;
		}

		if (correct == 10) {
			std::cout << "You Win!" << std::endl;
		}
	}
};

Board board;
bool dragging = false;
GLfloat mouseX = 0, mouseY = 0;

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
	glutKeyboardFunc(Keyboard); // 키보드 이벤트 콜백 함수 지정
	glutMouseFunc(Mouse); // 마우스 이벤트 콜백 함수 지정
	glutMotionFunc(Motion); // 마우스 움직임 콜백 함수 지정

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

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		board.reroll();
		dragging = false;
		glutPostRedisplay();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (!dragging) {
				mPosToGL(x, y, mouseX, mouseY);
				dragging = board.clickPuzzle(x, y);
			}
		}
		else if (state == GLUT_UP) {
			if (dragging) {
				// 퍼즐이 맞는 위치에 있는지 확인
				dragging = false;
				board.unclickPuzzle();
				glutPostRedisplay();
			}
		}
		break;
	}
}

GLvoid Motion(int x, int y) {
	if (dragging) {
		GLfloat xGL, yGL;
		mPosToGL(x, y, xGL, yGL);
		board.dragPuzzle(xGL - mouseX, yGL - mouseY);
		glutPostRedisplay();
	}
}