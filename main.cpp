/* Deplasarea observatorului intr-o scena 3D
SURSA:  lighthouse3D:  http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
Elemente de retinut:
- folosirea functiilor de desenare pentru a schita obiecte 3D
- schimbarea pozitiei observatorului se face in functia gluLookAt
- folosirea glutSpecialFunc si glutKeyboardFunc pentru interactiunea cu tastatura
*/

#include <windows.h>
#include <gl/freeglut.h>
#include <math.h>
#include "SOIL.h"
#include <chrono>
#include <iostream>
#include <string>


using namespace std;

std::chrono::high_resolution_clock::time_point previousTime;
std::chrono::high_resolution_clock::time_point currentTime;

const float PI = 3.14159f;
// angle of rotation for the camera direction
double angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;
GLuint textureWater, textureAsphalt, textureFront, textureBack, textureLeft, textureRight, textureTop, textureBottom, textureSky, textureCrash;
GLuint texturePoliceFront, texturePoliceBack, texturePoliceRight, texturePoliceLeft;
GLuint texturePoliceCabinFront, texturePoliceCabinBack, texturePoliceCabinRight, texturePoliceCabinLeft;


float cameraSpeed = 0.7f;
float moveSpeed = 3.0f;
GLboolean keyLeftPressed, keyRightPressed, keyUpPressed, keyDownPressed = false;
int frameCount = 0;
float fpsDeltaTime = 0;
GLuint textureID;
float zCar, xCar;
float sceneSpeed = 0;

int obstacol1, obstacol2, obstacol3;
bool initObstacle1, initObstacle2, initObstacle3;
double iObstacol1 = 50;
double iObstacol2 = 50;
double iObstacol3 = 50;
float genR, genG, genB;
float genR1, genG1, genB1;
float genR2, genG2, genB2;
float genR3, genG3, genB3;
bool collisionCheck = false;
double xCoin = 0;
double yCoin = 50;
bool displayCoin = false;
int chance = 50;
double score = 0;
float angleCoin = 0;
bool camera1 = true;
bool camera2 = false;
bool camera3 = false;
float increasingSpeed = 1;
bool flipLight = false;
bool flipFog = true;

