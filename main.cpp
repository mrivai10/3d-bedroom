#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

float deltaX = 0;
float deltaY = 0;
bool mouseDown = false;
float rotateX = 0;
float rotateY = 0;
float rotateZ = 0;
float translateX = 0;
float translateY = 0;
float translateZ = 0;

GLuint wallTexture;
GLuint wood1Texture;
GLuint wood2Texture;
GLuint bedTexture;
GLuint photoTexture;
GLuint photo2Texture;
GLuint keyboardTexture;
GLuint metalTexture;
GLuint bookTexture;

void project(int width, int height){
	if (height==0) height=1;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, width/height, 2, 750);
	glTranslatef(0, -30, -300);
	glMatrixMode(GL_MODELVIEW);
}

void loadTexture(const char* filename, GLuint &textureID) {
    int imgWidth, imgHeight, numChannels;
    unsigned char *imageData = stbi_load(filename, &imgWidth, &imgHeight, &numChannels, 0);
    
    if(imageData){
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(imageData);
    } else {
        std::cout << "Texture: " << filename << " Failed to Load" << std::endl;
    }
}
	

void init(){
	glClearColor(0.1, 0.1, 0.1, 0.1);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glPointSize(10.0);
	glLineWidth(7.0f);
	glEnable(GL_TEXTURE_2D);
	
	loadTexture("wall.jpg", wallTexture);
    loadTexture("wood.jpg", wood1Texture);
    loadTexture("wood2.jpg", wood2Texture);
    loadTexture("bed.jpg", bedTexture);
    loadTexture("photo.jpg", photoTexture);
    loadTexture("photo2.jpg", photo2Texture);
    loadTexture("keyboard.jpg", keyboardTexture);
    loadTexture("metal.jpg", metalTexture);
    loadTexture("book.jpg", bookTexture);
}

void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseDown=true;
		deltaX = x - rotateY;
		deltaY = -y + rotateX; 
	}
	else mouseDown = false;
}

void mouseMotion(int x, int y){
	if(mouseDown){
		rotateY = x - deltaX;
		rotateX = y + deltaY;
		glutPostRedisplay();
	}
}

void drawBlockSurface(GLfloat vertices[8][3], GLubyte indices[6][4], GLfloat textureCoords[4][2], GLuint textureID = 0) {
    if (textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    
    for (int i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            if (textureID != 0 && j == 0) {
                glTexCoord2fv(textureCoords[0]);
            }
            glVertex3fv(vertices[indices[i][j]]);
            if (textureID != 0 && j < 3) {
                glTexCoord2fv(textureCoords[j+1]);
            }
        }
        glEnd();
    }
}

void drawBlockEdges(GLfloat vertices[8][3], GLubyte indices[6][4]) {
    glLineWidth(1.5f);
    for (int i = 0; i < 6; i++) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 4; j++) {
            glVertex3fv(vertices[indices[i][j]]);
        }
        glEnd();
    }
}

void drawBlock(float x, float y, float z, float width, float height, float depth, GLfloat color[3], GLuint textureID = 0) {
	glPushMatrix();
    glTranslatef(-100, -20, -80);
    
    GLfloat vertices[8][3] = {
        {x, y, z}, 
		{x, y, z + depth}, 
		{x + width, y, z + depth}, 
		{x + width, y, z},
        {x, y + height, z}, 
		{x, y + height, z + depth}, 
		{x + width, y + height, z + depth}, 
		{x + width, y + height, z}
    };
    
    GLubyte indices[6][4] = {
        {0, 1, 2, 3}, // bottom
        {4, 5, 6, 7}, // top
        {5, 1, 2, 6}, // front
        {0, 4, 7, 3}, // back
        {2, 3, 7, 6}, // right
        {1, 5, 4, 0}  // left
    };
    
    GLfloat textureCoords[4][2] = {
        {0.0f, 0.0f}, 
		{1.0f, 0.0f}, 
		{1.0f, 1.0f}, 
		{0.0f, 1.0f}
    };
    
    glColor3fv(color);
    drawBlockSurface(vertices, indices, textureCoords, textureID);
    
    glColor3f(0.75f, 0.75f, 0.75f);
    drawBlockEdges(vertices, indices);
    
    glPopMatrix();
}

void floor(){
	GLfloat floor[3] = {0.7f, 0.7f, 0.5f};
	drawBlock(0.0f, 0.0f, 0.0f, 200.0f, 5.0f, 200.0f, floor, wood1Texture);
}

