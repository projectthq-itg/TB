// =======================================================
//     OpenGL Freeglut – Lighting + Camera + UFO 3D
// =======================================================

#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// ================= Global Variables ====================
const float PI = 3.14159265f;
const float MOVE_SPEED = 0.02f;
const float MOUSE_SENSITIVITY = 0.0025f;

float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 8.0f;
float cameraYaw = 0.0f, cameraPitch = -20.0f;

int windowWidth = 1200, windowHeight = 800;

bool lightingEnabled = true;
int lightMode = 1;
bool isDirectional = false;
bool isSpotlight = false;
float spotCutoff = 45.0f;
float spotExponent = 2.0f;
float linearAttenuation = 0.0f;
bool isShiny = true;

bool keys[256] = {false};
float objectRotationAngle = 0.0f;

// ================= Lighting =============================
void initLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matShiny[]    = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShiny);
}

void updateLightingLogic() {
    if (!lightingEnabled) {
        glDisable(GL_LIGHTING);
        return;
    }

    glEnable(GL_LIGHTING);

    GLfloat amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diff[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat off[] = {0.0f, 0.0f, 0.0f, 1.0f};

    if (lightMode == 1)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diff), glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    else if (lightMode == 2)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, off), glLightfv(GL_LIGHT0, GL_SPECULAR, off);
    else if (lightMode == 3)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diff), glLightfv(GL_LIGHT0, GL_SPECULAR, off);
    else if (lightMode == 4)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, off), glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

    GLfloat w = isDirectional ? 0.0f : 1.0f;
    GLfloat pos[] = {0.0f, 5.0f, 0.0f, w};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    if (isSpotlight) {
        GLfloat dir[] = {0.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
    } else {
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    }

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);

    GLfloat shinyMat[] = {isShiny ? 1.0f : 0.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinyMat);
}

// ================= Drawing ============================

void drawFloor() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.25f);
    glVertex3f(-10, 0, -10);
    glVertex3f(-10, 0,  10);
    glVertex3f( 10, 0,  10);
    glVertex3f( 10, 0, -10);
    glEnd();

    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.35f);

    glBegin(GL_LINES);
    for (float i = -10; i <= 10; i += 1.0f) {
        glVertex3f(i, 0.01f, -10); glVertex3f(i, 0.01f, 10);
        glVertex3f(-10, 0.01f, i); glVertex3f(10, 0.01f, i);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

// ================== MODIFIKASI: UFO 3D =====================
void drawUFO() {

    glPushMatrix();

    // Animasi naik turun
    float hover = sin(objectRotationAngle * 0.03f) * 0.2f;
    glTranslatef(0.0f, 1.0f + hover, 0.0f);

    // Rotasi UFO
    glRotatef(objectRotationAngle, 0.0f, 1.0f, 0.0f);

   
    // BAGIAN ATAS (KUBAH SPHERE)
   
    glPushMatrix();
    glColor3f(0.3f, 0.7f, 1.0f);   // biru muda
    glTranslatef(0, 0.35f, 0);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();

   
    // BAGIAN BAWAH (GEPEK)
   
    glPushMatrix();
    glColor3f(0.6f, 0.6f, 0.6f);   // abu metalik
    glScalef(1.0f, 0.3f, 1.0f);
    glutSolidSphere(0.6, 30, 30);
    glPopMatrix();

    glPopMatrix();
}

void drawLightSource() {
    if (!lightingEnabled) return;

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(0, 5, 0);

    glColor3f(1.0f, 1.0f, 0.8f);
    glutSolidSphere(0.2, 20, 20);

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// =================
void updateCamera() {
    float yawRad = cameraYaw * PI / 180.f;
    float pitchRad = cameraPitch * PI / 180.f;

    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = -sin(pitchRad);
    float forwardZ = -cos(yawRad) * cos(pitchRad);

    float rightX = sin(yawRad + PI/2);
    float rightZ = -cos(yawRad + PI/2);

    if (keys['w'] || keys['W'])
        cameraPosX += forwardX * MOVE_SPEED,
        cameraPosY += forwardY * MOVE_SPEED,
        cameraPosZ += forwardZ * MOVE_SPEED;

    if (keys['s'] || keys['S'])
        cameraPosX -= forwardX * MOVE_SPEED,
        cameraPosY -= forwardY * MOVE_SPEED,
        cameraPosZ -= forwardZ * MOVE_SPEED;

    if (keys['a'] || keys['A'])
        cameraPosX -= rightX * MOVE_SPEED,
        cameraPosZ -= rightZ * MOVE_SPEED;

    if (keys['d'] || keys['D'])
        cameraPosX += rightX * MOVE_SPEED,
        cameraPosZ += rightZ * MOVE_SPEED;

    if (keys[' ']) cameraPosY += MOVE_SPEED;
    if (keys[17])  cameraPosY -= MOVE_SPEED; // CTRL
}

// ================= Input ==============================
void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;

    if (key == 27) exit(0);
    if (key == 'l' || key == 'L') lightingEnabled = !lightingEnabled;

    if (key == '1') lightMode = 1;
    if (key == '2') lightMode = 2;
    if (key == '3') lightMode = 3;
    if (key == '4') lightMode = 4;

    if (key == 'y' || key == 'Y') isDirectional = !isDirectional;
    if (key == 'h' || key == 'H') isSpotlight = !isSpotlight;
    if (key == 'i' || key == 'I') isShiny     = !isShiny;

    if (key == 'j' || key == 'J') spotCutoff = std::max(0.f, spotCutoff - 2);
    if (key == 'k' || key == 'K') spotCutoff = std::min(90.f, spotCutoff + 2);

    if (key == 'n' || key == 'N') linearAttenuation += 0.05f;
    if (key == 'm' || key == 'M') linearAttenuation = std::max(0.f, linearAttenuation - 0.05f);
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void mouseMotion(int x, int y) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    if (x == centerX && y == centerY) return;

    cameraYaw   += (x - centerX) * MOUSE_SENSITIVITY * 30.f;
    cameraPitch += (y - centerY) * MOUSE_SENSITIVITY * 30.f;

    cameraPitch = std::max(-89.f, std::min(89.f, cameraPitch));

    glutWarpPointer(centerX, centerY);
}

// ================= GLUT Loop ==========================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth/windowHeight, 0.1, 150.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yawRad = cameraYaw * PI / 180.f;
    float pitchRad = cameraPitch * PI / 180.f;

    gluLookAt(
        cameraPosX,
        cameraPosY,
        cameraPosZ,
        cameraPosX + sin(yawRad) * cos(pitchRad),
        cameraPosY - sin(pitchRad),
        cameraPosZ - cos(yawRad) * cos(pitchRad),
        0, 1, 0
    );

    updateLightingLogic();
    drawFloor();
    drawUFO();       // MODIFIKASI
    drawLightSource();

    glutSwapBuffers();
}

void idle() {
    updateCamera();

    // animasi rotasi UFO
    objectRotationAngle += 1.0f;
    if (objectRotationAngle > 360.0f) objectRotationAngle = 0.0f;

    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

// ================= MAIN ===============================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(" ");

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
