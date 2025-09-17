#include "tools.h"

void randColor(GLfloat& r, GLfloat& g, GLfloat& b) {
	r = rand() / static_cast<GLfloat>(RAND_MAX);
	g = rand() / static_cast<GLfloat>(RAND_MAX);
	b = rand() / static_cast<GLfloat>(RAND_MAX);
}

void randSquarePos(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2) {
	x1 = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
	y1 = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
	
	x2 = x1 + 0.05f;
	y2 = y1 - 0.05f;
}

void randRectPos(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2) {
	GLfloat randX[2], randY[2];
	for (int i = 0; i < 2; i++) {
		randX[i] = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
		randY[i] = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
	}
	if (randX[0] < randX[1]) {
		x1 = randX[0];
		x2 = randX[1];
	}
	else {
		x1 = randX[1];
		x2 = randX[0];
	}
	if (randY[0] > randY[1]) {
		y1 = randY[0];
		y2 = randY[1];
	}
	else {
		y1 = randY[1];
		y2 = randY[0];
	}
}

void mPosToGL(int mx, int my, GLfloat& xGL, GLfloat& yGL)
{
	xGL = (mx / 400.0f) - 1.0f;
	yGL = 1.0f - (my / 300.0f);
}

bool isMouseIn(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2, int mx, int my)
{
	GLfloat xGL, yGL;
	mPosToGL(mx, my, xGL, yGL);

	if (xGL > x1 && xGL < x2 && yGL < y1 && yGL > y2) return true;
	else return false;
}