void wall(){
	GLfloat leftWall[3] = {0.6f, 0.6f, 0.5f};
	drawBlock(0.0f, 5.0f, 0.0f, 195.0f, 150.0f, 5.0f, leftWall, wallTexture);
	
	GLfloat rightWall[3] = {0.6f, 0.6f, 0.5f};
	drawBlock(200.0f, 5.0f, 0.0f, -5.0f, 150.0f, 200.0f, rightWall, wallTexture);
}

void wardrobe(){
	//base
	GLfloat wardrobe[3] = {0.55f, 0.55f, 0.35f};
	drawBlock(7.0f, 5.0f, 7.0f, 36.0f, 98.0f, 38.0f, wardrobe, wood2Texture);
	
	//detail
	GLfloat wardrobeDetail[3] = {0.65f, 0.65f, 0.45f};
	drawBlock(5.0f, 5.0f, 5.0f, 2.0f, 100.0f, 42.0f, wardrobeDetail, wood2Texture);
	drawBlock(43.0f, 5.0f, 5.0f, 2.0f, 100.0f, 42.0f, wardrobeDetail, wood2Texture);
	drawBlock(7.0f, 103.0f, 45.0f, 36.0f, 2.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(7.0f, 5.0f, 5.0f, 36.0f, 100.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(7.0f, 25.0f, 45.0f, 36.0f, 2.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(23.0f, 27.0f, 45.0f, 2.0f, 76.0f, 2.0f, wardrobeDetail, wood2Texture);
	
	//drawer
	drawBlock(9.0f, 10.0f, 45.0f, 32.0f, 10.0f, 2.0f, wardrobeDetail, wood2Texture);
	
	//handle
	drawBlock(20.0f, 55.0f, 45.0f, 1.0f, 2.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(20.0f, 70.0f, 45.0f, 1.0f, 2.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(20.0f, 53.0f, 47.0f, 1.0f, 22.0f, 1.0f, wardrobeDetail, wood2Texture);
	
	drawBlock(27.0f, 55.0f, 45.0f, 1.0f, 2.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(27.0f, 70.0f, 45.0f, 1.0f, 2.0f, 2.0f, wardrobeDetail, wood2Texture);
	drawBlock(27.0f, 53.0f, 47.0f, 1.0f, 22.0f, 1.0f, wardrobeDetail, wood2Texture);
}

void desk(){
	GLfloat desk[3] = {0.65f, 0.55f, 0.3f};
	
	//top
	drawBlock(120.0f, 65.0f, 5.0f, 75.0f, 5.0f, 30.0f, desk, wood2Texture);
	
	//leg
	GLfloat desk2[3] = {0.55f, 0.45f, 0.25f};
	drawBlock(120.0f, 5.0f, 5.0f, 5.0f, 60.0f, 5.0f, desk2, wood2Texture);
	drawBlock(190.0f, 5.0f, 5.0f, 5.0f, 60.0f, 5.0f, desk2, wood2Texture);
	drawBlock(120.0f, 5.0f, 30.0f, 5.0f, 60.0f, 5.0f, desk2, wood2Texture);
	drawBlock(190.0f, 5.0f, 30.0f, 5.0f, 60.0f, 5.0f, desk2, wood2Texture);
	
	//legsub
	drawBlock(120.0f, 15.0f, 10.0f, 5.0f, 5.0f, 20.0f, desk2, wood2Texture);
	drawBlock(190.0f, 15.0f, 10.0f, 5.0f, 5.0f, 20.0f, desk2, wood2Texture);
}

void trashCan(){
	GLfloat trashCan[3] = {0.5f, 0.5f, 0.5f};
	
	drawBlock(95.0f, 5.0f, 2.0f, 20.0f, 60.0f, 5.0f, trashCan, metalTexture);
	drawBlock(95.0f, 5.0f, 2.0f, 2.0f, 60.0f, 25.0f, trashCan, metalTexture);
	drawBlock(95.0f, 5.0f, 25.0f, 20.0f, 60.0f, 2.0f, trashCan, metalTexture);
	drawBlock(115.0f, 5.0f, 2.0f, 2.0f, 60.0f, 25.0f, trashCan, metalTexture);
	drawBlock(95.0f, 5.0f, 2.0f, 20.0f, 2.0f, 25.0f, trashCan, metalTexture);
}

void bed(){
	GLfloat bed[3] = {0.65f, 0.55f, 0.3f};
	//top
	drawBlock(5.0f, 25.0f, 130.0f, 80.0f, 10.0f, 70.0f, bed, wood2Texture);
	
	//topbed
	drawBlock(5.0f, 35.0f, 135.0f, 5.0f, 30.0f, 60.0f, bed, wood2Texture);
	drawBlock(5.0f, 35.0f, 130.0f, 5.0f, 35.0f, 5.0f, bed, wood2Texture);
	drawBlock(5.0f, 35.0f, 195.0f, 5.0f, 35.0f, 5.0f, bed, wood2Texture);
	
	//leg
	drawBlock(5.0f, 5.0f, 130.0f, 5.0f, 20.0f, 5.0f, bed, wood2Texture);
	drawBlock(80.0f, 5.0f, 130.0f, 5.0f, 20.0f, 5.0f, bed, wood2Texture);
	drawBlock(5.0f, 5.0f, 195.0f, 5.0f, 20.0f, 5.0f, bed, wood2Texture);
	drawBlock(80.0f, 5.0f, 195.0f, 5.0f, 20.0f, 5.0f, bed, wood2Texture);
	
	//legsub
	drawBlock(5.0f, 10.0f, 135.0f, 5.0f, 5.0f, 60.0f, bed, wood2Texture);
	drawBlock(80.0f, 10.0f, 135.0f, 5.0f, 5.0f, 60.0f, bed, wood2Texture);
	drawBlock(10.0f, 10.0f, 130.0f, 70.0f, 5.0f, 5.0f, bed, wood2Texture);
	drawBlock(10.0f, 10.0f, 195.0f, 70.0f, 5.0f, 5.0f, bed, wood2Texture);
	
	//legstand
	drawBlock(42.5f, 10.0f, 130.0f, 5.0f, 15.0f, 5.0f, bed, wood2Texture);
	drawBlock(42.5f, 10.0f, 195.0f, 5.0f, 15.0f, 5.0f, bed, wood2Texture);
	
	//bed
	GLfloat bed2[3] = {0.9f, 0.9f, 0.9f};
	drawBlock(10.0f, 35.0f, 130.0f, 75.0f, 10.0f, 70.0f, bed2, bedTexture);
}

void pillow(){
	GLfloat pillow[3] = {0.9f, 0.85f, 0.8f};
	
	drawBlock(10.0f, 35.0f, 150.0f, 12.0f, 20.0f, 30.0f, pillow, bedTexture);
}

void chair(){
	GLfloat chair[3] = {0.65f, 0.55f, 0.3f};
	
	//top
	drawBlock(145.0f, 40.0f, 45.0f, 35.0f, 5.0f, 35.0f, chair, wood2Texture);
	drawBlock(145.0f, 45.0f, 75.0f, 35.0f, 40.0f, 5.0f, chair, wood2Texture);
	
	//leg
	drawBlock(145.0f, 5.0f, 45.0f, 5.0f, 35.0f, 5.0f, chair, wood2Texture);
	drawBlock(175.0f, 5.0f, 45.0f, 5.0f, 35.0f, 5.0f, chair, wood2Texture);
	drawBlock(145.0f, 5.0f, 75.0f, 5.0f, 35.0f, 5.0f, chair, wood2Texture);
	drawBlock(175.0f, 5.0f, 75.0f, 5.0f, 35.0f, 5.0f, chair, wood2Texture);
}

void pitcure(){
	//frame
	GLfloat pitcure[3] = {0.65f, 0.55f, 0.3f};
	drawBlock(193.0f, 100.0f, 90.0f, 2.0f, 40.0f, 2.0f, pitcure, wood2Texture);
	drawBlock(193.0f, 100.0f, 120.0f, 2.0f, 40.0f, 2.0f, pitcure, wood2Texture);
	drawBlock(193.0f, 140.0f, 90.0f, 2.0f, 2.0f, 32.0f, pitcure, wood2Texture);
	drawBlock(193.0f, 100.0f, 90.0f, 2.0f, 2.0f, 32.0f, pitcure, wood2Texture);
	
	//glass
	GLfloat glass[3] = {0.5f, 0.5f, 0.5f};
	drawBlock(194.0f, 101.0f, 91.0f, 1.0f, 40.0f, 30.0f, glass, photoTexture);
}

void laptop(){
	//keyboard screen
    glPushMatrix();
    
    glTranslatef(145.0f, 90.0f, 7.0f);
    glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
    
    GLfloat monitor[3] = {0.5f, 0.5f, 0.5f};
    //base
    drawBlock(0.0f, 0.0f, 0.0f, 30.0f, 30.0f, 1.0f, monitor);
    //frame
    drawBlock(0.0f, 0.0f, 1.0f, 1.0f, 30.0f, 1.0f, monitor);
    drawBlock(29.0f, 0.0f, 1.0f, 1.0f, 30.0f, 1.0f, monitor);
    drawBlock(0.0f, 29.0f, 1.0f, 30.0f, 1.0f, 1.0f, monitor);
    drawBlock(0.0f, 3.0f, 1.0f, 30.0f, 1.0f, 1.0f, monitor);
    
    GLfloat screen[3] = {0.8f, 0.9f, 0.9f};
    drawBlock(1.0f, 0.0f, 0.5f, 28.0f, 28.0f, 1.0f, screen, photo2Texture);
    
    glPopMatrix();
	
	//keyboard
	GLfloat keyboardBase[3] = {0.5f, 0.5f, 0.5f};
	drawBlock(145.0f, 70.0f, 16.0f, 30.0f, 2.0f, 17.0f, keyboardBase);
	GLfloat keyboard[3] = {0.8f, 0.9f, 0.9f};
	drawBlock(146.0f, 72.0f, 17.0f, 28.0f, 1.0f, 15.0f, keyboard, keyboardTexture);
}

void book(){
	GLfloat book[3] = {0.8f, 0.8f, 0.8f};
	drawBlock(194.0f, 70.0f, 5.0f, 1.0f, 30.0f, 15.0f, book, bookTexture);
	drawBlock(191.0f, 70.0f, 5.0f, 3.0f, 30.0f, 1.0f, book, bookTexture);
	drawBlock(190.0f, 70.0f, 5.0f, 1.0f, 30.0f, 15.0f, book, bookTexture);
	
	GLfloat paper[3] = {0.9f, 0.9f, 0.9f};
	drawBlock(191.0f, 70.0f, 6.0f, 3.0f, 30.0f, 13.0f, paper);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
    case 'W':
        translateZ += 3.0; // bergerak ke depan
        break;
    case 's':
    case 'S':
        translateZ -= 3.0; // bergerak ke belakang
        break;
    case 'd':
    case 'D':
        translateX -= 3.0; // bergerak ke kanan
        break;
    case 'a':
    case 'A':
        translateX += 3.0; // bergerak ke kiri
        break;
    case 'e':
    case 'E':
        rotateZ -= 3.0; // rotasi searah jarum jam
        break;
    case 'f':
    case 'F':
        rotateZ += 3.0; // rotasi lawan arah jarum jam
        break;
    case 'j':
    case 'J':
         rotateY -= 3.0; // rotasi ke kiri
        break;
    case 'l':
    case 'L':
        rotateY += 3.0; // rotasi ke kanan
        break;
    case 'i':
    case 'I':
        rotateX -= 3.0; // rotasi ke atas
        break;
    case 'm':
    case 'M':
        rotateX += 3.0; // rotasi ke bawah
        break;
    case 'n':
    case 'N':
        translateY += 3.0; // bergerak ke bawah
        break;
    case 'u':
    case 'U':
        translateY -= 3.0; // bergerak ke atas
        break;
    }

    glutPostRedisplay();
}

void setupLighting(){
    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   	GLfloat lightPosition[] = { 100.0f, 150.0f, 100.0f, 1.0f };

    GLfloat matAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat matDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat highShininess[] = { 100.0f };


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, highShininess);
}

void display() {
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	glLoadIdentity();
  	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
  	
  	glTranslatef(translateX, translateY, translateZ);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, 1.0);
    
	GLfloat modelview[16];
  	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

  	GLfloat lightPosition[] = { 100.0f, 150.0f, 100.0f, 1.0f };
  	GLfloat lightPositionTransformed[4];
  	for (int i = 0; i < 4; i++) {
    	lightPositionTransformed[i] = 0.0f;
    	for (int j = 0; j < 4; j++) {
      	lightPositionTransformed[i] += modelview[i + j * 4] * lightPosition[j];
    	}
  	}

  	glLightfv(GL_LIGHT0, GL_POSITION, lightPositionTransformed);

  	floor();
  	wall();
  	wardrobe();
  	desk();
  	trashCan();
  	bed();
  	pillow();
  	chair();
  	pitcure();
  	laptop();
  	book();

  	glutSwapBuffers();
}

int main (int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(250, 80);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("3D World Bedroom");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(project);
	glutKeyboardFunc(keyboard);
	setupLighting();
    init();
    glutMainLoop();
    
    return 0;
}
