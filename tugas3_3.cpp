#include <GL/glut.h>
#include <Math.h>

void myinit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 10.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
    glClear
}