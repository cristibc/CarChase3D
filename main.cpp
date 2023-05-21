/* Deplasarea observatorului intr-o scena 3D
SURSA:  lighthouse3D:  http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
Elemente de retinut:
- folosirea functiilor de desenare pentru a schita obiecte 3D
- schimbarea pozitiei observatorului se face in functia gluLookAt
- folosirea glutSpecialFunc si glutKeyboardFunc pentru interactiunea cu tastatura
*/

#include<gl/freeglut.h>
#include<math.h>
#include "SOIL.h"
#include <windows.h>
#include <chrono>
#include <iostream>

using namespace std;

std::chrono::high_resolution_clock::time_point previousTime;
std::chrono::high_resolution_clock::time_point currentTime;


// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;
GLuint textureWater;
float cameraSpeed = 0.7f;
float moveSpeed = 3.0f;
GLboolean keyLeftPressed, keyRightPressed, keyUpPressed, keyDownPressed = false;
int frameCount = 0;
float fpsDeltaTime = 0;

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}

void init(void) {
	textureWater = SOIL_load_OGL_texture("water.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS);
}


void update(float deltaTime) {
	float distanceToMove = cameraSpeed * deltaTime;

	if (keyLeftPressed) {
		angle -= cameraSpeed * deltaTime;
		lx = sin(angle);
		lz = -cos(angle);
	}

	if (keyRightPressed) {
		angle += cameraSpeed * deltaTime;
		lx = sin(angle);
		lz = -cos(angle);
	}

	if (keyUpPressed) {
		x += lx * moveSpeed * deltaTime;
		z += lz * moveSpeed * deltaTime;
	}

	if (keyDownPressed) {
		x -= lx * moveSpeed * deltaTime;
		z -= lz * moveSpeed * deltaTime;
	}

	fpsDeltaTime += deltaTime;


	if (fpsDeltaTime >= 1.0)
	{
		cout << frameCount << endl;

		frameCount = 0;
		fpsDeltaTime = 0;
	}

}


void renderScene(void)
{
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Draw ground
	//glColor3f(0.9f, 0.9f, 0.9f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureWater);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(10, 0);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(10, 10);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(0, 10);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 0.1f, -50.0f);
	glVertex3f(-10.0f, 0.1f, 50.0f);
	glVertex3f(10.0f, 0.1f, 50.0f);
	glVertex3f(10.0f, 0.1f, -50.0f);
	glEnd();

	 //Draw 36 SnowMen
	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i * 10.0, 0, j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}
	glutSwapBuffers();
	frameCount++;

}

void idleFunc() {

	auto currentTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = currentTime - previousTime;
	float deltaTime = duration.count();

	previousTime = currentTime;

	update(deltaTime);
	renderScene();

	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	}
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
}

//void processSpecialKeys(int key, int xx, int yy) {
//
//	float fraction = 0.1f;
//
//	switch (key)
//	{
//	case GLUT_KEY_LEFT:
//		angle -= 0.01f;
//		lx = sin(angle);
//		lz = -cos(angle);
//		break;
//	case GLUT_KEY_RIGHT:
//		angle += 0.01f;
//		lx = sin(angle);
//		lz = -cos(angle);
//		break;
//	case GLUT_KEY_UP:
//		x += lx * fraction;
//		z += lz * fraction;
//		break;
//	case GLUT_KEY_DOWN:
//		x -= lx * fraction;
//		z -= lz * fraction;
//		break;
//	}
//
//
//}

// Functie check activata la eliberarea tastelor
void keyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		keyLeftPressed = false;
		break;
	case GLUT_KEY_RIGHT:
		keyRightPressed = false;
		break;
	case GLUT_KEY_UP:
		keyUpPressed = false;
		break;
	case GLUT_KEY_DOWN:
		keyDownPressed = false;
		break;
	default:
		break;
	}
}

// Functie check activata la apasarea tastelor, implementare smooth
void keyPressed(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		keyLeftPressed = true;
		break;
	case GLUT_KEY_RIGHT:
		keyRightPressed = true;
		break;
	case GLUT_KEY_UP:
		keyUpPressed = true;
		break;
	case GLUT_KEY_DOWN:
		keyDownPressed = true;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Proiect 3D");

	init();

	previousTime = std::chrono::high_resolution_clock::now();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunc);
	//glutKeyboardFunc(processNormalKeys);
	//glutSpecialFunc(processSpecialKeys);

	glutSpecialFunc(keyPressed);
	glutSpecialUpFunc(keyUp);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