// Light position
GLfloat lightPos[] = { -40.0f, 20.0f, 15.0f, 0.0f };
GLfloat white[] = { 1, 1, 1, 1 };
GLfloat black[] = { 0, 0, 0, 1 };
GLfloat yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };




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
	gluPerspective(45.0f, ratio, 0.1f, 500.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void increaseScore(int value) {
	if (collisionCheck != true) {
		score += 1;
	}

	glutTimerFunc(100, increaseScore, 0);
}

void flipTheLight(int value) {
	if (collisionCheck != true) {
		flipLight = !flipLight;
	}
	glutTimerFunc(1000, flipTheLight, 0);

}

// Functie pentru adaugarea textului mai usor
void displayText(float x, float y, const std::string& text) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2f(x, y);

	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawCar(float R, float G, float B) {

	glPushMatrix();
	glRotatef(90.0f, 0.0, 1.0, 0.0);

	GLfloat shadowAmbient[] = { 0.2f, 0.2f, 0.2f, 0.5f }; 
	GLfloat shadowDiffuse[] = { 0.2f, 0.2f, 0.2f, 0.5f };
	GLfloat shadowSpecular[] = { 0.0f, 0.0f, 0.0f, 0.5f }; 
	GLfloat shadowShininess = 0.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Shadow of the car
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, shadowAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shadowDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, shadowSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shadowShininess);
	glTranslatef(0.05, -0.45, 0.05);
	glScalef(2.0, 0.0, 1.2);
	glColor4f(0.0, 0.0, 0.0, 0.3f);
	glutSolidCube(1.0);
	glPopMatrix();

	glDisable(GL_BLEND);

	GLfloat ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse1[] = { R, G, B, 1.0f };
	GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess1 = 50.0f;

	GLfloat ambient2[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse2[] = { R-0.3f, G-0.3f, B-0.3f, 1.0f };
	GLfloat specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess2 = 30.0f;

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess1);


	// Body
	glColor3f(R, G, B);
	glPushMatrix();
	glScalef(2.0f, 0.5f, 1.0f); 
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);


	//HeadLights

	// Left Headlight
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-1.05f, 0.05f, -0.33f);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();
	// Right Headlight
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-1.05f, 0.05f, 0.33f);
	glutSolidSphere(0.07, 20, 20); 
	glPopMatrix();

	//Rear Lights

	// Left Rear Light
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(1.0f, 0.06f, -0.33f);
	glScalef(0.2f, 0.2f, 0.5f); 
	glutSolidCube(0.5);
	glPopMatrix();
	// Right Rear Light
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(1.0f, 0.06f, 0.33f);
	glScalef(0.2f, 0.2f, 0.5f); 
	glutSolidCube(0.5);
	glPopMatrix();

	// Left Rear Light DETAIL
	glColor3f(0.95f, 0.6f, 0.4f);
	glPushMatrix();
	glTranslatef(1.01f, 0.06f, -0.33f);
	glScalef(0.2f, 0.1f, 0.4f);  
	glutSolidCube(0.5);
	glPopMatrix();
	// Right Rear Light DETAIL
	glColor3f(0.95f, 0.6f, 0.4f);
	glPushMatrix();
	glTranslatef(1.01f, 0.06f, 0.33f);
	glScalef(0.2f, 0.1f, 0.4f);
	glutSolidCube(0.5);
	glPopMatrix();

	//Bucata fata gri de la faruri
	glColor3f(0.3f, 0.3f, 0.3f); 
	glPushMatrix();
	glTranslatef(-0.85f, 0.05f, 0.0f);
	glScalef(0.8f, 0.5f, 1.7f);
	glutSolidCube(0.5);
	glPopMatrix();

	//Liniute pe bucata fata gri de la faruri
	//1
	glColor3f(0.6f, 0.6f, 0.6f);
	glPushMatrix();
	glTranslatef(-0.86f, 0.05f, 0.0f);
	glScalef(0.8f, 0.05f, 0.9f); 
	glutSolidCube(0.5);
	glPopMatrix();
	//2
	glColor3f(0.6f, 0.6f, 0.6f);
	glPushMatrix();
	glTranslatef(-0.86f, 0.0f, 0.0f);
	glScalef(0.8f, 0.05f, 0.9f);
	glutSolidCube(0.5);
	glPopMatrix();
	//3
	glColor3f(0.6f, 0.6f, 0.6f);
	glPushMatrix();
	glTranslatef(-0.86f, 0.1f, 0.0f);
	glScalef(0.8f, 0.05f, 0.9f);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess2);

	// Cabin
	glColor3f(R - 0.3, G - 0.3, B - 0.3);  
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f); 
	glScalef(0.8f, 0.5f, 0.8f);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();

	// Wheels
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(-0.5f, -0.25f, 0.3f);
	glutSolidCylinder(0.25, 0.25, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, -0.25f, 0.3f);
	glutSolidCylinder(0.25, 0.25, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5f, -0.25f, -0.55f);
	glutSolidCylinder(0.25, 0.25, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, -0.25f, -0.55f);
	glutSolidCylinder(0.25, 0.25, 20, 20);
	glPopMatrix();
	glPopMatrix();


}

void setupLighting()
{
	GLfloat ambientLumina[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat difuzLumina[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat alb[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat directieSpot[] = { 5, -1, -1, 0 };

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);

	// Set light properties
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLumina);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzLumina);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
}

void generateLucky() {
	int isLucky;
	xCoin = 0;
	yCoin = 50;
	displayCoin = false;
	isLucky = rand() % chance;
	if (isLucky == 0) {
		cout << "Lucky one!" << endl;
		chance = 50;
		yCoin = -50;
		displayCoin = true;
	}
	else {
		cout << "You're not lucky! chance: 1/" << chance << endl;
		chance = chance / 2;
	}
}

