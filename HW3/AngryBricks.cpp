//-----------------------------------------------------
// CSCE 4813 Programming Project #3
// Author: Gregory Renteria
// Code: AngryBricks.cpp
// Goal: create an OpenGL program that displays
//       a colorful brick using the display callback,
//		 drag it across the screen using the mouse and 
//       motion callback, and simulating and displaying 
//		 motion using the idle and timer callback.
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
#define SLEEP_TIME 80		//step-size

// Global variables
bool moving = false;			//boolean to check the if the cube is moving 
float xCoordinate, yCoordinate, 
	  x1Location, y1Location,	//to record (x1, y1) location of the mouse when it clicks
	  x2Location, y2Location;	//to record (x2, y2) location of the mouse when it releases
float rotationAngle = 10;		//rotation angle by default
float xCoordVector = 1; 		//x-coordinate vector used in the glRotate function
float yCoordVector = 1;			//y-coordinate vector used in the glRotate function
float zCoordVector = 0;			//z-coordinate vector used in the glRotate function
int counter;					//counter for the timer callback to check how fast it is
vector<float> velocityVector(1);	//vector to store the values of the x and y coordinate speed

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
   	glColor3f(1.0, 0.0, 0.0);
   	glVertex3f(ax, ay, az);
   	glVertex3f(bx, by, bz);
   	glVertex3f(cx, cy, cz);
   	glVertex3f(dx, dy, dz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(0.0, 1.0, 0.0);
   	glVertex3f(ax, ay, az);
   	glVertex3f(dx, dy, dz);
   	glVertex3f(hx, hy, hz);
   	glVertex3f(ex, ey, ez);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(0.0, 0.0, 1.0);
   	glVertex3f(ax, ay, az);
   	glVertex3f(ex, ey, ez);
   	glVertex3f(fx, fy, fz);
   	glVertex3f(bx, by, bz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(1.0, 1.0, 0.0);
   	glVertex3f(gx, gy, gz);
   	glVertex3f(fx, fy, fz);
   	glVertex3f(ex, ey, ez);
   	glVertex3f(hx, hy, hz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(1.0, 0.0, 1.0);
   	glVertex3f(gx, gy, gz);
  	glVertex3f(cx, cy, cz);
   	glVertex3f(bx, by, bz);
   	glVertex3f(fx, fy, fz);
   	glEnd();

   	glBegin(GL_POLYGON);
   	glColor3f(0.0, 1.0, 1.0);
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
	glRotatef(rotationAngle, xCoordVector, yCoordVector - 1.0, zCoordVector);
	glRotatef(-rotationAngle, xCoordVector - 1.0, yCoordVector, zCoordVector);
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
		
		moving = false;	//sets movement to false when clicks
		
		x1Location = xCoordinate;
		y1Location = yCoordinate;
	}
	else if(state == GLUT_UP){
		xCoordinate = MIN_X_VIEW + (x - MIN_X_SCREEN) * scaleX;
		yCoordinate = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * scaleY;
		
		moving = true;	//sets movement to true when releases
		
		x2Location = xCoordinate;
		y2Location = yCoordinate;

		//Erase the brick and redraw it in a new location
		glutPostRedisplay();
	}
		
	velocityVector[0]= x1Location - x2Location;	//subtracting the position in x1 and x2 for the mouse click and release
	velocityVector[1]= y1Location - y2Location;	//subtracting the position in y1 and y2 for the mouse click and release
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

//------------------
// Timer callback
//------------------
void timer(int value){
	//Print parameters
	cout << "timer " << counter++ << endl;
	
	//Redraw objects
	glutPostRedisplay();
	glutTimerFunc(SLEEP_TIME, timer, 0);
	
	//small if statement to start rotating the cube right after motion
	if(!moving){
		return;
	}
	
	//Updating the position of the cube after being launched
	xCoordinate += velocityVector[0];
	yCoordinate += velocityVector[1];
	
	//Check if the cube exceeds the edges of the window
	if(xCoordinate > MAX_X_VIEW - 5){
		xCoordinate = MAX_X_VIEW - 5;
		velocityVector[0] = -velocityVector[0];
	}
	else if(xCoordinate < MIN_X_VIEW + 5){
		xCoordinate = MIN_X_VIEW + 5;
		velocityVector[0] = -velocityVector[0];		
	}
	
	if(yCoordinate > MAX_Y_VIEW -5){
		yCoordinate = MAX_Y_VIEW - 5;
		velocityVector[1] = -velocityVector[1];
	}
	else if(yCoordinate < MIN_Y_VIEW + 5){
		yCoordinate = MIN_Y_VIEW + 5;
		velocityVector[1] = -velocityVector[1];
		moving = false;
	}
	
	//Update the rotation of the cube
	rotationAngle += 2.0;
	 
	//setting up the gravity in the cube with the y-velocity vector
	velocityVector[1] -= 15.0;
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
	glutTimerFunc(0, timer,0);

    glutMainLoop();
	return 0;
}
