/*********
CTIS164 - Template Source Program
----------
STUDENT : Abdurahman Atakishiyev
SECTION : 02
HOMEWORK: Shooting Game - HW2
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
using  namespace std;
#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600
#define f first
#define s second
#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         0 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define START 0
#define GAME 1
#define SPEED 4

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //

/*Project Variables */
int state = START;
pair <int, int> p[5];
double speed[5] = { 0 };
int sec1 = 2, sec2 = 0, mSec1 = 0, mSec2 = 0;
int timerState = 0;
int coor[5] = { -140,-620,-1080,-460,-960 };
int playerY = 0, playerX = 0;
double visible = 255, visibleE[5] = { 255,255,255,255,255 };
int totalEnemy = 0, burst = 0, pts = 0, lastpt = 0, px, ex1, ex2, hit;

void Init();

void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
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
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
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
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void drawGradient(int x1, int y1, int w, int h, float r, float g, float b) {
	glBegin(GL_QUADS);
	glColor3ub(r, g, b);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glColor3ub(r + 102, g + 102, b + 102);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
}
//Drawing player rocket launcher
void player() {
	
	glColor4ub(75, 75, 75, visible);
	glRectf(-75 + playerX, -270 + playerY, -60 + playerX, -240 + playerY);
	glColor4ub(0, 0, 0, visible);
	glBegin(GL_TRIANGLES);
	glVertex2f(-78 + playerX, -240 + playerY);
	glVertex2f(-68 + playerX, -230 + playerY);
	glVertex2f(-57 + playerX, -240 + playerY);

	glVertex2f(-84 + playerX, -270 + playerY);
	glVertex2f(-75 + playerX, -270 + playerY);
	glVertex2f(-75 + playerX, -250 + playerY);

	glVertex2f(-51 + playerX, -270 + playerY);
	glVertex2f(-60 + playerX, -270 + playerY);
	glVertex2f(-60 + playerX, -250 + playerY);
	glEnd();
	glColor3ub(26, 56, 37);
	glRectf(-100 + playerX, -273, -30 + playerX, -285);
	glRectf(-30 + playerX, -285, playerX, -235);
	glRectf(-90 + playerX, -273, -84 + playerX, -250);
	glRectf(-50 + playerX, -273, -44 + playerX, -250);
	glColor3f(0, 0, 0);
	circle(-90 + playerX, -288, 10);
	circle(-35 + playerX, -288, 10);
	glRectf(-15 + playerX,-270, playerX,-245);
	glColor3f(1, 1, 1);
	circle(-90 + playerX, -288, 5);
	circle(-35 + playerX, -288, 5);
}
//Drawing enemy planes
void enemy(pair <int, int> p, int i) {
	//Primary Color assignment
	glColor4ub(99, 110, 114,visibleE[i]);
	//Body
	glRectf(-470 + p.f, 230 + p.s, -390 + p.f, 210 + p.s);
	circle(-463 + p.f , 220 + p.s, 10);
	//Wings and front
	glBegin(GL_TRIANGLES);
	glVertex2f(-390 + p.f , 230 + p.s);
	glVertex2f(-390 + p.f , 210 + p.s);
	glVertex2f(-360 + p.f, 210 + p.s);

	glColor4ub(0, 0, 0, visibleE[i]);
	glVertex2f(-470 + p.f , 245 + p.s);
	glVertex2f(-470 + p.f, 230 + p.s);
	glVertex2f(-440 + p.f , 230 + p.s);

	glVertex2f(-470 + p.f, 223 + p.s);
	glVertex2f(-475 + p.f, 218 + p.s);
	glVertex2f(-455 + p.f, 223 + p.s);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(-435 + p.f, 218 + p.s);
	glVertex2f(-420 + p.f, 218 + p.s);
	glVertex2f(-440 + p.f , 195 + p.s);
	glVertex2f(-445 + p.f , 195 + p.s);
	glEnd();
	//Windows
	glColor4ub(0, 0, 0, visibleE[i]);
	circle(-410 + p.f , 223 + p.s, 2);
	circle(-418 + p.f , 223 + p.s, 2);
	circle(-426 + p.f , 223 + p.s, 2);
	circle(-434 + p.f, 223 + p.s, 2);
	circle(-442 + p.f , 223 + p.s, 2);
	//Cabin Glass
	glColor4ub(0, 0, 0, visibleE[i]);
	glBegin(GL_QUADS);
	glVertex2f(-400 + p.f, 225 + p.s);
	glVertex2f(-400 + p.f , 222 + p.s);
	glVertex2f(-377 + p.f , 222 + p.s);
	glVertex2f(-382 + p.f , 225 + p.s);
	glEnd();

}
//Basic graphic elements present in all states
void basis() {
	
	glColor3f(0, 0, 0);
	glRectf(600, 300, -600, 250);
	glRectf(600, 250, 450, -300);
	glColor3f(1, 1, 1);
	vprint(-580, 285, GLUT_BITMAP_9_BY_15, "  TIME");

	vprint(-380, 285, GLUT_BITMAP_9_BY_15, "  TOTAL R.");
	
	vprint(-90, 285, GLUT_BITMAP_9_BY_15, "  BURST R.");

	vprint(260, 285, GLUT_BITMAP_9_BY_15, "  LAST pt");

	vprint(460, 285, GLUT_BITMAP_9_BY_15, "  TOTAL pts");

	glColor3f(0, 1, 0);
	vprint(-565, 265, GLUT_BITMAP_8_BY_13, "%d%d:%d%d",  sec1, sec2, mSec1, mSec2);
	vprint(-330, 265, GLUT_BITMAP_9_BY_15, "%d", totalEnemy);
	vprint(-50, 265, GLUT_BITMAP_9_BY_15, "%d", burst);
	vprint(300, 265, GLUT_BITMAP_9_BY_15, "%d", lastpt);
	glColor3f(1, 0, 0);
	vprint2(510, 245, 0.25, "%d", pts);

}