void generateColor(void) {

	int pickPrimary = rand() % 6;
	if (pickPrimary == 0) {
		genR = 1;
		genG = (float)rand() / (float)RAND_MAX;
		genB = 0;
	}
	else if (pickPrimary == 1) {
		genR = (float)rand() / (float)RAND_MAX;
		genG = 1;
		genB = 0;
	}
	else if (pickPrimary == 2) {
		genR = 0;
		genG = 1;
		genB = (float)rand() / (float)RAND_MAX;
	}
	else if (pickPrimary == 3) {
		genR = 0;
		genG = (float)rand() / (float)RAND_MAX;
		genB = 1;
	}
	else if (pickPrimary == 4) {
		genR = (float)rand() / (float)RAND_MAX;
		genG = 0;
		genB = 1;
	}
	else if (pickPrimary == 5) {
		genR = 1;
		genG = 0;
		genB = (float)rand() / (float)RAND_MAX;
	}
}

bool Collision(double x1, double y1, double x2, double y2, double xWidth, double xHeight, double yWidth, double yHeight) {
	if (x1 < x2 + yWidth && x1 + xWidth > x2 && y1 < y2 + yHeight && y1 + xHeight > y2)
	{
		return 1;
	}
	return 0;
}


// Generatorul de masini obstacole, acestea sunt generate random
void Obstacole(int value) {
	// Initializam indicele pentru translatie
	iObstacol1 = 50;
	iObstacol2 = 50;
	iObstacol3 = 50;

	// Check-uri pentru existenta obstacolelor
	initObstacle1 = false;
	initObstacle2 = false;
	initObstacle3 = false;

	// Generam un set de obstacole pozitionate random, intre 1 si 2 obstacole
	if (collisionCheck != true) {
		generateLucky();

		srand(time(NULL));
		cout << "Called function" << endl;
		obstacol1 = rand() % 2;
		obstacol2 = rand() % 2;
		obstacol3 = rand() % 2;
		int sumObstacol = obstacol1 + obstacol2 + obstacol3;
		while (sumObstacol < 1 || sumObstacol > 2) {
			cout << obstacol1 << " " << obstacol2 << " " << obstacol3 << endl;
			obstacol1 = rand() % 2;
			obstacol2 = rand() % 2;
			obstacol3 = rand() % 2;
			sumObstacol = obstacol1 + obstacol2 + obstacol3;
		}
		cout << "Good obstacle path: " << obstacol1 << " " << obstacol2 << " " << obstacol3 << endl;


		if (obstacol1 == 1) {
			iObstacol1 = -50;
			initObstacle1 = true;
			generateColor();
			genR1 = genR;
			genG1 = genG;
			genB1 = genB;
		}

		if (obstacol2 == 1) {
			iObstacol2 = -50;
			initObstacle2 = true;
			generateColor();
			genR2 = genR;
			genG2 = genG;
			genB2 = genB;
		}

		if (obstacol3 == 1) {
			iObstacol3 = -50;
			initObstacle3 = true;
			generateColor();
			genR3 = genR;
			genG3 = genG;
			genB3 = genB;
		}
	}

	// Timer pentru loop-ul obstacolelor
	glutTimerFunc(4000, Obstacole, 0);

}




