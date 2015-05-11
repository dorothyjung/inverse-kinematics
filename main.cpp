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

#define LEMNISCATE 1
#define FLOWER 2
#define LIMACON 3

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
int t = 1;
bool animate = 0;
MatrixXf currJ;
Vector3f nextpt;

int shape = LEMNISCATE;



//****************************************************
// Init function
//****************************************************
void initScene(){

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LINE_SMOOTH);
	glShadeModel(GL_SMOOTH);
	
	GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0}; 
	GLfloat position[] = {0.0, 0.0, -100.0, 1.0};
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
// draw joints using spheres
//***************************************************
void drawJoints() {

	//create quadric for use in spheres, cylinders
	GLUquadric* quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	//define parameters for spheres, cylinders
	int slices = 20;
	int stacks = 20;
	float radius = 1.0f;
	float base = 0.5f;
	float top = 0.5f;

	int i;
	for (i=0; i<ARM.endpt.size(); i++) {

		glPushMatrix();
		glLoadIdentity();							// make sure transformation is "zero'd"
		glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
		glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
		glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

		glTranslatef(ARM.endpt[i][0], ARM.endpt[i][1], ARM.endpt[i][2]);

		if (i == 3) {
			radius = 0.75f;
		}

		gluSphere(quad, radius, slices, stacks);
		glPopMatrix();

	}

}

//****************************************************
// draw arm segments using cylinders
//***************************************************
void drawArmSegments() {

	//create quadric for use in spheres, cylinders
	GLUquadric* quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	//define parameters for spheres, cylinders
	int slices = 20;
	int stacks = 20;
	float radius = 1.0f;
	float base = 0.5f;
	float top = 0.5f;

	Vector3f orig(0.0, 0.0, 1.0);				// default orientation in opengl
	Vector3f bot(0.0, 0.0, 0.0);				// base of arm at origin
	Vector3f prev;
	Vector3f seg;								// used to find rotate axis, diff btwn endptA - endptB
	Vector3f axis_of_rotation;
	float deg_rotate = 0.0f;					// degrees to rotate by (in degrees)

	int i;
	for (i=0; i<ARM.endpt.size(); i++) {

		glPushMatrix();
		glLoadIdentity();							// make sure transformation is "zero'd"
		glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
		glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
		glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

		if (i == 0) {
			prev = bot;
		}

		seg = (ARM.endpt[i] - prev);
		axis_of_rotation = orig.cross(seg);
		deg_rotate = (180/PI)*acos(orig.dot(seg) / seg.norm());
		glTranslatef(prev[0], prev[1], prev[2]);
		glRotatef(deg_rotate, axis_of_rotation[0], axis_of_rotation[1], axis_of_rotation[2]);
		gluCylinder(quad, base, top, seg.norm(), slices, stacks);
		glPopMatrix();

		prev = ARM.endpt[i];
	}
}

//****************************************************
// calls functions to draw current arm configuration
//***************************************************
void drawArm() {

	//create quadric for use in spheres, cylinders
	GLUquadric* quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	//define parameters for spheres, cylinders
	int slices = 20;
	int stacks = 20;
	float radius = 1.0f;
	float base = 0.5f;
	float top = 0.5f;

	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	// glBegin(GL_POLYGON);						// draw ground
	// glVertex3f(-1000.0f,1000.0f,0.0f);
 	// glVertex3f(1000.0f,1000.0f,0.0f);
 	// glVertex3f(1000.0f,-1000.0f,0.0f);
 	// glVertex3f(-1000.0f,-1000.0f,0.0f);
	// glEnd();

	int MAX = 100;
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	
	switch(shape) {
		case LEMNISCATE:
			// Loop 1: Lemniscate
			for(int i = 0; i < MAX; i++){
				float step = (i*2*PI)/MAX;
				float r = 5*(2 + cos(2*(step))); 
				glVertex3f(r*cos(step),r*sin(step),10+4*cos(step));
			}
			break;

		case FLOWER:
			// Loop 2: Rose
			for(int i = 0; i < MAX; i++){
				float step = (i*2*PI)/MAX;
				float r = 13*sin(5*(step)); 
				glVertex3f(r*cos(step),r*sin(step),8);
			}
			break;

		case LIMACON:
			// Loop 3: Limacon
			for(int i = 1; i < MAX; i++){
				float step = (i*2*PI)/MAX;
				float r = 4 + 8*cos(step); 
				float z = 6 - 6*cos(3*step);
				glVertex3f(r*cos(step),r*sin(step),z);	
			}
			break;
	}

	glEnd();
	
	gluSphere(quad, radius, slices, stacks);	// draw base
	glPopMatrix();

	drawArmSegments();							// draw arm segments
	drawJoints();								// draw joints

}

