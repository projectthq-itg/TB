#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>

using namespace std;

GLuint texture_Earth_ID;
GLuint texture_Moon_ID;
GLuint textureID = 0;

float tick = 0.0;

void createObject() {
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE);
    gluQuadricNormals(object, GLU_SMOOTH);
    gluSphere(object, 3, 100, 100);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(-4.0, 0.0, 0.0);
    glRotatef(tick += 0.2, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture_Earth_ID);
    createObject();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.0, 0.0, 0.0);
    glRotatef(tick, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture_Moon_ID);
    createObject();
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

int loadTexture(const char* path) {
    glGenTextures(1, &textureID);

    void* imgData;
    int imgWidth, imgHeight;

    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);

    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        imgWidth,
        imgHeight,
        0,
        GL_BGR,
        GL_UNSIGNED_BYTE,
        imgData
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    FreeImage_Unload(bitmap2);

    textureID++;
    return textureID - 1;
}

void initProjection() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1800.0 / 900.0, 1.0, 100.0);
    gluLookAt(
        10.0, 10.0, 20.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    glMatrixMode(GL_MODELVIEW);

    texture_Earth_ID = loadTexture("imej/bumi.png");
    texture_Moon_ID  = loadTexture("imej/bulan.jpg");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1800, 900);
    glutCreateWindow("2");
    glewInit();
    initProjection();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
