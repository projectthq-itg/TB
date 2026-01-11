#include <GL/glut.h>
void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3ub(30, 32, 30);
glLineWidth(1.5);
//glBegin(GL_LINES);
//glBegin(GL_LINE_STRIP);
glBegin(GL_LINE_LOOP);
for (float x = -1.5f; x <= 1.5f; x += 0.01f)
{
float y = 2.5f * (x * x * x * x - 1.5 * x * x);
glVertex2f(x, y);
}
glEnd();
glColor3ub(60, 61, 55);
glBegin(GL_LINES);
glVertex2f(-2.0f, 0.0f);
glVertex2f(2.0f, 0.0f);
glVertex2f(0.0f, -2.0f);
glVertex2f(0.0f, 2.0f);
glEnd();
glFlush();
}
int main(int argc, char **argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(500, 500);
glutInitWindowPosition(100, 100);
glutCreateWindow("");
glutDisplayFunc(display);
glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
gluOrtho2D(-2.0f, 2.0f, -2.0f, 2.0f);
glutMainLoop();
return 0;
}
