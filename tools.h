#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct rtPos {
	GLfloat x1, y1, x2, y2;
};

void randColor(GLfloat& r, GLfloat& g, GLfloat& b);
void randRectPos(rtPos& pos);
void randSquarePos(rtPos& pos);

void mPosToGL(int mx, int my, GLfloat& xGL, GLfloat& yGL);
bool isMouseIn(rtPos& pos, int mx, int my);