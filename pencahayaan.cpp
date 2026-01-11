#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

// ================= KONSTANTA =================
static constexpr float PI = 3.14159265358979323846f;

// ================= KAMERA (PERTEMUAN9000) =================
float MOVE_SPEED = 0.04f;
float MOUSE_SENSITIVITY = 0.1f;

int windowWidth = 1200;
int windowHeight = 800;

float cameraPosX = 0.0f;
float cameraPosY = 2.0f;
float cameraPosZ = 10.0f;

float cameraYaw   = 180.0f;
float cameraPitch = 0.0f;

bool keysDown[256] = { false };
bool mouseActive = false;

// ================= LIGHTING (TETAP) =================
bool lightingEnabled = true;
int lightMode = 1;
bool isDirectional = false;
bool isSpotlight = false;
float spotCutoff = 45.0f;
float spotExponent = 2.0f;
float linearAttenuation = 0.0f;
bool isShiny = true;

// ================= DINDING SETENGAH DENGAN PINTU GESER =================
float doorOffset = 0.0f;  // Posisi pintu geser (0 untuk tertutup, -7 untuk terbuka)
bool doorOpen = false;    // Status pintu
float doorSpeed = 0.1f;   // Kecepatan pergerakan pintu

// ===================================================
// ================= LIGHTING =========================
void initLighting()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    GLfloat mat_specular[]  = { 0.7f,0.7f,0.7f,1 };
    GLfloat mat_shininess[] = { 32 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void updateLightingLogic()
{
    if (!lightingEnabled) {
        glDisable(GL_LIGHTING);
        return;
    }
    glEnable(GL_LIGHTING);

    GLfloat ambient[]  = {0.2f,0.2f,0.2f,1};
    GLfloat diffuse[]  = {0.8f,0.8f,0.8f,1};
    GLfloat specular[] = {1,1,1,1};
    GLfloat black[]    = {0,0,0,1};

    if(lightMode==1){
        glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    }
    if(lightMode==2){
        glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,black);
        glLightfv(GL_LIGHT0,GL_SPECULAR,black);
    }
    if(lightMode==3){
        glLightfv(GL_LIGHT0,GL_AMBIENT,black);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,black);
    }
    if(lightMode==4){
        glLightfv(GL_LIGHT0,GL_AMBIENT,black);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,black);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    }

    GLfloat w = isDirectional ? 0.0f : 1.0f;
    GLfloat lightPos[] = {0.0f, 4.8f, 0.0f, w};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    if(isSpotlight){
        GLfloat dir[] = {0,-1,0};
        glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,dir);
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,spotCutoff);
        glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,spotExponent);
    } else {
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180);
    }

    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,linearAttenuation);

    GLfloat specMat[] = {isShiny?1.0f:0.0f,isShiny?1.0f:0.0f,isShiny?1.0f:0.0f,1};
    glMaterialfv(GL_FRONT,GL_SPECULAR,specMat);
}

// ===================================================
// ================= PRIMITIVE ========================
void drawScaledCube(float x,float y,float z)
{
    glPushMatrix();
    glScalef(x,y,z);
    glutSolidCube(1);
    glPopMatrix();
}

void drawCylinder(float r,float t)
{
    GLUquadric* q=gluNewQuadric();
    gluCylinder(q,r,r,t,20,4);
    gluDisk(q,0,r,20,1);
    glPushMatrix();
    glTranslatef(0,0,t);
    gluDisk(q,0,r,20,1);
    glPopMatrix();
    gluDeleteQuadric(q);
}

