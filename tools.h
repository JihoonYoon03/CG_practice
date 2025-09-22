#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct rtPos {
	GLfloat x1, y1, x2, y2;
};

struct ColorRGB {
	GLfloat r, g, b;
};

void randColor(ColorRGB& color);
void randRectPos(rtPos& pos);
void randSquarePos(rtPos& pos, GLfloat offset);

void mPosToGL(int mx, int my, GLfloat& xGL, GLfloat& yGL);
bool isMouseIn(rtPos& pos, int mx, int my);