#ifndef EIGEN
#define EIGEN
#include <Eigen/Dense>
#include <Eigen/SVD>
#endif

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cfloat>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#include <string>
#include <string.h>
#include <stdlib.h>

#include "arm.h"

#define PI 3.14159265  // Should be used from mathlib

inline float sqr(float x) { return x*x; }
using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
	public:
    int w, h; // width and height
};

//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

float x_trans = 0.0f;
float y_trans = 0.0f;
float z_trans = 0.0f;
float theta = 0.0f;
float phi = 0.0f;

float bb_max = -1*FLT_MAX;
float z_max = -1*FLT_MAX;
Arm ARM;


//****************************************************
// Init function
//****************************************************
void initScene(){

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	
	GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0}; 
	GLfloat position[] = {500.0, 500.0, 100.0, 1.0};
	GLfloat twoside[] = {1.0};

	glFrontFace(GL_CW);

	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glViewport (0,0,viewport.w,viewport.h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	gluPerspective(2*atan(viewport.h/2),1,1,30);

}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
	viewport.w = w;
	viewport.h = h;

	glViewport (0,0,viewport.w,viewport.h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, w/h, 1, FLT_MAX);
	//gluPerspective(30, w/h, 10, 30);
}

//****************************************************
// calls functions to interpolate and sets pixel
//***************************************************
void drawSurface() {

	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	glBegin(GL_POLYGON);						// draw ground
	glVertex3f(-1000.0f,1000.0f,0.0f);
    glVertex3f(1000.0f,1000.0f,0.0f);
    glVertex3f(1000.0f,-1000.0f,0.0f);
    glVertex3f(-1000.0f,-1000.0f,0.0f);
	glEnd();

	GLUquadric* quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	int slices = 20;
	int stacks = 20;
	float radius = 1.0f;
	float base = 0.5f;
	float top = 0.5f;

	gluSphere(quad, radius, slices, stacks);
	gluCylinder(quad, base, top, LEN1, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	glTranslatef(0,0,LEN1);
	gluSphere(quad, radius, slices, stacks);
	gluCylinder(quad, base, top, LEN2, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	glTranslatef(0,0,LEN1+LEN2);
	gluSphere(quad, radius, slices, stacks);
	gluCylinder(quad, base, top, LEN3, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	glTranslatef(0,0,LEN1+LEN2+LEN3);
	gluSphere(quad, radius, slices, stacks);
	gluCylinder(quad, base, top, LEN4, slices, stacks);
	glPopMatrix();
	
	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	glTranslatef(0,0,LEN1+LEN2+LEN3+LEN4);
	gluSphere(quad, 0.75, slices, stacks);
	glPopMatrix();

}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

	glViewport (0,0,viewport.w,viewport.h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, viewport.w/viewport.h, 1, FLT_MAX);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// clear the color buffer
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
	// gluLookAt(0, 0, 2*bb_max*sqrt(3)+z_trans, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
	
	//start drawing
	drawSurface();

	glFlush();
	glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// 's': program toggles between flat/smooth shading
// 'w': program toggles between filled/wireframe
// +/- keys: zoom in/ out
// Exit when spacebar is pressed
//*****************************************************

void key_modes(unsigned char key, int x, int y){
	switch(key){

	 	// + zoom in
	 	case 43:

	 		z_trans -= 1;
	 		//cout << "+ PRESSED: " << z_trans << endl;
	 		glutPostRedisplay();
	 		break;

	 	// - zoom out
	 	case 45:
			z_trans += 1;
	 		//cout << "- PRESSED: " << z_trans << endl;
			glutPostRedisplay();
			break;

	 	// space bar
		case 32:
	 		exit(0);
	}
	glutPostRedisplay();
}

//****************************************************
// arrow keys: object will be rotated
// shift+arrow: object will be translated.
//*****************************************************
void trans_modes(int key, int x, int y){
	
	int mod = glutGetModifiers();

	if (mod != GLUT_ACTIVE_SHIFT) {
 		switch(key){

 			// rotate reverse
 			case GLUT_KEY_DOWN:
 				phi += 1;
				if(phi >= 360) {
					phi -= 360;
				} 
				glutPostRedisplay();
				break;

 			// rotate forward
 			case GLUT_KEY_UP:
 				phi -= 1;
				if(phi < 0) {
					phi += 360;
				} 
				glutPostRedisplay();
				break;

 			// rotate left
 			case GLUT_KEY_LEFT:
				theta -= 1;
				if(theta < 0) {
					theta += 360;
				} 
				glutPostRedisplay();
				break;

 			// rotate right
 			case GLUT_KEY_RIGHT:
				theta += 1;
				if(theta >= 360) {
					theta -= 360;
				} 
				glutPostRedisplay();
				break;
 		}	

 	//implement some other functionality (besides translations) for shift+arrow keys
 	} else {
 		switch(key){
 			// translate left
 			case GLUT_KEY_LEFT:
 				x_trans -= 1;
		 		break;

 	 		// translate right
 	 		case GLUT_KEY_RIGHT:
		 		x_trans += 1;
		 		break;

		 	// translate up
		 	case GLUT_KEY_UP:
		 		y_trans += 1;
		 		break;

		 	// translate down
		 	case GLUT_KEY_DOWN:
		 		y_trans -= 1;
		 		break;
 	 	}
 	}
	glutPostRedisplay();
}

//****************************************************
// the usual stuff, nothing exciting here
//**************************************************** 
int main(int argc, char *argv[]) {

	//initialize arm
	Eigen::VectorXf angles(12);
	angles << 0, 0, 0,
			  0, 0, 0,
			  0, 0, 0,
			  0, 0, 0; 

	ARM = Arm(angles);
	initRotationMatrices();

	//This initializes glut
	glutInit(&argc, argv);

	//This tells glut to use a double-buffered window with red, green, and blue channels 
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	// Initalize theviewport size
	viewport.w = 700;
	viewport.h = 700;

	//The size and position of the window
	glutInitWindowSize(viewport.w, viewport.h);
	glutInitWindowPosition(0,0);
	glutCreateWindow("as4");

	initScene();							// quick function to set up scene

	glutDisplayFunc(myDisplay);				// function to run when its time to draw something
	glutReshapeFunc(myReshape);				// function to run when the window gets resized

	glutKeyboardFunc(key_modes);			// exit when spacebar is pressed, 's', 'w', zoom, etc.
	glutSpecialFunc(trans_modes);			// for transformations using keys

	glutMainLoop();							// infinite loop that will keep drawing and resizing
	return 0;
}
