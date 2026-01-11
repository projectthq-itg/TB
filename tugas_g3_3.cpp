#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
void myinit()
{
 glClearColor(1.0, 1.0, 1.0, 1.0);
 glColor3f(1.0, 0.0, 0.0);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(-1.0, 10.0, -2.0, 2.0);
}
void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glColor3ub(30, 32, 30);
 glLineWidth(1.5);
 //glBegin(GL_LINES);
 //glBegin(GL_LINE_STRIP);
 glBegin(GL_LINE_LOOP);
 for (float x = 0.0; x <= 6.28; x += 0.1)
 {
 glVertex2f(x, sin(x));
 }
 glEnd();
 glBegin(GL_LINES);
 glColor3ub(60, 61, 55);
 glVertex2f(-10.0f, 0.0f);
 glVertex2f(10.0f, 0.0f);
 glVertex2f(0.0f, -10.0f);
 glVertex2f(0.0f, 10.0f);
 glEnd();
 glFlush();
}
void kunci(unsigned char key, int x, int y)
{
 switch (key)
 {
 case 27: // ESC
 case 'q':
 exit(0);
 break;
 }
 glutPostRedisplay();
}
int main(int argc, char *argv[])
{
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
 glutInitWindowSize(400, 400);
 glutInitWindowPosition(100, 100);
 glutCreateWindow("");
 glutDisplayFunc(display);
 glutKeyboardFunc(kunci);
 myinit();
 glutMainLoop();
 return 0;
}
