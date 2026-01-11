#include <GL/glut.h>
#include <math.h>

float objects = 10.0f;
const float PI = 3.1416f;
float radius = 40.0f;
float rotationAngle = 0.0f;
float rotation = 0.0f;
float aspect;

GLfloat light_position[4] = {0.0f, 30.0f, 50.0f, 1.0f};

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 200.0);
    gluLookAt(0.0, 20.0, 70.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mykeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;

    case 'w':
        light_position[2] -= 1.0;
        break;
    case 's':
        light_position[2] += 1.0;
        break;
    case 'a':
        light_position[0] -= 1.0;
        break;
    case 'd':
        light_position[0] += 1.0;
        break;
    case 'r':
        light_position[0] = 0.0f;
        light_position[2] = 50.0f;
        break;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(light_position[0], light_position[1], light_position[2]);
    glEnd();

    glPointSize(6.0);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0.0, 1.0);
    glVertex3f(light_position[0], light_position[1], light_position[2]);
    glEnd();
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glColor3ub(255, 255, 0);
    glRotatef(rotation += 0.1f, 0.0f, 1.0f, 0.0f);
    glRotated(90, 1.0, 0.0, 0.0);
    glutSolidSphere(10.0, 30, 30);
    glPopMatrix();

    for (int i = 0; i < (int)objects; ++i)
    {
        float angle = (i * (360.0f / objects) * (PI / 180.0f)) + rotationAngle;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        glColor3ub(100, 150, 80);
        glutSolidTetrahedron();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(x * 1.2f, 1.0f, z * 1.2f);
        glColor3ub(150, 200, 255);
        glutSolidDodecahedron();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(x * 1.3f, 1.0f, z * 1.2f);
        glColor3ub(255, 255, 100);
        glutSolidIcosahedron();
        glPopMatrix();
    }

    glPushMatrix();
    glRotatef(5.0f, 1.0f, 0.0f, 0.5f);
    for (int i = 0; i < (int)objects; ++i)
    {
        float angle = (i * (360.0f / objects) * (PI / 180.0f)) - rotationAngle;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        glColor3ub(255, 255, 000);
        glutSolidOctahedron();
        glPopMatrix();
    }
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();

    rotationAngle += 0.0005f;
}

void inisialisasi()
{
    GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_ambient[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(mykeyboard);

    
    inisialisasi();

    glutMainLoop();
    return 0;
}
