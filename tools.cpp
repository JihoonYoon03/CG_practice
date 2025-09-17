#include "tools.h"

void randColor(GLfloat& r, GLfloat& g, GLfloat& b) {
	r = rand() / static_cast<GLfloat>(RAND_MAX);
	g = rand() / static_cast<GLfloat>(RAND_MAX);
	b = rand() / static_cast<GLfloat>(RAND_MAX);
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