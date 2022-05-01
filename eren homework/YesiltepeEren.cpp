/*********
   CTIS164 - Template Source Program
----------
STUDENT :Eren Yeþiltepe
SECTION :02
HOMEWORK:3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
1)far lights added
2)walls are colored /done
3)path of the car is drawn(linked list structure is implemented)
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

#define TIMER_PERIOD  32 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height



typedef struct {
    double x, y;
}vertex_t;

typedef struct {
    polar_t speed,light;
    vec_t pos;
}car_t;


vertex_t mouse;
car_t car;

bool paused = 0, mouseout = 0;
bool lightVis = 1;
int lightmode = 0;//0:long far,1:short far,2:far off
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void vertex(vertex_t P, vertex_t Tr, double angle)
{
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

void circleVer(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        vertex({ x + r * cos(angle) ,y + r * sin(angle) }, { car.pos.x,car.pos.y }, (car.speed.angle-90)* D2R);
    }
    glEnd();
}

void elipseVer(int x, int y, int r1,int r2)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        vertex({ x + r1 * cos(angle) ,y + r2 * sin(angle) }, { car.pos.x,car.pos.y }, (car.speed.angle - 90) * D2R);
    }
    glEnd();
}


void drawCar(car_t car) 
{
    float s = 3;
    double anglecar = (car.speed.angle-90) * D2R;
    glColor3f(1, 0, 0);


    //base
    glBegin(GL_POLYGON);
    vertex({ -5 * s,16 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 5 * s,16 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 6 * s,13 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 6 * s,-13 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 5 * s,-16 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -5 * s,-16 * s }, { car.pos.x,car.pos.y }, anglecar);   
    vertex({ -6 * s,-13 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -6 * s,13 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd(); 

    glColor3ub(0, 0, 255);
    glBegin(GL_POLYGON);
    vertex({ -5 * s,8 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 5* s,8 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 4 * s,4 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -4 * s,4 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    glBegin(GL_POLYGON);
    vertex({ -3 * s,2 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 3 * s,2 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 3 * s,0 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -3 * s,0 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    //bot rectangle
    glBegin(GL_POLYGON);
    vertex({ -4 * s,-9 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 4 * s,-9 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 4 * s,-12 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -4 * s,-12 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    //wheels
    glBegin(GL_POLYGON);
    vertex({ -7 * s,11 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -6 * s,11 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -6 * s,6 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -7 * s,6 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    glBegin(GL_POLYGON);
    vertex({ 6 * s,11 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 7* s,11 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 7 * s,6 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 6 * s,6 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    glBegin(GL_POLYGON);
    vertex({ 6 * s,-7 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 7 * s,-7 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 7 * s,-12 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ 6 * s,-12 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    glBegin(GL_POLYGON);
    vertex({ -7 * s,-7 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -6 * s,-7 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -6 * s,-12 * s }, { car.pos.x,car.pos.y }, anglecar);
    vertex({ -7 * s,-12 * s }, { car.pos.x,car.pos.y }, anglecar);
    glEnd();

    //fars
   glColor3f(0.5, 0.5, 0);

   circleVer(-4*s, 14*s, s);
   circleVer(4*s , 14*s , s);
  

   if (lightmode!=2)
   {
       double xp = (0 - (car.light.magnitude  + 14) * s * sin(anglecar)) + car.pos.x;
       double yp = (0 + (car.light.magnitude+14) * s * cos(anglecar)) + car.pos.y;
       float changelight;

       printf("yp:%.2f,xp:%.2f\n", yp, xp);

       if (xp < -400)
           changelight = (-400 - xp) / 10.;
       else if (xp > 400)
           changelight = (xp - 400) / 10.;
       else if (yp > 300)
           changelight = (yp - 300) / 10.;
       else if (yp < -300)
           changelight = ( - 300-yp) / 10.;
       else
           changelight = 0;

       car.light.magnitude -= 2*changelight;
       glColor4f(66/255., 239/255., 245/255.,0.5);

       glBegin(GL_TRIANGLES);
       vertex({0,14 * s }, { car.pos.x,car.pos.y }, anglecar);
       vertex({ -car.light.magnitude/6.*powf(3,1/2.) * s,(car.light.magnitude*3/4.+14) * s }, { car.pos.x,car.pos.y }, anglecar);
       vertex({ car.light.magnitude / 6. * powf(3,1 / 2.) * s,(car.light.magnitude * 3 / 4. + 14) * s }, { car.pos.x,car.pos.y }, anglecar);
       glEnd();


       glColor4f(66 / 255., 239 / 255., 245 / 255., 0.5+changelight/10);
       elipseVer(0, car.light.magnitude*s + 12 * s- changelight / 2, car.light.magnitude / 3*s , car.light.magnitude/3*s- changelight);
       
   }

}

void drawBg()
{
    //left wall
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(-450, 350);
    glVertex2f(-400, 300);
    glVertex2f(-400, -300);
    glVertex2f(-450, -350);
    glEnd();

    //top wall
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
    glVertex2f(-450, 350);
    glVertex2f(-400, 300);
    glVertex2f(400, 300);
    glVertex2f(450, 350);
    glEnd();

    //right wall
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
    glVertex2f(450, 350);
    glVertex2f(400, 300);
    glVertex2f(400, -300);
    glVertex2f(450, -350);
    glEnd();

    //bottom wall
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(-450, -350);
    glVertex2f(-400, -300);
    glVertex2f(400, -300);
    glVertex2f(450, -350);
    glEnd();

    //surface
    glColor3f(0.4, 0.4, 0.4);
    glRectf(-400, 300, 400, -300);

    glColor3f(0, 0, 0.8);
    vprint2(-100, winHeight / 2. - 50, 0.2, "Eren Yesiltepe");

        if(paused)
             vprint2(-150, -winHeight / 2. + 30, 0.15, "Paused. Enter spacebar to continue");
        else
            vprint2(-150, -winHeight / 2. + 30, 0.15, "Playing. Enter spacebar to pause");

        if(mouseout)
            vprint2(-350, -winHeight / 2. + 30, 0.15, "Mouse is outside");
        else
            vprint2(-350, -winHeight / 2. + 30, 0.15, "Mouse is inside");

        vprint2(210, -winHeight / 2. + 30, 0.15, "Car angle=%0.2f",car.speed.angle);
}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBg();
    drawCar(car);

    glColor3f(0, 0, 1);
    circle(mouse.x, mouse.y, 5);


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ' && paused == 0)
        paused = true;
    else if (key == ' ' && paused == 1)
        paused = false;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        lightmode++;
        if (lightmode == 3)
            lightmode = 0;

        if (lightmode == 0)
            car.light.magnitude = 30;
        else if (lightmode == 1)
            car.light.magnitude = 20;

    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.
    int x2 = x - winWidth / 2;
    int y2 = winHeight / 2 - y;

    

    if (!paused)
    {
        mouse.x = x - winWidth / 2;
        mouse.y = winHeight / 2 - y;

        if (x2 > -400 && x2 < 400 && y2<300 && y2>-300)
        {
            mouseout = false;

            car.speed.angle = atan2(mouse.y - car.pos.y, mouse.x - car.pos.x) / D2R;
            if (car.speed.angle < 0)
                car.speed.angle += 360;
        }
        else {
            mouseout = true;
            car.speed.magnitude = 10;
        }
    }
   
        
    
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (!paused)
    {
        if (car.pos.x<mouse.x + 20 && car.pos.x>mouse.x - 20 && car.pos.y<mouse.y + 20 && car.pos.y>mouse.y - 20)
            car.speed.magnitude = 0;
        else {
            car.speed.magnitude = 10;
            car.pos = addV(car.pos, pol2rec(car.speed));
        }
        printf("mag:%0.2f\n", car.speed.magnitude);

        vec_t tmp;
        if (mouseout)
            if ((car.pos.x > -390 && car.pos.x < -370) || (car.pos.x < 390 && car.pos.x > 370))
            {
                tmp = pol2rec(car.speed);
                tmp = { -tmp.x,tmp.y };
                car.speed = rec2pol(tmp);
            }
            else if ((car.pos.y > -290 && car.pos.y < -270) || (car.pos.y < 290 && car.pos.y > 270))
            {
                tmp = pol2rec(car.speed);
                tmp = { tmp.x,-tmp.y };
                car.speed = rec2pol(tmp);
            }
    }

    

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Eren Yeþiltepe-Catch the mouse pointer");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    car.speed.magnitude = 10;
    car.speed.angle = 0;
    car.light.angle = 0;
    car.light.magnitude = 30;


    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}