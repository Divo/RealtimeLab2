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



GLuint v_toon, f_toon, p_toon;
GLuint v_phong, f_phong, p_phong;
GLuint v_minn, f_minn, p_minn;

char * vs[3];
char * fs[3];

float lpos[4] = {1,0.5,1,0};
float rot = 0; 
float xrot = 0;

DWORD		lastTickCount;

GLint tresh1;
float tresh1_value = 0.95;

GLint gl_ior;
float ior = 1.9;
GLint gl_k = 1.5;
float k = 1.5;
GLint gl_roughness = 0.15;
float roughness = 0.15;


float clamp_amt = 0.0;
GLint gl_clamp;



void setShadersminnaert();
void setShaderstoon();
void setShadersPhong();

typedef void (*fn)();
static fn funcs[] = {setShadersminnaert, setShaderstoon, setShadersPhong };

int calls[] = {0, 1, 2};

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


int rotate (int * a, int n, int k) {
int c, tmp, v;

    if (a == NULL || n <= 0) return -__LINE__;
    if (k < 0 || k >= n) {
        k %= n;
        if (k < 0) k += n;
    }
    if (k == 0) return 0;

    c = 0;
    for (v = 0; c < n; v++) {
        int t = v, tp = v + k;
        int tmp = a[v];
        c++;
        while (tp != v) {
            a[t] = a[tp];
            t = tp;
            tp += k;
            if (tp >= n) tp -= n;
            c++;
        }
        a[t] = tmp;
    }

    return 0;
}
void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,10.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	funcs[calls[0]]();
	glPushMatrix();
		glTranslatef(-3.0f, 0.0f, 0.0f);
		glRotatef(rot, 0, 1, 0);
		glRotatef(xrot, 0, 0, 1);
		glutSolidTeapot(1);
	glPopMatrix();
	funcs[calls[1]]();
	glPushMatrix();
		glRotatef(rot, 0, 1, 0);
		glRotatef(xrot, 0, 0, 1);
		glutSolidCube(1);
	glPopMatrix();
	funcs[calls[2]]();
	glPushMatrix();
		glTranslatef(3.0f, 0.0f, 0.0f);
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



void loadShadersminnaert() {
	char *vs = NULL,*fs = NULL;

	v_minn = glCreateShader(GL_VERTEX_SHADER);
	f_minn = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead("minnaert.vert");
	fs = textFileRead("minnaert.frag");
	
	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v_minn, 1, &vv, NULL);
	glShaderSource(f_minn, 1, &ff, NULL);
	
	free(vs);free(fs);

	glCompileShader(v_minn);
	glCompileShader(f_minn);
	
	p_minn = glCreateProgram();
	glAttachShader(p_minn,f_minn);
	glAttachShader(p_minn,v_minn);

	glLinkProgram(p_minn);
	
}

void setShadersminnaert() {

	glUseProgram(p_minn);

	gl_ior = glGetUniformLocation(p_minn, "ior");
	glUniform1f(gl_ior, ior);

	gl_k = glGetUniformLocation(p_minn, "k");
	glUniform1f(gl_k, k);

	gl_roughness = glGetUniformLocation(p_minn, "roughness");
	glUniform1f(gl_roughness, roughness);



}

void loadShaderstoon() {
	char *vs = NULL,*fs = NULL;

	v_toon = glCreateShader(GL_VERTEX_SHADER);
	f_toon = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead("toon.vert");
	fs = textFileRead("toon.frag");
	
	const char * f1 = fs;
	const char * v1 = vs;

	glShaderSource(v_toon, 1, &v1, NULL);
	glShaderSource(f_toon, 1, &f1, NULL);
	
	free(vs);free(fs);

	glCompileShader(v_toon);
	glCompileShader(f_toon);
	
	p_toon = glCreateProgram();
	glAttachShader(p_toon,f_toon);
	glAttachShader(p_toon,v_toon);

	glLinkProgram(p_toon);
	
}

void setShaderstoon() {

	glUseProgram(p_toon);

	tresh1 = glGetUniformLocation(p_toon, "tresh");
	glUniform1f(tresh1, tresh1_value);

}

void loadShadersPhong() {
	char *vs = NULL,*fs = NULL;

	v_phong = glCreateShader(GL_VERTEX_SHADER);
	f_phong = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead("phong.vert");
	fs = textFileRead("phong.frag");
	
	const char * f1 = fs;
	const char * v1 = vs;

	glShaderSource(v_phong, 1, &v1, NULL);
	glShaderSource(f_phong, 1, &f1, NULL);
	
	free(vs);free(fs);

	glCompileShader(v_phong);
	glCompileShader(f_phong);
	
	p_phong = glCreateProgram();
	glAttachShader(p_phong,f_phong);
	glAttachShader(p_phong,v_phong);

	glLinkProgram(p_phong);
	
}

void setShadersPhong() {

	glUseProgram(p_phong);

	gl_clamp = glGetUniformLocation(p_phong, "clamp_amt");
	glUniform1f(gl_clamp, clamp_amt);

	//glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

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
		setShaderstoon();
		break;
	case 'x':
		setShadersPhong();
		break;
	case 'c':
		setShadersminnaert();
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
	case 'i' :
		tresh1_value -= 0.1;
		k -= 1.1;
		clamp_amt -= 0.1;
		break;
	case 'o' :
		tresh1_value += 0.1;
		k += 1.1;
		clamp_amt += 0.1;
		break;
	case 'k' :
		rotate(calls, 3, 1);
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
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

	loadShadersminnaert();
	loadShadersPhong();
	loadShaderstoon();

	setShaderstoon();


	glutMainLoop();

	return 0;
}

