//--------------------------------------------------
//  CSCE 4813 Programming Project #1
//  Author: Gregory Renteria
//  Code: ChartDisplay.cpp
//  Goal: OpenGL program that reads a sequence of
//        commands to display the corresponding chart
//        on the screen.
//---------------------------------------------------

#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

//--------------------------
// Initialization function
//--------------------------
void init(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
}


//------------------------
// Display method
//------------------------
void display(){
	//REMEMBER TO CHANGE THE NAME OF THE FILE BEFORE
	//COMPILING AND RUNNING THE PROGRAM
	//THE OPTIONS ARE (column.txt, point.txt, area.txt
	//or line.txt)

	glClear(GL_COLOR_BUFFER_BIT);

	//Reading in the corresponding data file
	ifstream data("point.txt");
	string graphCommand;
	data >> graphCommand;
    while(!data.eof()){
		if(graphCommand == "set_color"){
            float R,G,B;
            data >> R >> G >> B;
			//set_color command
            glColor3f(R,G,B);
        }
       	if(graphCommand == "draw_line"){
            float width,x1,y1,x2,y2;
            data >> width >> x1 >> y1 >> x2 >> y2;
			//draw_line command
            glLineWidth(width);
            glBegin(GL_LINES);
                glVertex3f(x1,y1,0);
                glVertex3f(x2,y2,0);
            glEnd();
            glFlush();
        }
		if(graphCommand == "draw_point"){
			float size,x1,y1;
			data >> size >> x1 >> y1;
			//draw_point command
			glPointSize(size);
			glBegin(GL_POINTS);
				glVertex2f(x1,y1);
			glEnd();
			glFlush();
		}
        if(graphCommand == "draw_polygon"){
			float count, x1, y1;
			//draw_polygon command
			glBegin(GL_POLYGON);
			data >> count;
			while(count > 0){
				data >> x1 >> y1;
				glVertex2f(x1,y1);
				count--;
			}
			glEnd();
			glFlush();
		}
		data >> graphCommand;
	}
    data.close();
}

//------------------------
// Main program
//------------------------
int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Chart");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}
