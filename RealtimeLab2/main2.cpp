/*
  Simple Demo for GLSL 2.0
  www.lighthouse3d.com
  (slightly modified)

*/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <windows.h>	// for timeGetTime()
#include <mmsystem.h>


#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"


#pragma comment(lib, "glew32.lib")



GLuint v, f, p;

char * vs[3];
char * fs[3];

float lpos[4] = {1,0.5,1,0};
float rot = 0; 
float xrot = 0;

DWORD		lastTickCount;



void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);


}


void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glPushMatrix();
	glRotatef(rot, 0, 1, 0);
	glRotatef(xrot, 0, 0, 1);
	glutSolidTeapot(1);
	glPopMatrix();
	glutSwapBuffers();
}


/**
THIS CODE IS GARBAGE, BEYOND MUCK

Problem with const arguments I could _not_ figure out
**/



void setShadersGouraud() {
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead("base.vert");
	fs = textFileRead("gouraud.frag");
	
	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);
	
	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);
	
	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
}

void setShadersBasic() {
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead("basic.vert");
	fs = textFileRead("basic.frag");
	
	const char * f1 = fs;
	const char * v1 = vs;

	glShaderSource(v, 1, &v1, NULL);
	glShaderSource(f, 1, &f1, NULL);
	
	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);
	
	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
}

void setShadersPhong() {
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead("phong.vert");
	fs = textFileRead("phong.frag");
	
	const char * f1 = fs;
	const char * v1 = vs;

	glShaderSource(v, 1, &v1, NULL);
	glShaderSource(f, 1, &f1, NULL);
	
	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);
	
	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
}


void processNormalKeys(unsigned char key, int x, int y) {

	switch (key)
	{
	case 27: 
		exit(0);
	case 'a':
		rot -= 5;
		break;
	case 'd':
		rot += 5;
		break;
	case 'z':
		setShadersBasic();
		break;
	case 'x':
		setShadersPhong();
		break;
	case 'c':
		setShadersGouraud();
		break;
	case 'w':
		xrot += 5;
		break;
	case 's':
		xrot -= 5;
		break;
	case 'r':
		xrot = 0;
		break;
	}

	glutPostRedisplay();
}

void updateScene(){
	
	// Wait until at least 16ms passed since start of last frame
	// Effectively caps framerate at ~60fps
	while(timeGetTime()-lastTickCount<16);
	lastTickCount=timeGetTime();
	rot += 1.0f;

	//computeInverseKinematicsAngles();
   
    // Do any other updates here
	
	// Draw the next frame
    glutPostRedisplay();

}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("GLSL Example");

	glutDisplayFunc(renderScene);
	glutIdleFunc(updateScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0,1.0,1.0,1.0);
//	glEnable(GL_CULL_FACE);

	glewInit();
	
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
	

	setShadersBasic();


	glutMainLoop();

	return 0;
}

