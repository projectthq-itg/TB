#include <GL/freeglut.h>
#include <stdio.h>

/* Batas window kliping */
float xmin = 100, ymin = 100;
float xmax = 400, ymax = 300;

/* Kode region */
#define INSIDE 0   // 0000
#define LEFT   1   // 0001
#define RIGHT  2   // 0010
#define BOTTOM 4   // 0100
#define TOP    8   // 1000

/* Titik garis */
float xStart = 50,  yStart = 150;
float xEnd   = 450, yEnd   = 250;

/* Hitung kode region */
int computeCode(float x, float y) {
    int code = INSIDE;

    if (x < xmin)       code |= LEFT;
    else if (x > xmax)  code |= RIGHT;

    if (y < ymin)       code |= BOTTOM;
    else if (y > ymax)  code |= TOP;

    return code;
}

/* Algoritma Cohen–Sutherland */
void cohenSutherland(float *x1, float *y1, float *x2, float *y2) {
    int code1 = computeCode(*x1, *y1);
    int code2 = computeCode(*x2, *y2);
    int accept = 0;

    while (1) {
        /* Trivially accepted */
        if ((code1 == 0) && (code2 == 0)) {
            accept = 1;
            break;
        }
        /* Trivially rejected */
        else if (code1 & code2) {
            break;
        }
        /* Perlu dipotong */
        else {
            float x, y;
            int outcode = code1 ? code1 : code2;

            float m = (*y2 - *y1) / (*x2 - *x1);

            /* Atas */
            if (outcode & TOP) {
                y = ymax;
                x = *x1 + (ymax - *y1) / m;
            }
            /* Bawah */
            else if (outcode & BOTTOM) {
                y = ymin;
                x = *x1 + (ymin - *y1) / m;
            }
            /* Kanan */
            else if (outcode & RIGHT) {
                x = xmax;
                y = *y1 + m * (xmax - *x1);
            }
            /* Kiri */
            else if (outcode & LEFT) {
                x = xmin;
                y = *y1 + m * (xmin - *x1);
            }

            if (outcode == code1) {
                *x1 = x;
                *y1 = y;
                code1 = computeCode(*x1, *y1);
            } else {
                *x2 = x;
                *y2 = y;
                code2 = computeCode(*x2, *y2);
            }
        }
    }

    /* Jika ditolak, kosongkan garis */
    if (!accept) {
        *x1 = *y1 = *x2 = *y2 = 0;
    }
}

/* Fungsi display */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    /* Window kliping (PUTIH) */
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
    glEnd();

    /* Garis sebelum kliping (MERAH) */
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
        glVertex2f(xStart, yStart);
        glVertex2f(xEnd, yEnd);
    glEnd();

    /* Proses kliping */
    float cx1 = xStart, cy1 = yStart;
    float cx2 = xEnd,   cy2 = yEnd;
    cohenSutherland(&cx1, &cy1, &cx2, &cy2);

    /* Garis setelah kliping (HIJAU) */
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
        glVertex2f(cx1, cy1);
        glVertex2f(cx2, cy2);
    glEnd();

    glFlush();
}

/* Inisialisasi OpenGL */
void init() {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 400);
}

/* Main */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 500);
    glutCreateWindow("Cohen–Sutherland Line Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
