#include <GL/glut.h>

void garis ();
int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Latihan 3 - Membuatobjek sebuah Garis");
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-500.0,300.0,-400.0,400.0);
    glutIdleFunc(garis);
    glutDisplayFunc(garis);
    glutMainLoop();
}

void garis ()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    glVertex2d(0,0);
    glColor3f(0.0,0.0,0.0);
    glVertex2d(300,400);
    glEnd();
    glFinish();
}