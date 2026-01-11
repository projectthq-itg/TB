#include <GL/glut.h>

void garis();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-500.0, 300.0, -400.0, 400.0);

    glutDisplayFunc(garis);
    glutMainLoop();
}

void garis()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);


    glColor3f(0.2, 1.0, 1.0);
    glVertex2d(-400, 200);
    glVertex2d(400, 200);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2d(-400, 0);
    glVertex2d(400, 0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2d(-400, -200);
    glVertex2d(400, -200);

    glEnd();
    glFlush();
}
