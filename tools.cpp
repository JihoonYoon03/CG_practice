#include "tools.h"

void randColor(ColorRGB& color) {
	color.r = rand() / static_cast<GLfloat>(RAND_MAX);
	color.g = rand() / static_cast<GLfloat>(RAND_MAX);
	color.b = rand() / static_cast<GLfloat>(RAND_MAX);
}

void randSquarePos(rtPos& pos) {
	pos.x1 = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
	pos.y1 = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
	
	pos.x2 = pos.x1 + 0.05f;
	pos.y2 = pos.y1 - 0.05f;
}

void randRectPos(rtPos& pos) {
	GLfloat randX[2], randY[2];
	for (int i = 0; i < 2; i++) {
		randX[i] = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
		randY[i] = rand() / static_cast<GLfloat>(RAND_MAX) * 2.0f - 1.0f;
	}
	if (randX[0] < randX[1]) {
		pos.x1 = randX[0];
		pos.x2 = randX[1];
	}
	else {
		pos.x1 = randX[1];
		pos.x2 = randX[0];
	}
	if (randY[0] > randY[1]) {
		pos.y1 = randY[0];
		pos.y2 = randY[1];
	}
	else {
		pos.y1 = randY[1];
		pos.y2 = randY[0];
	}
}

void mPosToGL(int mx, int my, GLfloat& xGL, GLfloat& yGL)
{
	xGL = (mx / 400.0f) - 1.0f;
	yGL = 1.0f - (my / 300.0f);
}

bool isMouseIn(rtPos& pos, int mx, int my)
{
	GLfloat xGL, yGL;
	mPosToGL(mx, my, xGL, yGL);

	if (xGL > pos.x1 && xGL < pos.x2 && yGL < pos.y1 && yGL > pos.y2) return true;
	else return false;
}