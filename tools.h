#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

void randColor(GLfloat& r, GLfloat& g, GLfloat& b);
void randRectPos(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2);

void mPosToGL(int mx, int my, GLfloat& xGL, GLfloat& yGL);
bool isMouseIn(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2, int mx, int my);