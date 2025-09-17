#include "tools.h"

void randColor(GLfloat& r, GLfloat& g, GLfloat& b) {
	r = rand() / static_cast<GLfloat>(RAND_MAX);
	g = rand() / static_cast<GLfloat>(RAND_MAX);
	b = rand() / static_cast<GLfloat>(RAND_MAX);
}