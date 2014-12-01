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


//****************************************************
// Init function
//****************************************************
void initScene(){

	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_LIGHTING);
	// glEnable(GL_LIGHT0);
	// glEnable(GL_NORMALIZE);
	// glShadeModel(GL_SMOOTH);
	// glDisable(GL_CULL_FACE);
	
	// GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};
	// GLfloat diffuse[] = {1.0, 0.0, 1.0, 1.0};
	// GLfloat specular[] = {1.0, 1.0, 1.0, 1.0}; 
	// GLfloat position[] = {0.0, 0.0, 100.0, 1.0};
	// GLfloat twoside[] = {1.0};

	// glFrontFace(GL_CW);

	// glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, ambient);
	// glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	// glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	// glLightfv(GL_LIGHT0, GL_POSITION, position);

	// // glViewport (0,0,viewport.w,viewport.h);
	// // glMatrixMode(GL_PROJECTION);
	// // glLoadIdentity();
 // 	// gluPerspective(2*atan(viewport.h/2),1,1,30);

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
void drawSurfaceUniform() { 
	
	vector<LocalGeo*> spatch;

	for (int i = 0; i < polygon.size(); i++){
		// cout << i << endl;
		spatch = *(polygon[i]);

		for (int j = 0; j < spatch.size(); j+=4){

		//cout << "spatch size" << endl;
		//cout << spatch.size() << endl; 
			// cout << j << endl;

			if(isFilledMode){
			    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				glBegin(GL_POLYGON);

			} else {

				if (isHiddenLine) {
					glBegin(GL_LINE_LOOP);

				} else {
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					glBegin(GL_POLYGON);
				}
			}

			// cout << "I'm drawing!" << endl;

			Vect point = (*(spatch[j])).pos;
			Vect normal = (*(spatch[j])).norm;

			// cout << point.x << " " << point.y << " " << point.z << endl;
			// cout << normal.x << " " << normal.y << " " << normal.z << endl;

			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);

			point = (*(spatch[j+1])).pos;
			normal = (*(spatch[j+1])).norm;
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);

			point = (*(spatch[j+2])).pos;
			normal = (*(spatch[j+2])).norm;
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);

			point = (*(spatch[j+3])).pos;
			normal = (*(spatch[j+3])).norm;
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);
			glEnd();
		}
	}
	//cout << "polygon size" << endl;
	//cout << polygon.size() << endl;


	// 	cout << "spatch size" << endl;
	// cout << spatch.size() << endl;
	glPopMatrix();

}

