#include <GL/glut.h>

void display() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(-1, 0);
        glVertex2f(-1, 1);
        glVertex2f(1, 1);
        glVertex2f(1, 0);
    glEnd();

    glColor3f(0, 0, 0);

   
    glRasterPos2f(-0.4f, -0.1f);

    const char *text = "NKRI HARGA MATI";

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("nama dan nim saya hilangkan pak/bu, demi keamanan jejak digital saya");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}