void init(void) {
	textureWater = SOIL_load_OGL_texture("water.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS);
	textureAsphalt = SOIL_load_OGL_texture("asphalt.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS);
	textureFront = SOIL_load_OGL_texture("skybox/front.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureBack = SOIL_load_OGL_texture("skybox/back.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureLeft = SOIL_load_OGL_texture("skybox/left.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureRight = SOIL_load_OGL_texture("skybox/right.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureTop = SOIL_load_OGL_texture("skybox/top.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureBottom = SOIL_load_OGL_texture("skybox/bottom.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureCrash = SOIL_load_OGL_texture("crash.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceFront = SOIL_load_OGL_texture("policecar/police_front.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceBack = SOIL_load_OGL_texture("policecar/police_back.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceLeft = SOIL_load_OGL_texture("policecar/police_left.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceRight = SOIL_load_OGL_texture("policecar/police_right.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceCabinFront = SOIL_load_OGL_texture("policecar/policecabin_front.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceCabinBack = SOIL_load_OGL_texture("policecar/policecabin_back.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceCabinLeft = SOIL_load_OGL_texture("policecar/policecabin_left.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	texturePoliceCabinRight = SOIL_load_OGL_texture("policecar/policecabin_right.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);

	GLfloat fogColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.06f);
	glFogf(GL_FOG_START, 50.0f);
	glFogf(GL_FOG_END, 100.0f); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	PlaySound(TEXT("nightrider.wav"), NULL, SND_ASYNC | SND_LOOP);
}

void update(float deltaTime) {
	float distanceToMove = cameraSpeed * deltaTime;
	float cameraSpeed = 5;

	if (keyLeftPressed && x > -3) {
		x -= distanceToMove * cameraSpeed;
		xCar -= distanceToMove * cameraSpeed;
	}

	if (keyRightPressed && x < 3) {
		x += distanceToMove * cameraSpeed;
		xCar += distanceToMove * cameraSpeed;

	}
	increasingSpeed += 0.00001;

	sceneSpeed += distanceToMove * 20 * increasingSpeed;
	if (sceneSpeed > 11)
		sceneSpeed = -10;

	if (initObstacle1 == 1 && iObstacol1 < 100) {
		iObstacol1 += distanceToMove * 20 * increasingSpeed;
	}

	if (initObstacle2 == 1 && iObstacol2 < 100) {
		iObstacol2 += distanceToMove * 20 * increasingSpeed;
	}

	if (initObstacle3 == 1 && iObstacol3 < 100) {
		iObstacol3 += distanceToMove * 20 * increasingSpeed;
	}

	if (initObstacle1 == 0 && displayCoin == true) {
		xCoin = -2.1;
		yCoin += distanceToMove * 20 * increasingSpeed;
	}
	else if (initObstacle2 == 0 && displayCoin == true) {
		xCoin = 0;
		yCoin += distanceToMove * 20 * increasingSpeed;
	}
	else if (initObstacle3 == 0 && displayCoin == true) {
		xCoin = 2.1;
		yCoin += distanceToMove * 20 * increasingSpeed;
	}

	if (displayCoin == true) {
		angleCoin += distanceToMove * 75;
	}


	if (keyUpPressed) {
		z += distanceToMove;
	}

	if (keyDownPressed) {
		z -= distanceToMove;
	}

	fpsDeltaTime += deltaTime;


	if (fpsDeltaTime >= 1.0)
	{
		cout << "FPS: " << frameCount << endl;

		frameCount = 0;
		fpsDeltaTime = 0;
	}

	// Collision check for obstacles
	if (Collision(xCar-0.2, 2, -2.1, iObstacol1, 1.2, 2, 1, 2) == 1) {
		collisionCheck = true;
	}

	if (Collision(xCar-0.2, 2, 0, iObstacol2, 1.2, 2, 1, 2) == 1) {
		collisionCheck = true;
	}

	if (Collision(xCar-0.2, 2, 2.1, iObstacol3, 1.2, 2, 1, 2) == 1) {
		collisionCheck = true;
	}

	// Collision check for coin
	if (Collision(xCar - 0.2, 1, xCoin, yCoin, 1.2, 2, 1, 1) == 1) {
		score += 100;
		xCoin = 0;
		yCoin = 50;
		cout << "Touched coin!" << endl;
	}

	if (flipFog) {
		glEnable(GL_FOG);
	}
	else if (!flipFog) {
		glDisable(GL_FOG);
	}


}

void drawCoin() {
	const int numSides = 100; 
	const float radius = 0.5f;
	glPushMatrix();
	glRotatef(angleCoin, 0, 1, 0);

	// Front
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 33.0f);

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);

	for (int i = 0; i <= numSides; ++i) {
		float angle = static_cast<float>(i) / numSides * 2.0f * PI;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, 0.0f);
	}

	glEnd();
	glPopMatrix();

	// Back
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
	glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, -0.2f);

	for (int i = numSides; i >= 0; --i) {
		float angle = static_cast<float>(i) / numSides * 2.0f * PI;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, -0.2f);
	}

	glEnd();
	glPopMatrix();

	// Coin's side
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
	glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
	glBegin(GL_QUAD_STRIP);

	for (int i = 0; i <= numSides; ++i) {
		float angle = static_cast<float>(i) / numSides * 2.0f * PI;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, 0.0f);
		glVertex3f(x, y, -0.2f);
	}

	glEnd();
	glPopMatrix();

	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);

}

void drawSky(void) {
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float skyboxSize = 300.0f;

	// Front quad
	glBindTexture(GL_TEXTURE_2D, textureFront);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(skyboxSize, -skyboxSize, -skyboxSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyboxSize, -skyboxSize, -skyboxSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
	glEnd();

	// Left quad
	glBindTexture(GL_TEXTURE_2D, textureLeft);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(skyboxSize, -skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyboxSize, -skyboxSize, -skyboxSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(skyboxSize, skyboxSize, skyboxSize);
	glEnd();

	// Back quad
	glBindTexture(GL_TEXTURE_2D, textureBack);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxSize, -skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyboxSize, -skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyboxSize, skyboxSize, skyboxSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
	glEnd();

	// Right quad
	glBindTexture(GL_TEXTURE_2D, textureRight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxSize, -skyboxSize, -skyboxSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyboxSize, -skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
	glEnd();

	// Top quad
	glBindTexture(GL_TEXTURE_2D, textureTop);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyboxSize, skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
	glEnd();

	// Bottom quad
	glBindTexture(GL_TEXTURE_2D, textureBottom);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxSize, -skyboxSize, -skyboxSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxSize, -skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyboxSize, -skyboxSize, skyboxSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyboxSize, -skyboxSize, -skyboxSize);
	glEnd();

	// Restore enable bits and matrix
	glPopAttrib();

}


void renderScene(void)
{
	if (collisionCheck != true)
	{
		// Clear Color and Depth Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset transformations
		glLoadIdentity();
		// Set the camera
		// Default camera below

		//gluLookAt(x, 1.0f, z,
		//	x + lx, 1.0f, z + lz,
		//	0.0f, 1.0f, 0.0f);



		if (camera1 == true) {
			gluLookAt(x, 2.0f, z,
				x, 1.0f, z - 4,
				0.0f, 1.0f, 0.0f);
		}

		else if (camera2 == true) {
			gluLookAt(0.0, 2.0f, z,
				0.0, 1.0f, z - 4,
				0.0f, 1.0f, 0.0f);
		}

		else if (camera3 == true) {
			if (flipFog) {
				flipFog = !flipFog;
			}
			gluLookAt(0.0, 16.0f, z-9,
				0.0, 1.0f, z-9,
				0.0f, 0.0f, -1.0f);
		}

		drawSky();

		setupLighting();


		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		//Moving the scene
		glPushMatrix();
		glTranslatef(0, 0, sceneSpeed);

		// Draw ground
		//glColor3f(0.9f, 0.9f, 0.9f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureWater);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-200.0f, 0.0f, -200.0f);
		glTexCoord2f(10, 0);
		glVertex3f(-200.0f, 0.0f, 200.0f);
		glTexCoord2f(10, 10);
		glVertex3f(200.0f, 0.0f, 200.0f);
		glTexCoord2f(0, 10);
		glVertex3f(200.0f, 0.0f, -200.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		// Asphalt
		//glColor3f(0.9f, 0.9f, 0.9f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureAsphalt);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-4.0f, 0.1f, -200.0f);
		glTexCoord2f(10, 0);
		glVertex3f(-4.0f, 0.1f, 200.0f);
		glTexCoord2f(10, 10);
		glVertex3f(4.0f, 0.1f, 200.0f);
		glTexCoord2f(0, 10);
		glVertex3f(4.0f, 0.1f, -200.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		// Median of road
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		float lineLength = 3.0f;
		float lineHeight = 0.11f;
		float lineStart = -200.0f;
		float lineEnd = 200.0f;
		float lineWidth = 0.1f;
		float lineSpacing = 7.0f;
		float lineXPos = -1.33f;

		for (float z = lineStart; z < lineEnd; z += lineSpacing)
		{
			glBegin(GL_QUADS);
			glVertex3f(lineXPos - lineWidth, lineHeight, z);
			glVertex3f(lineXPos - lineWidth, lineHeight, z + lineSpacing - lineLength);
			glVertex3f(lineXPos + lineWidth, lineHeight, z + lineSpacing - lineLength);
			glVertex3f(lineXPos + lineWidth, lineHeight, z);
			glEnd();
		}

		lineLength = 3.0f;
		lineHeight = 0.11f;
		lineStart = -200.0f;
		lineEnd = 200.0f;
		lineWidth = 0.1f;
		lineSpacing = 7.0f;
		lineXPos = 1.33f;


		for (float z = lineStart; z < lineEnd; z += lineSpacing)
		{
			glBegin(GL_QUADS);
			glVertex3f(lineXPos - lineWidth, lineHeight, z);
			glVertex3f(lineXPos - lineWidth, lineHeight, z + lineSpacing - lineLength);
			glVertex3f(lineXPos + lineWidth, lineHeight, z + lineSpacing - lineLength);
			glVertex3f(lineXPos + lineWidth, lineHeight, z);
			glEnd();
		}

		glPopMatrix();

		// Player Car

		glPushMatrix();
		glTranslatef(xCar, 0.6, 0);
		glRotatef(180, 0, 1, 0);

		GLfloat shadowAmbient[] = { 0.2f, 0.2f, 0.2f, 0.5f };
		GLfloat shadowDiffuse[] = { 0.2f, 0.2f, 0.2f, 0.5f };
		GLfloat shadowSpecular[] = { 0.0f, 0.0f, 0.0f, 0.5f };
		GLfloat shadowShininess = 0.0f;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Shadow of the car
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, shadowAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shadowDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, shadowSpecular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shadowShininess);
		glTranslatef(-0.05, -0.45, 0.05);
		glRotatef(90, 0, 1, 0);
		glScalef(2.0, 0.0, 1.2);
		glColor4f(0.0, 0.0, 0.0, 0.3f);
		glutSolidCube(1.0);
		glPopMatrix();

		glDisable(GL_BLEND);

		glDisable(GL_LIGHTING);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

;
		//drawCar(0, 0.925, 1);
		//glPopMatrix();

		// Girofar
		glPushMatrix();
		glTranslatef(-0.18, 0.8, 0.2);
		glScalef(0.35, 0.05, 0.1);
		if (flipLight) {
			glColor4f(1.0, 0.0, 0.0, 1.0);
		}
		else {
			glColor4f(0.0, 0.0, 1.0, 1.0);

		}
		glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.18, 0.8, 0.2);
		glScalef(0.35, 0.05, 0.1);
		if (flipLight) {
			glColor4f(0.0, 0.0, 1.0, 1.0);
		}
		else {
			glColor4f(1.0, 0.0, 0.0, 1.0);

		}
		glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
		glRotatef(90, 0, 1, 0);

		//HeadLights

		// Left Headlight
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(-1.05f, 0.05f, -0.33f);
		glutSolidSphere(0.07, 20, 20);
		glPopMatrix();
		// Right Headlight
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(-1.05f, 0.05f, 0.33f);
		glutSolidSphere(0.07, 20, 20);
		glPopMatrix();

		//Rear Lights

		// Left Rear Light
		glColor3f(1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glTranslatef(1.0f, 0.06f, -0.33f);
		glScalef(0.2f, 0.2f, 0.5f);
		glutSolidCube(0.5);
		glPopMatrix();
		// Right Rear Light
		glColor3f(1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glTranslatef(1.0f, 0.06f, 0.33f);
		glScalef(0.2f, 0.2f, 0.5f);
		glutSolidCube(0.5);
		glPopMatrix();

		// Left Rear Light DETAIL
		glColor3f(0.95f, 0.6f, 0.4f);
		glPushMatrix();
		glTranslatef(1.01f, 0.06f, -0.33f);
		glScalef(0.2f, 0.1f, 0.4f);
		glutSolidCube(0.5);
		glPopMatrix();
		// Right Rear Light DETAIL
		glColor3f(0.95f, 0.6f, 0.4f);
		glPushMatrix();
		glTranslatef(1.01f, 0.06f, 0.33f);
		glScalef(0.2f, 0.1f, 0.4f); 
		glutSolidCube(0.5);
		glPopMatrix();



		// Bucata fata gri de la faruri
		glColor3f(0.3f, 0.3f, 0.3f);
		glPushMatrix();
		glTranslatef(-0.85f, 0.05f, 0.0f);
		glScalef(0.8f, 0.5f, 1.7f);
		glutSolidCube(0.5);
		glPopMatrix();

		//Liniute pe bucata fata gri de la faruri
		//1
		glColor3f(0.6f, 0.6f, 0.6f);
		glPushMatrix();
		glTranslatef(-0.86f, 0.05f, 0.0f);
		glScalef(0.8f, 0.05f, 0.9f);
		glutSolidCube(0.5);
		glPopMatrix();
		//2
		glColor3f(0.6f, 0.6f, 0.6f);
		glPushMatrix();
		glTranslatef(-0.86f, 0.0f, 0.0f);
		glScalef(0.8f, 0.05f, 0.9f);
		glutSolidCube(0.5);
		glPopMatrix();
		//3
		glColor3f(0.6f, 0.6f, 0.6f);
		glPushMatrix();
		glTranslatef(-0.86f, 0.1f, 0.0f);
		glScalef(0.8f, 0.05f, 0.9f);
		glutSolidCube(0.5);
		glPopMatrix();
		glPopMatrix();


		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);


		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glScalef(1.0f, 0.5f, 2.0f);

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);


		// Front face
		glBindTexture(GL_TEXTURE_2D, texturePoliceFront);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glEnd();

		// Back face
		glBindTexture(GL_TEXTURE_2D, texturePoliceBack);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
		glEnd();

		// Left face
		glBindTexture(GL_TEXTURE_2D, texturePoliceLeft);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		glEnd();

		// Right face
		glBindTexture(GL_TEXTURE_2D, texturePoliceRight);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
		glEnd();

		// Top face
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glEnd();

		// Bottom face
		glBegin(GL_QUADS);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glEnd();

		glPopMatrix();

		// CABIN

		glColor3f(1.0f, 1.0f, 1.0f); 
		glPushMatrix();
		glTranslatef(0.0f, 0.5f, 0.0f);
		glScalef(0.8, 0.5f, 0.8f);

		// Front face
		glBindTexture(GL_TEXTURE_2D, texturePoliceCabinFront);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glEnd();

		// Back face
		glBindTexture(GL_TEXTURE_2D, texturePoliceCabinBack);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
		glEnd();

		// Left face
		glBindTexture(GL_TEXTURE_2D, texturePoliceCabinLeft);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		glEnd();

		// Right face
		glBindTexture(GL_TEXTURE_2D, texturePoliceCabinRight);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
		glEnd();

		// Top face
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glEnd();

		// Bottom face
		glBegin(GL_QUADS);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glEnd();

		glPopMatrix();

		glRotatef(90, 0, 1, 0);

		// Wheels
		GLfloat wheelShininess = 100.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, wheelShininess);

		glColor3f(0.0f, 0.0f, 0.0f);
		glPushMatrix();
		glTranslatef(-0.5f, -0.25f, 0.3f);
		glutSolidCylinder(0.25, 0.25, 20, 20); 
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5f, -0.25f, 0.3f);
		glutSolidCylinder(0.25, 0.25, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.5f, -0.25f, -0.55f); 
		glutSolidCylinder(0.25, 0.25, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5f, -0.25f, -0.55f);
		glutSolidCylinder(0.25, 0.25, 20, 20);

		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

		// END PLAYER CAR

		// Obstacles

		glPushMatrix();
		glTranslated(-2.1, 0.6, iObstacol1);
		drawCar(genR1, genG1, genB1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0.6, iObstacol2);
		drawCar(genR2, genG2, genB2);
		glPopMatrix();

		glPushMatrix();
		glTranslated(2.1, 0.6, iObstacol3);
		drawCar(genR3, genG3, genB3);
		glPopMatrix();

		glPushMatrix();
		glTranslated(xCoin, 0.7, yCoin);
		drawCoin();
		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);
		displayText(30, 750, "Score: ");
		string scoreString = to_string(int(score));
		const char* scoreDisplayable = scoreString.c_str();
		displayText(95, 749, scoreDisplayable);

		glutSwapBuffers();
	}
	else {
		int highScore = score;
		string highScoreString = to_string(highScore);
		const char* highScoreDisplayable = highScoreString.c_str();
		glutIdleFunc(NULL);
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1200, 0, 800);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		// Bind the game over texture
		glBindTexture(GL_TEXTURE_2D, textureCrash);

		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(0, 0);
		glTexCoord2f(1, 1); glVertex2f(1200, 0);
		glTexCoord2f(1, 0); glVertex2f(1200, 800);
		glTexCoord2f(0, 0); glVertex2f(0, 800);
		glEnd();

		glColor3f(0.0, 0.0, 0.0);
		displayText(500, 500, "High Score: ");
		displayText(480, 465, "Press UP to restart ");
		displayText(630, 499, highScoreDisplayable);

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();



		glutSwapBuffers();
	}
	frameCount++;
}

