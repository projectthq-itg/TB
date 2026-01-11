#include <GL/glut.h>

int w = 800, h = 600;
float camX = 0, camY = 0;
float zoom = 1.0f;
bool showGrid = true;

float px = 0, py = 0;
float psize = 0.6f;

void drawGrid() {
    if (!showGrid) return;
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = -50; i <= 50; i++) {
        if (i % 5 == 0) glColor3f(0.75, 0.75, 0.75);
        else glColor3f(0.9, 0.9, 0.9);
        glVertex2f(i, -50);
        glVertex2f(i, 50);
    }
    for (int j = -50; j <= 50; j++) {
        if (j % 5 == 0) glColor3f(0.75, 0.75, 0.75);
        else glColor3f(0.9, 0.9, 0.9);
        glVertex2f(-50, j);
        glVertex2f(50, j);
    }
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex2f(-50,0);
    glVertex2f(50,0);
    glColor3f(0,1,0);
    glVertex2f(0,-50);
    glVertex2f(0,50);
    glEnd();
}

void drawPlayer() {
    float s = psize/2;
    glColor3f(0.2,0.6,1);
    glBegin(GL_QUADS);
    glVertex2f(px-s, py-s);
    glVertex2f(px+s, py-s);
    glVertex2f(px+s, py+s);
    glVertex2f(px-s, py+s);
    glEnd();

    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px-s, py-s);
    glVertex2f(px+s, py-s);
    glVertex2f(px+s, py+s);
    glVertex2f(px-s, py+s);
    glEnd();
}

void display() {
    glClearColor(0.97,0.97,0.97,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-camX, -camY, 0);
    glScalef(zoom, zoom, 1);

    drawGrid();
    drawPlayer();

    glutSwapBuffers();
}

void reshape(int W, int H) {
    w = W; h = H;
    glViewport(0,0,W,H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float asp = (float)W/(float)H;
    float hh = 10.0f/zoom;
    float hw = hh*asp;
    glOrtho(-hw+camX, hw+camX, -hh+camY, hh+camY, -1,1);
    glMatrixMode(GL_MODELVIEW);
}

void updateProj() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float asp = (float)w/(float)h;
    float hh = 10.0f/zoom;
    float hw = hh*asp;
    glOrtho(-hw+camX, hw+camX, -hh+camY, hh+camY, -1,1);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key=='g') showGrid=!showGrid;
    if (key=='+') zoom+=0.1;
    if (key=='-') zoom-=0.1;
    if (zoom<0.2) zoom=0.2;
    if (key=='r') {px=py=0; zoom=1;}
    updateProj();
}

void special(int key, int x, int y) {
    if (key==GLUT_KEY_UP) py+=0.3;
    if (key==GLUT_KEY_DOWN) py-=0.3;
    if (key==GLUT_KEY_LEFT) px-=0.3;
    if (key==GLUT_KEY_RIGHT) px+=0.3;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(w,h);
    glutCreateWindow("Bidang 2D FreeGLUT");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}
