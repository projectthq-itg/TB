#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// ---------------------------------- global variabel
const float PI = 3.14159265f;
const float MOVE_SPEED = 0.002f;
const float MOUSE_SENSITIVITY = 0.002f;

float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 8.0f;
float cameraYaw = 0.0f, cameraPitch = -20.0f;

int lastMouseX = 0, lastMouseY = 0;
bool mouseActive = false;
bool keys[256] = {false};

int windowWidth = 1200, windowHeight = 800;

bool lightingEnabled = true;
int lightMode = 1;
bool isDirectional = false;
bool isSpotlight = false;

float spotCutoff = 45.0f;
float spotExponent = 2.0f;
float linearAttenuation = 0.0f;
bool isShiny = true;

// ---------------------------------- fungsi pencahayaan
void initLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void updateLightingLogic() {
    if (!lightingEnabled) {
        glDisable(GL_LIGHTING);
        return;
    }

    glEnable(GL_LIGHTING);

    GLfloat val_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat val_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat val_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat val_black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    if (lightMode == 1) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_specular);
    } 
    else if (lightMode == 2) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_black);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_black);
    }
    else if (lightMode == 3) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_black);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_black);
    }
    else if (lightMode == 4) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_black);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_black);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_specular);
    }

    GLfloat w_component = isDirectional ? 0.0f : 1.0f;
    GLfloat light_pos[] = {0.0f, 5.0f, 0.0f, w_component};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    if (isSpotlight) {
        GLfloat spotDir[] = {0.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
    } else {
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    }

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

    if (isShiny) {
        GLfloat mat_spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    } else {
        GLfloat mat_spec[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    }
}

// ---------------------------------- fungsi objek: torus
void drawTorus() {
    glColor3f(0.50f, 0.20f, 0.05f);
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);

    // Torus: inner radius, outer radius, sides, rings
    glutSolidTorus(0.2f, 0.6f, 30, 30);

    glPopMatrix();
}

// ---------------------------------- objek lain
void drawLightSource() {
    if (!lightingEnabled) return;

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(0.0f, 5.0f, 0.0f);

    if (isDirectional) glColor3f(1.0f, 1.0f, 0.0f);
    else glColor3f(1.0f, 1.0f, 1.0f);

    if (isSpotlight) glutWireSphere(0.2, 10, 10);
    else glutSolidSphere(0.15, 20, 20);

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawFloor() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.25f);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.35f);
    glBegin(GL_LINES);
    for (float i = -10.0f; i <= 10.0f; i += 1.0f) {
        glVertex3f(i, 0.01f, -10.0f); glVertex3f(i, 0.01f, 10.0f);
        glVertex3f(-10.0f, 0.01f, i); glVertex3f(10.0f, 0.01f, i);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// ---------------------------------- kamera
void updateCamera() {
    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;

    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = -sin(pitchRad);
    float forwardZ = -cos(yawRad) * cos(pitchRad);

    float rightX = sin(yawRad + PI/2);
    float rightZ = -cos(yawRad + PI/2);

    if (keys['w'] || keys['W']) {
        cameraPosX += forwardX * MOVE_SPEED;
        cameraPosY += forwardY * MOVE_SPEED;
        cameraPosZ += forwardZ * MOVE_SPEED;
    }
    if (keys['s'] || keys['S']) {
        cameraPosX -= forwardX * MOVE_SPEED;
        cameraPosY -= forwardY * MOVE_SPEED;
        cameraPosZ -= forwardZ * MOVE_SPEED;
    }
    if (keys['a'] || keys['A']) {
        cameraPosX -= rightX * MOVE_SPEED;
        cameraPosZ -= rightZ * MOVE_SPEED;
    }
    if (keys['d'] || keys['D']) {
        cameraPosX += rightX * MOVE_SPEED;
        cameraPosZ += rightZ * MOVE_SPEED;
    }

    if (keys[' ']) cameraPosY += MOVE_SPEED;
    if (keys[GLUT_KEY_CTRL_L] || keys[GLUT_KEY_CTRL_R]) cameraPosY -= MOVE_SPEED;
}

// ---------------------------------- display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth / windowHeight, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;

    float lookX = cameraPosX + sin(yawRad) * cos(pitchRad);
    float lookY = cameraPosY - sin(pitchRad);
    float lookZ = cameraPosZ - cos(yawRad) * cos(pitchRad);

    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, lookX, lookY, lookZ, 
              0.0f, 1.0f, 0.0f);

    updateLightingLogic();

    drawFloor();
    drawTorus();
    drawLightSource();

    glutSwapBuffers();
}

// ---------------------------------- input
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

    if (key == 'j' || key == 'J') { spotCutoff -= 2.0f; if (spotCutoff < 0) spotCutoff = 0; }
    if (key == 'k' || key == 'K') { spotCutoff += 2.0f; if (spotCutoff > 90) spotCutoff = 90; }

    if (key == 'n' || key == 'N') linearAttenuation += 0.05f;
    if (key == 'm' || key == 'M') { linearAttenuation -= 0.05f; if (linearAttenuation < 0) linearAttenuation = 0; }

    if (key == 'i' || key == 'I') isShiny = !isShiny;
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKey(int key, int x, int y) {
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R)
        keys[key] = true;
}

void specialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R)
        keys[key] = false;
}

void mouseMotion(int x, int y) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    if (x == centerX && y == centerY) return;

    float deltaX = (x - centerX) * MOUSE_SENSITIVITY * 50.0f;
    float deltaY = (y - centerY) * MOUSE_SENSITIVITY * 50.0f;

    cameraYaw += deltaX;
    cameraPitch += deltaY;

    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    glutWarpPointer(centerX, centerY);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseActive = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(windowWidth/2, windowHeight/2);
        lastMouseX = windowWidth/2;
        lastMouseY = windowHeight/2;
    }
}

void idle() {
    updateCamera();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("");

    glutFullScreen();

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    initLighting();

    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