// ===================================================
// ================= OBJEK ============================
void drawFloor()
{
    glBegin(GL_QUADS);
    glColor3f(0.2f,0.2f,0.25f);
    glVertex3f(-10,0,-10);
    glVertex3f(-10,0,10);
    glVertex3f(10,0,10);
    glVertex3f(10,0,-10);
    glEnd();

    glDisable(GL_LIGHTING);
    glColor3f(0.3f,0.3f,0.35f);
    glBegin(GL_LINES);
    for(float i=-10;i<=10;i++){
        glVertex3f(i,0.01f,-10); glVertex3f(i,0.01f,10);
        glVertex3f(-10,0.01f,i); glVertex3f(10,0.01f,i);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawRoom()
{
    glColor3f(0.75f,0.62f,0.45f);
    // Dinding belakang
    glPushMatrix(); glTranslatef(0,2.7f,-6); drawScaledCube(14,6,0.22f); glPopMatrix();
    // Dinding kanan
    glPushMatrix(); glTranslatef(7,2.7f,0);  drawScaledCube(0.22f,6,12); glPopMatrix();
    // Dinding kiri
    glPushMatrix(); glTranslatef(-7,2.7f,0); drawScaledCube(0.22f,6,12); glPopMatrix();
    // Atap
    glPushMatrix(); glTranslatef(0,6,0);     drawScaledCube(14,0.22f,12); glPopMatrix();
    // Lantai
    glColor3f(0.68f,0.56f,0.40f);
    glPushMatrix(); drawScaledCube(14,0.22f,12); glPopMatrix();
}

// ================= DINDING SETENGAH DI SISI DEPAN =================
void drawHalfWallWithSlidingDoor()
{
    // Warna dinding (sedikit berbeda dari ruangan)
    glColor3f(0.70f,0.57f,0.40f);
    
    // Dinding setengah tinggi (tinggi 3, setengah dari tinggi ruangan)
    float wallHeight = 3.0f;
    float wallThickness = 0.22f;
    
    // Posisi dinding: di sisi depan (z = 6) yang tadinya tidak ada dinding
    // Dinding ini terdiri dari 2 bagian: kiri dan kanan, dengan celah di tengah untuk pintu
    float wallLength = 7.0f; // Panjang masing-masing bagian dinding
    
    // Bagian kiri dinding (tetap)
    glPushMatrix();
    glTranslatef(-6.5f, wallHeight/2, 6.0f);
    drawScaledCube(wallLength, wallHeight, wallThickness);
    glPopMatrix();
    
    // Bagian kanan dinding (bagian yang bergerak sebagai pintu)
    glPushMatrix();
    // doorOffset: 0 = tertutup (posisi semula), -6.5 = terbuka (geser ke kiri)
    glTranslatef(6.5f + doorOffset, wallHeight/2, 6.0f);
    drawScaledCube(wallLength, wallHeight, wallThickness);
    glPopMatrix();
}

// Fungsi untuk mengupdate posisi pintu
void updateDoor()
{
    if (doorOpen && doorOffset > -6.5f) {
        // Pintu terbuka: geser ke kiri
        doorOffset -= doorSpeed;
        if (doorOffset < -6.5f) doorOffset = -6.5f;
    } else if (!doorOpen && doorOffset < 0.0f) {
        // Pintu tertutup: geser ke kanan
        doorOffset += doorSpeed;
        if (doorOffset > 0.0f) doorOffset = 0.0f;
    }
}

// Fungsi untuk membuka/tutup pintu
void toggleDoor()
{
    doorOpen = !doorOpen;
    printf("Pintu %s\n", doorOpen ? "DIBUKA" : "DITUTUP");
}

// Fungsi untuk menampilkan status pintu di layar
void displayDoorStatus()
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(-4.0f, 5.0f, 0.0f);
    
    const char* status = doorOpen ? "Pintu: TERBUKA (Tekan K untuk menutup)" : "Pintu: TERTUTUP (Tekan K untuk membuka)";
    for (const char* c = status; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    glEnable(GL_LIGHTING);
}

void drawCar()
{
    glPushMatrix();
    glTranslatef(-2,0.7f,0);
    glColor3f(1,0.18f,0.18f);
    drawScaledCube(3,1,1.7f);
    glPushMatrix(); glTranslatef(0,0.85f,-0.1f); drawScaledCube(1.8f,1,1.3f); glPopMatrix();
    glColor3f(0.06f,0.06f,0.06f);
    glPushMatrix(); glTranslatef(1.25f,-0.45f,0.9f); glRotatef(90,0,1,0); drawCylinder(0.45f,0.35f); glPopMatrix();
    glPushMatrix(); glTranslatef(-1.25f,-0.45f,0.9f); glRotatef(90,0,1,0); drawCylinder(0.45f,0.35f); glPopMatrix();
    glPushMatrix(); glTranslatef(1.25f,-0.45f,-0.9f); glRotatef(90,0,1,0); drawCylinder(0.45f,0.35f); glPopMatrix();
    glPushMatrix(); glTranslatef(-1.25f,-0.45f,-0.9f); glRotatef(90,0,1,0); drawCylinder(0.45f,0.35f); glPopMatrix();
    glPopMatrix();
}

// ================= RAK =================
void drawRack()
{
    glColor3f(0.55f,0.32f,0.15f);

    glPushMatrix(); glTranslatef(4.5f,1.8f,-2); drawScaledCube(0.2f,3.5f,0.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(6.0f,1.8f,-2); drawScaledCube(0.2f,3.5f,0.2f); glPopMatrix();

    for(int i=0;i<3;i++){
        glPushMatrix();
        glTranslatef(5.25f,0.6f+i*1.2f,-2);
        drawScaledCube(2.2f,0.2f,2);
        glPopMatrix();
    }
}

// ================= KARDUS =================
void drawBox(float x,float y,float z)
{
    glColor3f(0.9f,0.65f,0.25f);
    glPushMatrix();
    glTranslatef(x,y,z);
    drawScaledCube(1,1,1);
    glPopMatrix();
}

// ================= KURSI =================
void drawChair()
{
    glColor3f(0.3f,0.3f,0.3f);

    // 4 tiang
    glPushMatrix(); glTranslatef(-5.2f,0.9f,3.8f); drawScaledCube(0.15f,1.8f,0.15f); glPopMatrix();
    glPushMatrix(); glTranslatef(-4.8f,0.9f,3.8f); drawScaledCube(0.15f,1.8f,0.15f); glPopMatrix();
    glPushMatrix(); glTranslatef(-5.2f,0.9f,3.2f); drawScaledCube(0.15f,1.8f,0.15f); glPopMatrix();
    glPushMatrix(); glTranslatef(-4.8f,0.9f,3.2f); drawScaledCube(0.15f,1.8f,0.15f); glPopMatrix();

    // Dudukan
    glPushMatrix(); glTranslatef(-5.0f,1.8f,3.5f); drawScaledCube(0.8f,0.15f,0.8f); glPopMatrix();

    // Sandaran
    glPushMatrix(); glTranslatef(-5.0f,2.6f,3.1f); drawScaledCube(0.8f,1.2f,0.15f); glPopMatrix();
}

void drawLightSource()
{
    if(!lightingEnabled) return;
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(0,4.8f,0);
    glutSolidSphere(0.15,20,20);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// ================= MOVEMENT =================
void updateMovement()
{
    float yawRad = cameraYaw * PI / 180.0f;
    float forwardX = sinf(yawRad);
    float forwardZ = -cosf(yawRad);
    float rightX   = sinf(yawRad + PI/2);
    float rightZ   = -cosf(yawRad + PI/2);

    if(keysDown['w']||keysDown['W']){ cameraPosX+=forwardX*MOVE_SPEED; cameraPosZ+=forwardZ*MOVE_SPEED; }
    if(keysDown['s']||keysDown['S']){ cameraPosX-=forwardX*MOVE_SPEED; cameraPosZ-=forwardZ*MOVE_SPEED; }
    if(keysDown['a']||keysDown['A']){ cameraPosX-=rightX*MOVE_SPEED;   cameraPosZ-=rightZ*MOVE_SPEED; }
    if(keysDown['d']||keysDown['D']){ cameraPosX+=rightX*MOVE_SPEED;   cameraPosZ+=rightZ*MOVE_SPEED; }
    if(keysDown['q']||keysDown['Q']) cameraPosY+=MOVE_SPEED;
    if(keysDown['e']||keysDown['E']) cameraPosY-=MOVE_SPEED;
}

// ================= DISPLAY =================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateMovement();
    updateDoor(); // Update posisi pintu

    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;

    float lx = sinf(yawRad) * cosf(pitchRad);
    float ly = -sinf(pitchRad);
    float lz = -cosf(yawRad) * cosf(pitchRad);

    gluLookAt(cameraPosX,cameraPosY,cameraPosZ,
              cameraPosX+lx,cameraPosY+ly,cameraPosZ+lz,
              0,1,0);

    updateLightingLogic();

    drawFloor();
    drawRoom();
    drawHalfWallWithSlidingDoor(); // Dinding setengah di sisi depan
    drawCar();
    drawRack();
    drawBox(5.25f,1.0f,-2);
    drawBox(5.25f,2.2f,-2);
    drawChair();
    drawLightSource();
    displayDoorStatus(); // Tampilkan status pintu

    glutSwapBuffers();
}

// ================= INPUT =================
void keyboard(unsigned char k,int,int)
{
    keysDown[k]=true;
    if(k==27) exit(0);
    if(k=='l'||k=='L') lightingEnabled=!lightingEnabled;
    if(k>='1'&&k<='4') lightMode=k-'0';
    if(k=='y'||k=='Y') isDirectional=!isDirectional;
    if(k=='h'||k=='H') isSpotlight=!isSpotlight;
    if(k=='n'||k=='N') linearAttenuation+=0.05f;
    if(k=='m'||k=='M') linearAttenuation-=0.05f;
    if(k=='k'||k=='K') toggleDoor(); // Tombol K untuk membuka/menutup pintu
}
void keyboardUp(unsigned char k,int,int){ keysDown[k]=false; }

void mouse(int b,int s,int,int)
{
    if(b==GLUT_LEFT_BUTTON && s==GLUT_DOWN){
        mouseActive=true;
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(windowWidth/2,windowHeight/2);
    }
}

void mouseMotion(int x,int y)
{
    if(!mouseActive) return;
    int cx=windowWidth/2, cy=windowHeight/2;
    cameraYaw   += (x-cx)*MOUSE_SENSITIVITY;
    cameraPitch += (cy-y)*MOUSE_SENSITIVITY;
    if(cameraPitch>89) cameraPitch=89;
    if(cameraPitch<-89) cameraPitch=-89;
    glutWarpPointer(cx,cy);
}

void reshape(int w,int h)
{
    windowWidth=w; windowHeight=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(double)w/h,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}

// ================= MAIN =================
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Pencahayaan + Kamera + Pintu Geser");

    glClearColor(0.12f,0.12f,0.14f,1);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(display);

    printf("========================================\n");
    printf("KONTROL PROGRAM:\n");
    printf("========================================\n");
    printf("Gerakan Kamera:\n");
    printf("  W/A/S/D - Bergerak maju/kiri/mundur/kanan\n");
    printf("  Q/E      - Naik/turun\n");
    printf("  Mouse    - Melihat sekitar\n");
    printf("\nPencahayaan:\n");
    printf("  L        - Toggle lighting\n");
    printf("  1-4      - Mode pencahayaan\n");
    printf("  Y        - Toggle directional light\n");
    printf("  H        - Toggle spotlight\n");
    printf("  N/M      - Attenuation +/- \n");
    printf("\nPintu Geser:\n");
    printf("  K        - Buka/tutup pintu\n");
    printf("========================================\n");

    glutMainLoop();
    return 0;
}