//---------------------------------------
// CSCE 4813 Programming Project #4
// Program: image.cpp
// Purpose: Read and print pixels values.
// Author:  Gregory Renteria
//---------------------------------------
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
using namespace std;

// Material properties
float Ka = 0.5;
float Kd = 0.5;
float Ks = 0.5;
float Kp = 0.5;

//Surface class
#define SIZE 500
float Px[SIZE][SIZE];
float Py[SIZE][SIZE];
float Pz[SIZE][SIZE];
float Nx[SIZE][SIZE];
float Ny[SIZE][SIZE];
float Nz[SIZE][SIZE];
float R[SIZE][SIZE];
float G[SIZE][SIZE];
float B[SIZE][SIZE];

// Transformation variables
#define ROTATE 1
#define TRANSLATE 2
int xangle = 0;
int yangle = 0;
int zangle = 0;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;
int wireframe_mode = 1;
int color_mode = 2;
int phong_mode = 3;
int mode2 = wireframe_mode;

#define STEP 0.1


//---------------------------------------
// Read color and depth images
//---------------------------------------
void read_images(string color_name, im_color & color_image,
                 string depth_name, im_float & depth_image){
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
   dout.close();

   // ----------------
   // Read depth image
   // ----------------
   cout << "input file = " << depth_name << endl;
   depth_image.ReadJpg(depth_name);

   // Create output file name
   pos = depth_name.find(".jpg");
   depth_name.erase(pos, 4);
   depth_name += ".txt";
   cout << "output file = " << depth_name << endl;

   // Open output file
   dout.open(depth_name);

   // Get image dimensions
   xdim = depth_image.Xdim;
   ydim = depth_image.Ydim;
   cout << "xdim = " << xdim << endl;
   cout << "ydim = " << ydim << endl;

   // Print depth values
   for (int y = 0; y < ydim; y++)
      for (int x = 0; x < xdim; x++){
        dout << depth_image.Data2D[y][x] << " ";
        Pz[y][x] = float(depth_image.Data2D[y][x]) / 2000;
      }

   // Close output file
   dout << endl;
   dout.close();
}

//---------------------------------------
// Initialize material properties
//---------------------------------------
void init_material(float Ka, float Kd, float Ks, float Kp,
                   float Mr, float Mg, float Mb)
{
   // Material variables
   float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
   float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
   float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };

   // Initialize material
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);
}

//---------------------------------------
// Initialize light source
//---------------------------------------
void init_light(int light_source, float Lx, float Ly, float Lz,
                float Lr, float Lg, float Lb)
{
   // Light variables
   float light_position[] = { Lx, Ly, Lz, 0.0 };
   float light_color[] = { Lr, Lg, Lb, 1.0 };

   // Initialize light source
   glEnable(GL_LIGHTING);
   glEnable(light_source);
   glLightfv(light_source, GL_POSITION, light_position);
   glLightfv(light_source, GL_AMBIENT, light_color);
   glLightfv(light_source, GL_DIFFUSE, light_color);
   glLightfv(light_source, GL_SPECULAR, light_color);
   glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
   glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
   glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}


//---------------------------------------
// Initialize surface points
//---------------------------------------
void init_surface(float Xmin, float Xmax, float Ymin, float Ymax)
{
   // Initialize (x,y,z) coordinates of surface
   for (int i = 0; i < SIZE; i++)
   for (int j = 0; j < SIZE; j++)
   {
      Px[i][j] = Xmin + i * (Xmax - Xmin) / SIZE;
      Py[i][j] = Ymin + j * (Ymax - Ymin) / SIZE;
      Pz[i][j] = Pz[i][j];
      R[i][j] = R[i][j];
      G[i][j] = G[i][j];
      B[i][j] = B[i][j];
    }
}

//---------------------------------------
// Initialize surface normals
//---------------------------------------
void init_normals()
{
   // Initialize surface normals
   for (int i=0; i<SIZE-1; i++)
   for (int j=0; j<SIZE-1; j++)
   {
      // Get tangents S and T
      float Sx = Px[i+1][j] - Px[i][j];
      float Sy = Py[i+1][j] - Py[i][j];
      float Sz = Pz[i+1][j] - Pz[i][j];
      float Tx = Px[i][j+1] - Px[i][j];
      float Ty = Py[i][j+1] - Py[i][j];
      float Tz = Pz[i][j+1] - Pz[i][j];

      // Calculate N = S cross T
      Nx[i][j] = Sy*Tz - Sz*Ty;
      Ny[i][j] = Sz*Tx - Sx*Tz;
      Nz[i][j] = Sx*Ty - Sy*Tx;

      // Make unit vector
      float length = sqrt(Nx[i][j]*Nx[i][j]
                   + Ny[i][j]*Ny[i][j]
                   + Nz[i][j]*Nz[i][j]);
      if (length > 0)
      {
         Nx[i][j] /= length;
         Ny[i][j] /= length;
         Nz[i][j] /= length;
      }
   }

   // Handle last row and column
   for (int i=0; i<SIZE; i++)
   {
      Nx[i][SIZE-1] = Nx[i][SIZE-2];
      Ny[i][SIZE-1] = Ny[i][SIZE-2];
      Nz[i][SIZE-1] = Nz[i][SIZE-2];
      Nx[SIZE-1][i] = Nx[SIZE-2][i];
      Ny[SIZE-1][i] = Ny[SIZE-2][i];
      Nz[SIZE-1][i] = Nz[SIZE-2][i];
   }
}