//****************************************************
// calls functions to interpolate and sets pixel
//***************************************************
void drawSurfaceAdaptive() { 
	
	vector<LocalGeo*> spatch;

	for (int i = 0; i < polygon.size(); i++){
		// cout << i << endl;
		spatch = *(polygon[i]);

		for (int j = 0; j < spatch.size(); j+=3){

		//cout << "spatch size" << endl;
		//cout << spatch.size() << endl; 
			// cout << j << endl;


			if(isFilledMode){
			    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				glBegin(GL_POLYGON);

			} else {

				if (isHiddenLine) {
					glBegin(GL_LINE_LOOP);

				} else {
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					glBegin(GL_POLYGON);
				}
			}
			// cout << "I'm drawing!" << endl;

			Vect point = (*(spatch[j])).pos;
			Vect normal = (*(spatch[j])).norm;

			// cout << point.x << " " << point.y << " " << point.z << endl;
			// cout << normal.x << " " << normal.y << " " << normal.z << endl;

			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);

			point = (*(spatch[j+1])).pos;
			normal = (*(spatch[j+1])).norm;
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);

			point = (*(spatch[j+2])).pos;
			normal = (*(spatch[j+2])).norm;
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(point.x, point.y, point.z);

			glEnd();
		}
	}
	//cout << "polygon size" << endl;
	//cout << polygon.size() << endl;


	// 	cout << "spatch size" << endl;
	// cout << spatch.size() << endl;
	glPopMatrix();

}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

	glViewport (0,0,viewport.w,viewport.h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(30, viewport.w/viewport.h, 5, z_trans+30);
	gluPerspective(60, viewport.w/viewport.h, 1, FLT_MAX);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// clear the color buffer
	gluLookAt(0, 0, 2*bb_max*sqrt(3)+z_trans, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
	glPushMatrix();
	glLoadIdentity();							// make sure transformation is "zero'd"

	//glTranslatef(0, 0, z_trans);				// handle zoom in/out
	glTranslatef(x_trans, y_trans, 0);			// handle translations from arrow keys
	glRotatef(phi, 1, 0, 0);					// handle rotations about x axis
	glRotatef(theta, 0, 1, 0);					// handle azimuthal rotations

	int i;
	polygon.clear();
	
	if (isAdaptive) {
		for(i = 0; i < patches.size(); i++){
			vector<LocalGeo*>* spatch = new vector<LocalGeo*>();
			subdividePatchAdaptive(*(patches[i]), subdiv_param, spatch);
			polygon.push_back(spatch);
		}

		//start drawing adaptive
		drawSurfaceAdaptive();

	} else {
	 	for(i = 0; i < patches.size(); i++){
			vector<LocalGeo*>* spatch = new vector<LocalGeo*>();
			subdividePatchUniform(*(patches[i]), subdiv_param, spatch);
			polygon.push_back(spatch);
		}

		//start drawing uniform
		drawSurfaceUniform();
	}

	glFlush();
	glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// toggle between flat and smooth modes
//***************************************************
void toggleFlatSmooth() {
	if(isFlatShade){
		isFlatShade = false;
		glShadeModel(GL_FLAT);
	} else {
		isFlatShade = true;
		glShadeModel(GL_SMOOTH);
	}
}

//****************************************************
// toggle between filled and wireframe modes
//***************************************************
void toggleFilledWire() {
	if (isFilledMode) {
	 	isFilledMode = false;
	} else {
	 	isFilledMode = true;
	}
	isHiddenLine = false;
}

//****************************************************
// toggle between filled and hidden-line mode
//***************************************************
void toggleHiddenLine() {
	if (isFilledMode) {
	 	isFilledMode = false;
	 	isHiddenLine = true;
	} else {
	 	isFilledMode = true;
	 	isHiddenLine = false;
	}
}

//****************************************************
// 's': program toggles between flat/smooth shading
// 'w': program toggles between filled/wireframe
// +/- keys: zoom in/ out
// Exit when spacebar is pressed
//*****************************************************

void key_modes(unsigned char key, int x, int y){
	switch(key){
	 	// 's' pressed
		case 115:
			//cout << "S PRESSED" << endl;
			toggleFlatSmooth();
			break;

	 	// 'w' pressed
	 	case 119:
	 		//cout << "W PRESSED" << endl;
	 		toggleFilledWire();
	 		break;

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

		// When "h" is pressed the program will toggle between filled and hidden-line mode
		case 104:
			toggleHiddenLine();
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

// //****************************************************
// // parse patch data file
// //****************************************************
// void parsePatchData(char* file) {
// 	string STRING;
// 	ifstream in;
// 	in.open(file);
	
// 	float x, y, z;
// 	Vect* p0;
// 	Vect* p1;
// 	Vect* p2;
// 	Vect* p3;
// 	Curve* c; Patch* p;
// 	vector<Curve*> curves;
// 	int curves_seen = 0, patches_seen = 0, num_patches = 0;

// 	getline(in, STRING);
// 	stringstream ss0(STRING);
// 	ss0 >> num_patches;

// 	while(!in.eof()) {
// 		getline(in,STRING);
// 		if(STRING.length() > 10){

// 			stringstream ss(STRING);
// 			ss >> x >> y >> z;
// 			if(abs(x) > bb_max){ bb_max = x; }
// 			if(abs(y) > bb_max){ bb_max = y; }
// 			if(z > z_max){ z_max = z; }
// 			p0 = new Vect(x,y,z);

// 			ss >> x >> y >> z;
// 			if(abs(x) > bb_max){ bb_max = x; }
// 			if(abs(y) > bb_max){ bb_max = y; }
// 			if(z > z_max){ z_max = z; }

// 			p1 = new Vect(x,y,z);
			
// 			ss >> x >> y >> z;
// 			if(abs(x) > bb_max){ bb_max = x; }
// 			if(abs(y) > bb_max){ bb_max = y; }
// 			if(z > z_max){ z_max = z; }			
// 			p2 = new Vect(x,y,z);
			
// 			ss >> x >> y >> z;
// 			if(abs(x) > bb_max){ bb_max = x; }
// 			if(abs(y) > bb_max){ bb_max = y; }
// 			if(z > z_max){ z_max = z; }
// 			p3 = new Vect(x,y,z);

// 			c = new Curve(p0, p1, p2, p3);
// 			curves.push_back(c);
// 			curves_seen++;

// 			if(curves_seen == 4){
// 				curves_seen = 0;
// 				p = new Patch(curves[0], curves[1], curves[2], curves[3]);
// 				curves.clear();
// 				patches.push_back(p);
// 				patches_seen++;
// 			}

// 			if(patches_seen == num_patches){
// 				break;
// 			}
// 		}
// 	}

// 	in.close();
// }


//****************************************************
// the usual stuff, nothing exciting here
//**************************************************** 
int main(int argc, char *argv[]) {

	char* infile = NULL;

	if(argc > 1) {
	  	try {
	  		infile = argv[1];
			subdiv_param = strtof(argv[2],0);

			int i = 3;
			while(i < argc) {
				char* arg = argv[i];
				if(strcmp(arg, ADAPT_FLAG) == 0){
					isAdaptive = true;
					i++;
				} else {
					throw 2;
				}
			}
		} catch(int e) {
			if(e == 2){
				cout << "Unrecognized command. Program Aborted. " << endl;
				exit(1);
			} else {
				cout << "Error in input arguments. Program Aborted. " << endl;
				exit(1);
			}
	  }
	}

	parsePatchData(infile);

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
	glutCreateWindow("as3");

	initScene();							// quick function to set up scene

	glutDisplayFunc(myDisplay);				// function to run when its time to draw something
	glutReshapeFunc(myReshape);				// function to run when the window gets resized

	glutKeyboardFunc(key_modes);			// exit when spacebar is pressed, 's', 'w', zoom, etc.
	glutSpecialFunc(trans_modes);			// for transformations using keys

	glutMainLoop();							// infinite loop that will keep drawing and resizing
	return 0;
}
