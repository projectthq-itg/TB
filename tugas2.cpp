#include <GL/glut.h>

void titik ();
int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Latihan 1 - Membuatobjek sebuaaah Titik");
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glOrtho(2.0,2.0,2.0,2.0,-2.0,1.0);
    glutDisplayFunc(titik);
    glutMainLoop();
}

void titik ()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10.5);  
    glBegin(GL_POINTS);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(0,0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0,0.15);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0,0.30);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0,0.45);
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0,0.60);
    glEnd();
    glFinish();
}

