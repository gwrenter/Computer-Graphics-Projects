//------------------------------------------------------
// CSCE 4813 Programming Project #5
// Program: project5.cpp
// Purpose: OpenGL program that uses texture mapping
// to display images on geometric objects.
// Author:  Gregory Renteria
//------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

// Global variables
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
#define SIZE 512
#define SLEEP_TIME 80
float R[SIZE][SIZE];
float G[SIZE][SIZE];
float B[SIZE][SIZE];
float xCoordVector = 1;
float yCoordVector = 1;
float zCoordVector = 0;
int xdim = 512;
int ydim = 512;
int counter;

// Sequence of arrays
const int COUNT = 3;
float Px[COUNT];
float Py[COUNT];
float Pz[COUNT];
float Vx[COUNT];
float Vy[COUNT];
float Vz[COUNT];
float Ax[COUNT];
float Ay[COUNT];
float Az[COUNT];
float Radius[COUNT];
char filename[COUNT][100];
unsigned char *textureCube[COUNT];

//---------------------------------------
// Initialize texture image
//---------------------------------------
void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim)
{
   // Read jpg image
   im_color image;
   image.ReadJpg(name);

   xdim = 1; while (xdim < image.R.Xdim) xdim*=2;
   ydim = 1; while (ydim < image.R.Ydim) ydim*=2;
   if (xdim - image.R.Xdim > image.R.Xdim - xdim/2) xdim /=2;
   if (ydim - image.R.Ydim > image.R.Ydim - ydim/2) ydim /=2;
   image.Interpolate(xdim, ydim);
   xdim = image.R.Xdim;
   ydim = image.R.Ydim;

   // Copy image into texture array
   texture = (unsigned char *)malloc((unsigned int)(xdim*ydim*3));
   int index = 0;
   for (int y = 0; y < ydim; y++)
      for (int x = 0; x < xdim; x++)
      {
         texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
      }
}


//---------------------------------------
// Read color and depth images
//---------------------------------------
void read_images(string color_name, im_color & color_image){
   // ----------------
   // Read color image
   // ----------------
   cout << "input file = " << color_name << endl;
   color_image.ReadJpg(color_name);

   // Create output file name
   int pos = color_name.find(".jpg");
   color_name.erase(pos, 4);
   color_name += ".txt";
   cout << "output file = " << color_name << endl;

   // Open output file
   ofstream dout;
   dout.open(color_name);

   // Get image dimensions
   int xdim = color_image.R.Xdim;
   int ydim = color_image.R.Ydim;
   cout << "xdim = " << xdim << endl;
   cout << "ydim = " << ydim << endl;

   // Print color values
   for (int y = 0; y < ydim; y++)
      for (int x = 0; x < xdim; x++)
      {
         dout << color_image.R.Data2D[y][x] << " "
              << color_image.G.Data2D[y][x] << " "
              << color_image.B.Data2D[y][x] << endl;
        R[y][x] = float(color_image.R.Data2D[y][x]) / 255;
        G[y][x] = float(color_image.G.Data2D[y][x]) / 255;
        B[y][x] = float(color_image.B.Data2D[y][x]) / 255;
      }
   // Close output file
   dout << endl;
   dout.close();
}


//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init(){
   // Init view
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, MIN_Y_VIEW,
           MAX_Y_VIEW, MIN_Z_VIEW, MAX_Z_VIEW);

   // Init global data structure
   srand(time(NULL));

   const int VALUE = 75;
   for(int i = 0; i < COUNT; i++){
        Px[i] = (rand() % 100) + MIN_X_VIEW;
        Py[i] = (rand() % 50) + VALUE;
        Pz[i] = (rand() % 100) + MIN_Z_VIEW;
        Vx[i] = 0;
        Vy[i] = (rand() % 100) + MIN_Y_VIEW;
        Vz[i] = 0;
        Ax[i] = rand() % VALUE;
        Ay[i] = rand() % VALUE;
        Az[i] = rand() % VALUE;
        Radius[i] = rand() % VALUE;

        int switchImage = rand() % 2;
        if(switchImage == 1)
           sprintf(filename[i], "cats_dogs/dog%ld.jpg", random() % 10);
        else
           sprintf(filename[i], "cats_dogs/cat%ld.jpg", random() % 10);

        init_texture(filename[i], textureCube[i], xdim, ydim);
   }

   for(int i = 0; i < COUNT; i++){
      glEnable(GL_TEXTURE_2D);
      glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   }
}

//---------------------------------------
// Function to draw a cube
//---------------------------------------
void drawCube(float midx, float midy, float midz, float size, int index){
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0,
                GL_RGB, GL_UNSIGNED_BYTE, textureCube[index]);

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
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(ex, ey, ez);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(fx, fy, fz);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(cx, cy, cz);
   glEnd();
}


//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   for(int i = 0; i < COUNT; i++){
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(Px[i], Py[i], Pz[i]);
      glRotatef(Ax[i], xCoordVector, yCoordVector - 1.0, zCoordVector);
      glRotatef(-Ay[i], xCoordVector - 1.0, yCoordVector, zCoordVector);
      glRotatef(Az[i], xCoordVector, yCoordVector, zCoordVector - 1.0);
      drawCube(0.0, 0.0, 0.0, 16.0, i);

      // Draw objects
      glFlush();
   }
}


//-----------------------
//  Timer callback
//-----------------------
void timer(int value){
   //Print parameters
   cout << "timer" << counter++ << endl;

   glutTimerFunc(SLEEP_TIME, timer, 0);

   for(int i = 0; i < COUNT; i++){
      //Redraw objects
      glutPostRedisplay();

      Py[i] -= 0.9;

      //Update the rotation of the cube
      Ax[i] += 0.5;
      Ay[i] += 0.5;
      Az[i] += 0.5;
   }
}


//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   im_color color;
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("CatsDogs");
   glutDisplayFunc(display);
   init();
   glutTimerFunc(0, timer, 0);

   glutMainLoop();
   return 0;
}