//****************************************************
// function that recalculates Jacobian
//***************************************************
void drawSurface() {

	//http://cims.nyu.edu/~kiryl/Precalculus/Section_8.2-Graphs%20of%20Polar%20Equations/Graphs%20of%20Polar%20Equations.pdf

	float step, r, x, y, z;

	switch(shape) {
		case LEMNISCATE:
			// Loop 1: Lemniscate
			step = 0.005*t;
			r = 5*(2 + cos(2*(step)));
			x = r*cos(step);
			y = r*sin(step);
			z = 10 + 4*cos(step);
			break;

		case FLOWER:
			// Loop 2: Rose
			step = 0.005*t;
			r = 13*sin(5*(step)); 
			x = r*cos(step);
			y = r*sin(step);
			z = 8.0;
			break;

		case LIMACON:
			// Loop 3: Limacon
			step = 0.005*t;
			r = 4 + 8*cos(step); 
			x = r*cos(step);
			y = r*sin(step);
			z = 6 - 6*cos(3*step);
			break;
	}

	nextpt << x, y, z;
	if(nextpt.norm() > 14.0){
		nextpt = (13.95/nextpt.norm())*nextpt;
	}

	currJ = ARM.computeJacobian();
	//while((ARM.endpt[3]-nextpt).norm() > EPS){
		ARM.updateArm(currJ,nextpt);
	//}
	
	drawArm();

	t++;

}

//****************************************************
// function called by openGL, does actual drawing
//***************************************************
void myDisplay() {

	glViewport (0,0,viewport.w,viewport.h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, viewport.w/viewport.h, 1, FLT_MAX);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// clear the color buffer
	gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);
	// gluLookAt(0, 0, 2*bb_max*sqrt(3)+z_trans, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations

	//start drawing
	if (animate) {
		drawSurface();
	} else {
		drawArm();
	}

	glutPostRedisplay();

	glFlush();
	glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// 'n': starts and stops animation
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

		// 'n' to start animation
		case 110:
			if (animate) {
				animate = 0;
			} else {
				animate = 1;
			}
			break;

		// 'j' is pressed 106 lemniscate
		case 106:
			shape = LEMNISCATE;
			break;

		// 'k' is pressed 107 flower
		case 107:
			shape = FLOWER;
			break;

		// 'l' is pressed 108 limacon
		case 108:
			shape = LIMACON;
			break;

		// 'm' to step one frame
		case 109:
			// cout << "---------------------------------------------------------------------" << endl;
			// cout << "THETA IN MAIN: " << ARM.theta.transpose()*(180.0/PI) << endl;
			currJ = ARM.computeJacobian();
			nextpt << 0.0, 0.3, 13.6;
			
			// cout << "Next Goal Point: " << nextpt.transpose() << endl;

			ARM.updateArm(currJ,nextpt);

			// while(1){
			// 	if(ARM.updateArm(currJ,nextpt)){
			// 		break;
			// 	}
			// }
			// cout << "THETA IN MAIN after update: " << ARM.theta.transpose()*(180.0/PI) << endl;
			drawArm();
			t++;
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
	angles << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 

	ARM = Arm(angles);
	initRotationMatrices();
	// initPath();

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
