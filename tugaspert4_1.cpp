#include <GL/glut.h>

float xPos = 0.0;
float yPos = 0.0;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.5);

    glLoadIdentity();
    glTranslated(xPos, yPos, 0);

    glRecti(0.0, 0.0, 10, 10);
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

void myinit(const char *title)
{
    glutCreateWindow(title);
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboard);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 50.0, 0.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char *argv[])

{
 glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400); 
    glutInitWindowPosition(100, 100);
    myinit("taqim");
    glutMainLoop();
    return 0;  
}