//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init(){
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.3, 1.3, -1.3, 1.3, -1.3, 1.3);
   glEnable(GL_DEPTH_TEST);

   // Initialize smooth shading
   glShadeModel(GL_SMOOTH);
   glEnable(GL_NORMALIZE);
   init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
   init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
   init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);

   // Initialize surface
   init_surface(-1.0, 1.0, -1.0, 1.0);
   init_normals();
}

//---------------------------------
// Wireframe Display function
//---------------------------------
void wireframe_display(){
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_LIGHTING);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Draw the surface
   for (int i = 0; i < SIZE-1; i++)
      for (int j = 0; j < SIZE-1; j++)
      {
	 glBegin(GL_LINE_LOOP);
         glColor3f(1.0,1.0,1.0);
	 glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
	 glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
         glColor3f(1.0,1.0,1.0);
	 glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
	 glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
         glColor3f(1.0,1.0,1.0);
	 glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
	 glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
         glColor3f(1.0,1.0,1.0);
	 glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]);
	 glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
         glColor3f(1.0,1.0,1.0);
	 glEnd();
      }
   glFlush();
}

//-------------------------
// Color Display function
//-------------------------
void color_display(){
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);
   glDisable(GL_LIGHTING);

   // Draw the surface
   for (int i = 0; i < SIZE-1; i++)
      for (int j = 0; j < SIZE-1; j++){
         glBegin(GL_POLYGON);
         glColor3f(R[i][j], G[i][j], B[i][j]);
         glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
         glColor3f(R[i + 1][j], G[i + 1][j], B[i + 1][j]);
         glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
         glColor3f(R[i + 1][j + 1], G[i + 1][j + 1], B[i + 1][j + 1]);
         glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
         glColor3f(R[i][j + 1], G[i][j + 1], B[i][j + 1]);
         glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
         glEnd();
      }
   glFlush();
}

//--------------------------
// Phong Display function
//--------------------------
void phong_display(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_LIGHTING);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Initialize material properties
   init_material(Ka, Kd, Ks, 100 * Kp, 0.942, 0.45, 0.40);

   // Draw the surface
   for (int i = 0; i < SIZE-1; i++)
      for (int j = 0; j < SIZE-1; j++){
         glBegin(GL_POLYGON);
         glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
         glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
         glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
         glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
         glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
         glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
         glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]);
         glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
         glEnd();
      }
   glFlush();

}

//------------------------------
// Display callback for OpenGL
//------------------------------
void display(){
    if(mode2 == wireframe_mode)
    	wireframe_display();
    if(mode2 == color_mode)
    	color_display();
    if(mode2 == phong_mode)
    	phong_display();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Initialize random surface
   if (key == 'i')
   {
      init_surface(-1.0, 1.0, -1.0, 1.0);
      init_normals();
   }

   // Determine if we are in ROTATE or TRANSLATE mode
   if ((key == 'r') || (key == 'R'))
   {
      printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
      mode = ROTATE;
   }
   else if ((key == 't') || (key == 'T'))
   {
      printf
	 ("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
      mode = TRANSLATE;
   }

   // Handle ROTATE
   if (mode == ROTATE)
   {
      if (key == 'x')
	 xangle -= 5;
      else if (key == 'y')
	 yangle -= 5;
      else if (key == 'z')
	 zangle -= 5;
      else if (key == 'X')
	 xangle += 5;
      else if (key == 'Y')
	 yangle += 5;
      else if (key == 'Z')
	 zangle += 5;
   }

   // Handle TRANSLATE
   if (mode == TRANSLATE)
   {
      if (key == 'x')
	 xpos -= 5;
      else if (key == 'y')
	 ypos -= 5;
      else if (key == 'z')
	 zpos -= 5;
      else if (key == 'X')
	 xpos += 5;
      else if (key == 'Y')
	 ypos += 5;
      else if (key == 'Z')
	 zpos += 5;
   }

   // Handle material properties
   if (key == 'a')
      Ka -= STEP;
   if (key == 'd')
      Kd -= STEP;
   if (key == 's')
      Ks -= STEP;
   if (key == 'p')
      Kp -= STEP;
   if (key == 'A')
      Ka += STEP;
   if (key == 'D')
      Kd += STEP;
   if (key == 'S')
      Ks += STEP;
   if (key == 'P')
      Kp += STEP;
   if (Ka < 0)
      Ka = 0;
   if (Kd < 0)
      Kd = 0;
   if (Ks < 0)
      Ks = 0;
   if (Kp < STEP)
      Kp = STEP;

   //Switching display functions
   if(key == '1')
     mode2 = wireframe_mode;
   if(key == '2')
     mode2 = color_mode;
   if(key == '3')
     mode2 = phong_mode;

   glutPostRedisplay();
}


//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   im_color color;
   im_float depth;
   read_images("penny-image.jpg", color,
               "penny-depth.jpg", depth);
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Homework4");
   init();
    printf("Keyboard commands:\n");
    printf("   'x' - rotate x-axis -5 degrees\n");
    printf("   'X' - rotate x-axis +5 degrees\n");
    printf("   'y' - rotate y-axis -5 degrees\n");
    printf("   'Y' - rotate y-axis +5 degrees\n");
    printf("   'z' - rotate z-axis -5 degrees\n");
    printf("   'Z' - rotate z-axis +5 degrees\n");
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}