// Function for updating game physics based on time and not framerate
void idleFunc() {

	auto currentTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = currentTime - previousTime;
	float deltaTime = duration.count();

	previousTime = currentTime;

	update(deltaTime);
	renderScene();

	glutPostRedisplay();
}

// Functie check activata la eliberarea tastelor
void keyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		keyLeftPressed = false;
		break;
	case GLUT_KEY_RIGHT:
		keyRightPressed = false;
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
		if (collisionCheck == true) {
			//glClearColor(0, 0.839, 0.082, 1);
			collisionCheck = false;
			iObstacol1 = 50;
			iObstacol2 = 50;
			iObstacol3 = 50;
			yCoin = 50;
			score = 0;
			increasingSpeed = 1;
			glutIdleFunc(idleFunc);
			break;
		}
	default:
		break;
	}
	glutPostRedisplay();
}

void keysFunc(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		camera1 = true;
		camera2 = false;
		camera3 = false;
		break;

	case '2':
		camera1 = false;
		camera2 = true;
		camera3 = false;
		break;

	case '3':
		camera1 = false;
		camera2 = false;
		camera3 = true;
		break;

	case '4':
		flipFog = !flipFog;
		cout << "flipfog: " << flipFog << endl;
		break;

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
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Highway Racer 3D");

	init();

	previousTime = std::chrono::high_resolution_clock::now();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idleFunc);

	glutSpecialFunc(keyPressed);
	glutSpecialUpFunc(keyUp);
	glutKeyboardFunc(keysFunc);

	glutTimerFunc(5000, Obstacole, 0);
	glutTimerFunc(100, increaseScore, 0);
	glutTimerFunc(1000, flipTheLight, 0);


	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
