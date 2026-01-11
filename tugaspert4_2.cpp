#include <GL/glut.h>

float xPos = 1.0;
float yPos = 1.0;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.5);

    glLoadIdentity();
    glScalef(xPos, yPos, 0);
    glRecti(0, 0, 10, 10);
    glFlush();

}


void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'd':
            xPos += 1.0;
            break;
        case 'a':
            xPos -= 1.0;
            break;
        case 'w':
            yPos += 1.0;
            break;
        case 's':
            yPos -= 1.0;
            break;
    }
    glutPostRedisplay();
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-50.0, 50.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glLoadIdentity();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2");
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboard);
    
    init();
    glutMainLoop();


    return 0;
}