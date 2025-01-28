#include "Node.h"
#include "glut.h"

Node::Node()
{
	color = WHITE;
	parent = nullptr;
	g = MAX_G;
}

void Node::Show()
{
	switch (color)
	{
	case WHITE:
		glColor3d(1, 1, 1); // white
		break;
	case PACMAN:
		glColor3d(0.1, 0.6, 1); // blue
		break;
	case TARGET:
	case WHITE_TARGET:
	case GRAY_TARGET:
		glColor3d(1, 0, 0); // red
		break;
	case GRAY:
		glColor3d(0.1, 1, 0); // green
		break;
	case BLACK:
		glColor3d(0.9, 0.7, 0.5); // biege
		break;
	case PATH:
		glColor3d(1, 0, 1); // magenta
		break;
	}
	//fill with color
	glBegin(GL_POLYGON);
	glVertex2d(x - 1, y);
	glVertex2d(x, y + 1);
	glVertex2d(x + 1, y);
	glVertex2d(x, y - 1);
	glEnd();
	//frame
	glColor3d(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(x - 1, y);
	glVertex2d(x, y + 1);
	glVertex2d(x + 1, y);
	glVertex2d(x, y - 1);
	glEnd();
}
