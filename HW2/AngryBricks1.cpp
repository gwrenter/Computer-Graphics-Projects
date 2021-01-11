//-----------------------------------------------------
// CSCE 4813 Programming Project #2
// Author: Gregory Renteria
// Code: AngryBricks.cpp
// Goal: create an OpenGL program that displays
//       a colorful brick using the display callback,
//		 drag it across the screen using the mouse
//       callback.
//------------------------------------------------------

#include <GL/glu.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

// Global constants
#define MIN_X_VIEW -50
#define MAX_X_VIEW  50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW  50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW  50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500

// Global variables
float xCoordinate, yCoordinate;

//----------------------------
// Init function for OpenGL
//----------------------------
void init(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(MIN_X_VIEW, MAX_X_VIEW,
		MIN_Y_VIEW, MAX_Y_VIEW,
		MIN_Z_VIEW, MAX_Z_VIEW);
}

//---------------------------
// Function to draw a cube
//---------------------------
void drawCube(float midx, float midy, float midz, float size){
	// Define 8 vertices
   	float ax = midx - size / 2;
   	float ay = midy - size / 2;
   	float az = midz + size / 2;
  	float bx = midx + size / 2;
   	float by = midy - size / 2;
   	float bz = midz + size / 2;
   	float cx = midx + size / 2;
  	float cy = midy + size / 2;
   	float cz = midz + size / 2;
   	float dx = midx - size / 2;
   	float dy = midy + size / 2;
   	float dz = midz + size / 2;
   	float ex = midx - size / 2;
   	float ey = midy - size / 2;
   	float ez = midz - size / 2;
   	float fx = midx + size / 2;
   	float fy = midy - size / 2;
  	float fz = midz - size / 2;
   	float gx = midx + size / 2;
   	float gy = midy + size / 2;
   	float gz = midz - size / 2;
   	float hx = midx - size / 2;
   	float hy = midy + size / 2;
   	float hz = midz - size / 2;

   	// Draw 6 faces
   	glBegin(GL_POLYGON);
   	glColor3f(0.5, 0.0, 0.0);
   	glVertex3f(ax, ay, az);
   	glVertex3f(bx, by, bz);
   	glVertex3f(cx, cy, cz);
   	glVertex3f(dx, dy, dz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(1.0, 0.0, 0.0);
   	glVertex3f(ax, ay, az);
   	glVertex3f(dx, dy, dz);
   	glVertex3f(hx, hy, hz);
   	glVertex3f(ex, ey, ez);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(0.5, 0.0, 0.0);
   	glVertex3f(ax, ay, az);
   	glVertex3f(ex, ey, ez);
   	glVertex3f(fx, fy, fz);
   	glVertex3f(bx, by, bz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(1.0, 0.0, 0.0);
   	glVertex3f(gx, gy, gz);
   	glVertex3f(fx, fy, fz);
   	glVertex3f(ex, ey, ez);
   	glVertex3f(hx, hy, hz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(0.5, 0.0, 0.0);
   	glVertex3f(gx, gy, gz);
  	glVertex3f(cx, cy, cz);
   	glVertex3f(bx, by, bz);
   	glVertex3f(fx, fy, fz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(1.0, 0.0, 0.0);
   	glVertex3f(gx, gy, gz);
   	glVertex3f(hx, hy, hz);
   	glVertex3f(dx, dy, dz);
  	glVertex3f(cx, cy, cz);
   	glEnd();
}

//----------------------------
// Display callback function
//----------------------------
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xCoordinate, yCoordinate, 0);
	glRotatef(10.0, 1.0, 0.0, 0.0);
	glRotatef(-10.0, 0.0, 1.0, 0.0);
	drawCube(0.0, 0.0, 0.0, 10.0);
	glFlush();
}

//-----------------
// Mouse callback
//-----------------
void mouse(int button, int state, int x, int y){
	//Print parameters
	cout << "mouse " << button << " " << state << " "
	     << x << " " << y << endl;

	//Calculate scale factors
	float scaleX = (MAX_X_VIEW - MIN_X_VIEW) /
		(float)(MAX_X_SCREEN - MIN_X_SCREEN);
	float scaleY = (MIN_Y_VIEW - MAX_Y_VIEW) /
		(float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

	//Handle mouse down
	if(state == GLUT_DOWN){
		xCoordinate = MIN_X_VIEW + (x - MIN_X_SCREEN) * scaleX;
		yCoordinate = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * scaleY;
	}
	else if(state == GLUT_UP){
		xCoordinate = MIN_X_VIEW + (x - MIN_X_SCREEN) * scaleX;
		yCoordinate = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * scaleY;

		//Erase the brick and redraw it in a new location
		glutPostRedisplay();
	}
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y){
	// Calculate scale factors
   	float x_scale = (MAX_X_VIEW - MIN_X_VIEW) /
      		(float)(MAX_X_SCREEN - MIN_X_SCREEN);
   	float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) /
      		(float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   	// Handle mouse motion
   	xCoordinate = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   	yCoordinate = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   	glutPostRedisplay();
}

//-----------------
// Main program
//-----------------
int main(int argc, char* argv[]){
	// Initialize display window
    glutInit(&argc, argv);

    //Giving window size in x and y
    glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
    glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);

    //Giving name to window
    glutCreateWindow("Angry Bricks");

	//Initialize callback functions
    glutDisplayFunc(display);
	init();
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

    glutMainLoop();
	return 0;
}