void drawStart() {
	timerState = 0;
	mSec1 = mSec2 = sec2 = 0;
	sec1 = 2;
	playerY = 0;
	Init();
	player();
	basis();
	glColor3f(1, 1, 0);
	vprint2(480, 120, 0.33, "<F1>");
	vprint2(500, 60, 0.33, "for");
	vprint2(490, 0, 0.33, "new");
	vprint2(475, -60, 0.33, "game");
}

void drawGame() {
	player();
	for (int i = 0; i < 5; i++)
	{
		enemy(p[i], i);
	}
	basis();
	glColor3f(1, 1, 0);
	vprint2(475, 170, 0.3, "<F1>");
	vprint2(500, 110, 0.3, "to");
	vprint2(472, 50, 0.3, "pause");
	vprint2(500, 0, 0.3, "or");
	vprint2(460, -40, 0.3, "resume");
	vprint2(450, -80, 0.3, "---------");
	vprint2(460, -120, 0.3, "SPACE");
	vprint2(500, -180, 0.3, "to");
	vprint2(490, -240, 0.3, "fire");

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
	drawGradient(-600, 300, 1200, 600, 52, 31, 151);
	switch (state) {
	case START:
		drawStart();
		break;
	case GAME:
		drawGame();
		break;
	}
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
	if (key == 32 && playerY == 0 && state == GAME)
		playerY = 1;
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
	case GLUT_KEY_LEFT: if(state==GAME)if (playerX >= -500)playerX -= 7; break;
	case GLUT_KEY_RIGHT:if (state == GAME) if (playerX <= 450)playerX += 7; break;
	case GLUT_KEY_F1: if (state == START)
	{ 
		burst = lastpt = totalEnemy = pts = 0;
		state = GAME;
		timerState = 1; 
	}
	else {
		  if (timerState == 1)
			  timerState = 0;
		  else
			  timerState = 1;
		  }
			  break;
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



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (timerState == 1) {
		if (mSec2 == 0) {
			if (mSec1 == 0) {
				if (sec2 == 0) {
					sec1--;
					sec2 = 10;
				}
				sec2--;
				mSec1 = 10;
			}
			mSec1--;
			mSec2 = 9;
		}
		mSec2--;
		if (sec1 < 0)
		{
			state = START;
			timerState = 0;
		}
		for (int i = 0; i < 5; i++)
		{
			if (p[i].first == -100)
				totalEnemy++;
			if (p[i].f >= 1060)
			{
				p[i].s = rand() % 5 * (-1) * 50;
				p[i].f = -220;
				visibleE[i] = 255;
				glutPostRedisplay();
			}
			p[i].f += SPEED;
			px = (-78 + playerX);
			ex1 = (-473 + p[i].f);
			ex2 = (-360 + p[i].f);
			hit = abs(px - (ex1 + ex2) / 2);
			if (px > ex1 && (-78 + playerX) < ex2 && (-250 + playerY) < (240 + p[i].s) && (-250 + playerY) > (200 + p[i].s))
			{
				if (hit < 5 && hit>0)
					lastpt = 5;
				else if (hit > 113 / 2 && hit < 5 + 113 / 2)
					lastpt = 1;
				else if (hit == 113 / 4)
					lastpt = 3;
				else if (hit < 113 / 2 && hit > 113 / 4)
					lastpt = 2;
				else if (hit < 113 / 4 && hit>0)
					lastpt = 4;
				pts += lastpt;
				visibleE[i] = 0;
				p[i].s += 900;
				burst++;
			}
		}
		if (playerY >= 600)
		{
			visible = 255;
			playerY = 0;
		}
		else if (playerY != 0)
			playerY += 8;
	}
	glutPostRedisplay();
}
void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 5; i++)
	{
		p[i].s = (-1)*i * 80;
	}
	p[0].first = coor[0];
	p[1].first = coor[1];
	p[2].first = coor[2];
	p[3].first = coor[3];
	p[4].first = coor[4];

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("\"Plane Shotter\" by Abdurahman Atakishiyev");
	srand(time(NULL));
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);


	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	Init();

	glutMainLoop();
}
