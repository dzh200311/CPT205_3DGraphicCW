
#define FREEGLUT_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#define PI 3.14159265
#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h> 
#include <stdio.h>
#include <cmath>
#include "vector"
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

using namespace std;


//texture
GLint imagewidth0, imagewidth1, imagewidth2,imagewidth3;
GLint imageheight0, imageheight1, imageheight2, imageheight3;
GLint pixellength0, pixellength1, pixellength2, pixellength3;
vector<GLubyte*>p; //p for texture

GLuint texture[4];

// camera related parameter
float cameraX = 0.0f, cameraY = 3.0f, cameraZ = 25.0f; //camera position
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;  // look at point
float angleX = 0.0f, angleY = 0.0f; // Angle of View Rotation

//Object coordinates associated with the animation
int time_interval = 10; // declare refresh interval in ms
GLfloat manOnGround = 0.38f; // man coordinate in y-axis
GLfloat manPosition_x[5] = { 8.3f, 8.5f, 8.22f, 8.43f,8.33f };
GLfloat manPosition_z[5] = { 4.0f, -9.0f, 3.5f, -4.3f,2.9f };
GLfloat carPosition_x[3] = { -4.0,4.0,0.0 };
GLfloat carPosition_z[3] = { 19.3,19.3,17.9 };
GLfloat flagStep = 0; // step param for flag animation

// parameter for mouse
int lastMouseX = -1, lastMouseY = -1;
 

// lighting
GLfloat lightPosition[] = { 0.6f, 1.414f, 1.0f, 0.0f };  
GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };   
GLfloat lightDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };   
GLfloat lightSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };  

// material 
GLfloat materialAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   
GLfloat materialDiffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };   
GLfloat materialSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };  
GLfloat materialShininess = 250.0f; 


// function to read texture files
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
    GLubyte* pixeldata;
    FILE* pfile;
    fopen_s(&pfile, path, "rb");
    if (pfile == 0) exit(0);
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);
    pixellength = imagewidth * 3;
    while (pixellength % 4 != 0)pixellength++;
    pixellength *= imageheight;
    pixeldata = (GLubyte*)malloc(pixellength);
    if (pixeldata == 0) exit(0);
    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    p.push_back(pixeldata); // Similar to glDrawPixels for program 3
    fclose(pfile);
}


// Initialize lighting and materials .etc
void init() {
    glClearColor(0.27f, 0.37f, 0.59f, 1.0f);  // background color
    glEnable(GL_DEPTH_TEST);  
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    ReadImage("S.bmp", imagewidth0, imageheight0, pixellength0);
    ReadImage("SGLS.bmp", imagewidth1, imageheight1, pixellength1);
    ReadImage("FBGLS.bmp", imagewidth2, imageheight2, pixellength2);
    ReadImage("BG.bmp", imagewidth3, imageheight3, pixellength3);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set pixel storage modes (in the memory)
    glGenTextures(4, &texture[0]); // number of texture names to be generated and an array of texture names
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);


    glEnable(GL_LIGHTING);   
    glEnable(GL_LIGHT0);    

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    
    
}

void when_in_mainloop()
{ // idle callback function
    glutPostRedisplay(); // force OpenGL to redraw the current window
}


//animation function
void manAni(int value) {
    for (int i = 0; i < 5; ++i) {
        
        GLfloat speed = 0.01;
         
        if (manPosition_z[i] <= 20 && manPosition_z[i] >= -8) {
            manPosition_z[i] += speed;
        }
        else {
            manPosition_z[i] = -8;
        }

     
    }
    glutTimerFunc(time_interval, manAni, 1);
}

void carAni(int value) {
    for (int i = 0; i < 3; ++i) {

        
        GLfloat speed = 0.05;
        if (carPosition_x[i] <= 20 && manPosition_x[i] >= -8) {
            carPosition_x[i] += speed;
        }
        else {
            carPosition_x[i] = -8;
        }


    }
    glutTimerFunc(time_interval, carAni, 1);
}

void OnTimerFlag(int value)
{
    flagStep += 0.3;
    glutTimerFunc(20, OnTimerFlag, 1);
}

//function to draw object

//draw ground
void drawPlane() {
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-50.0f, -0.01f, -50.0f);
    glVertex3f(50.0f, -0.01f, -50.0f);
    glVertex3f(50.0f, -0.01f, 50.0f);
    glVertex3f(-50.0f, -0.01f, 50.0f);
    glEnd();


    //Drawing the floor of the square
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth3, imageheight3, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[3]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  
    glTexCoord2f(0.0, 0.0); glVertex3f(-8, 0.001f, 20.0f);
    glTexCoord2f(5.0, 0.0); glVertex3f(8, 0.001f, 20.0f);
    glTexCoord2f(5.0, 5.0); glVertex3f(8, 0.001f, 32.0f);
    glTexCoord2f(0.0, 5.0); glVertex3f(-8, 0.001f, 32.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    //sky box
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.5f, 0.9f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-100.0f, -0.01f, -50.0f);
    glVertex3f(100.0f, -0.01f, -50.0f);
    glVertex3f(100.0f, 100.0f, -50.0f);
    glVertex3f(-100.0f, 100.0f, -50.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-100.0f, -0.01f, 50.0f);
    glVertex3f(100.0f, -0.01f, 50.0f);
    glVertex3f(100.0f, 100.0f, 50.0f);
    glVertex3f(-100.0f, 100.0f, 50.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-50.0f, -0.01f, -100.0f);
    glVertex3f(-50.0f, -0.01f, 100.0f);
    glVertex3f(-50.0f, 100.0f, 100.0f);
    glVertex3f(-50.0f, 100.0f, -100.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(50.0f, -0.01f, -100.0f);
    glVertex3f(50.0f, -0.01f, 100.0f);
    glVertex3f(50.0f, 100.0f, 100.0f);
    glVertex3f(50.0f, 100.0f, -100.0f);
    glEnd();
    glPopMatrix();

}

// draw a man with walking motion. Where side==0 for left arm and 1 for right
void drawArm(int side) {
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    // Arm rotation animation. The direction of rotation is determined by the side parameter
    GLfloat angle = (side == 0) ? sin(glutGet(GLUT_ELAPSED_TIME) * 0.001) * 30.0 : -sin(glutGet(GLUT_ELAPSED_TIME) * 0.001) * 30.0;
    glRotatef(angle, 0.1, 0.0, 0.0);

    if (side == 0) {
        glPushMatrix();
        glTranslatef(-0.08, -0.1, 0.0);  
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0);
        glutSolidCube(0.04);
        glColor3f(1.0, 1.0, 0.0);
        glTranslatef(0, -0.04, 0.0);
        glutSolidCube(0.04);
        glPopMatrix();
    }else if (side == 1) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(0.08, -0.1, 0.0);  
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0);  
        glutSolidCube(0.04);
        glColor3f(1.0, 1.0, 0.0);
        glTranslatef(0, -0.04, 0.0); 
        glutSolidCube(0.04);
        glPopMatrix();
    }

    glPopMatrix();
}

// same for arm animation
void drawLeg(int side) {
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);  
    GLfloat angle = (side == 1) ? sin(glutGet(GLUT_ELAPSED_TIME) * 0.001) * 15.0 : -sin(glutGet(GLUT_ELAPSED_TIME) * 0.001) * 15.0;
    glRotatef(angle, 0.1, 0.0, 0.0);

    if (side == 0) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(-0.03, -0.26, 0.0);  
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0); 
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0);  
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0); 
        glutSolidCube(0.04);
        glPopMatrix();
    }
    else if (side == 1) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(0.03, -0.26, 0.0);  
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0);  
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0); 
        glutSolidCube(0.04);
        glTranslatef(0, -0.04, 0.0); 
        glutSolidCube(0.04);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawMan(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);  
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);  
    glutSolidCube(0.08);
    glPopMatrix();
   
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0); 
    glTranslatef(0.0, -0.1, 0.0);  
    glutSolidCube(0.1);
    glTranslatef(0.0, -0.1, 0.0);  
    glutSolidCube(0.1);
    glPopMatrix();

    drawArm(0);
    drawArm(1);
    drawLeg(0);
    drawLeg(1);

    glPopMatrix();
}


// draw a car
void drawCar(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y+0.13, z); 
    glScalef(0.4, 0.4, 0.4);
      glPushMatrix();
        glColor3f(0.3, 0.1, 0.9);
        glTranslatef(0.0, 0.4, 0);
        glutSolidCube(1.0);

        glTranslatef(1.0, 0, 0);
        glutSolidCube(1.0);

        glTranslatef(0.0, 0.7, 0.0);
        glutSolidCube(1.0);

        glTranslatef(1.0, -0.7, 0.0);
        glutSolidCube(1.0);
      glPopMatrix();

      glPushMatrix();
      glColor3f(0.8, 0.8, 0.9);
      glTranslatef(0.55, 0.9, 0);
      glRotatef(45, 0, 0, 1);
      glutSolidCube(0.99);
      glPopMatrix();

      glPushMatrix();
      glColor3f(0.8, 0.8, 0.9);
      glTranslatef(1.55, 0.9, 0);
      glRotatef(135, 0, 0, 1);
      glutSolidCube(0.99);
      glPopMatrix();

     glPushMatrix();
     glColor3f(0.0, 0.0, 0.0);
     glTranslatef(0, 0, 0.5);
     glutSolidCylinder(0.4,0.05,100,100);
     glTranslatef(2, 0, 0);
     glutSolidCylinder(0.4, 0.05, 100, 100);
     glPopMatrix();

     glPushMatrix();
     glColor3f(0.0, 0.0, 0.0);
     glTranslatef(0, 0, -0.55);
     glutSolidCylinder(0.4, 0.05, 100, 100);
     glTranslatef(2, 0, 0);
     glutSolidCylinder(0.4, 0.05, 100, 100);
     glPopMatrix();
    glPopMatrix();

}


//draw a Trapezoid which is used for CB 
void drawTrapezoid(GLfloat topWidth, GLfloat bottomWidth, GLfloat height, GLfloat depth) {
    glTranslatef(0.0f,-1.0f,0.0f);
    glPushMatrix();
    glutSolidCube(topWidth);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(topWidth/2,-topWidth*0.2f, 0.0f);
    glRotatef(135.0f, 0.0f, 0.0f, 1.0f);  
    glutSolidCube(topWidth);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-topWidth / 2, -topWidth * 0.2f, 0.0f);
    glRotatef(135.0f, 0.0f, 0.0f, 1.0f);  
    glutSolidCube(topWidth);
    glPopMatrix();

}

// draw a half circle which is used for school slogan 
void drawHalfCircle(float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);

    
    glVertex3f(0.0f, 0.0f, 0.0f);

    
    for (int i = 0; i <= segments; ++i) {
        float angle = i * PI / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }

    glEnd();
}

// draw a flag with animation
void draw1Flag(int length)
{
    glPushMatrix();
    //glDisable(GL_LIGHT0);
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(0.06, 0.06, 0.06);
    glPushMatrix();
    glTranslatef(0.0, -10.0, 0.0);
    glScalef(1.0, 30.0, 1.0);
    glColor3f(0,0,0);
    glutSolidCube(1);
    glPopMatrix();
    int x1, y1;
    glTranslatef(0.0, -1.0, 0.0);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(-1.0, -1.0, -1.0);
    for (float i = 0; i <= 0 + length; i = i + 0.05)
    {
        x1 = i;
        y1 = sin((i + flagStep)/2.0) + 1.0;
        glColor3f(0.0 / 255, 94.0 / 255, 255.0 / 255);
        glVertex2f(x1, y1);
        glColor3f(44.0 / 255, 228.0 / 255, 214.0 / 255);
        glVertex2f(x1, y1+5);
    }
    glEnd();
    //glEnable(GL_LIGHT0);
    glPopMatrix();

}

//draw school`s logo
void drawLogo() {
    //logo
    glPushMatrix();
    glTranslatef(-2.0f, 2.1f, 2.41f);
    glColor3f(0.09, 0.36, 0.66);
    glRotatef(180, 0, 0, 1);
    drawHalfCircle(0.2f, 100);
    glTranslatef(-0.2f, -0.2f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(0.4f, 0.2f, 0.0f);
    glVertex3f(0.4f, 0.0f, 0.0f);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-0.8f, 0.04f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glVertex3f(0.7f, 0.1f, 0.0f);
    glVertex3f(0.7f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();
}


//draw roads. where direct==0 The road runs along the x-axis. 1 runs along the z-axis
void drawRoad(int direct) {
    //x-axis
    if (direct == 0) {
        glPushMatrix();
        //left shoulder
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glVertex3f(0.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -0.1f);
        glVertex3f(4.0f, 0.0f, -0.1f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -0.1f);
        glVertex3f(0.0f, 0.1f, -0.1f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glVertex3f(4.0f, 0.0f, -0.1f);
        glVertex3f(4.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, 0.0f);
        glEnd();
        //front
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glVertex3f(4.0f, 0.1f, 0.0f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glEnd();
        //back
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, -0.1f);
        glVertex3f(0.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.0f, -0.1f);
        glEnd();
        glPopMatrix();

        //right shoulder
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glVertex3f(0.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, 0.0f);
        glEnd();
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -0.1f);
        glVertex3f(4.0f, 0.0f, -0.1f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glEnd();
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -0.1f);
        glVertex3f(0.0f, 0.1f, -0.1f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glEnd();
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glVertex3f(4.0f, 0.0f, -0.1f);
        glVertex3f(4.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, 0.0f);
        glEnd();
        //front
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glVertex3f(4.0f, 0.1f, 0.0f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glEnd();
        //back
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, -0.1f);
        glVertex3f(0.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.1f, -0.1f);
        glVertex3f(4.0f, 0.0f, -0.1f);
        glEnd();
        glPopMatrix();

        //road
        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.8f);
        glVertex3f(4.0f, 0.0f, 0.8f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.81f, 0.32f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.1f, 0.01f, 0.30f);
        glVertex3f(0.1f, 0.01f, 0.40f);
        glVertex3f(0.55f, 0.01f, 0.40f);
        glVertex3f(0.55f, 0.01f, 0.30f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.81f, 0.32f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.75f, 0.01f, 0.30f);
        glVertex3f(0.75f, 0.01f, 0.40f);
        glVertex3f(1.25f, 0.01f, 0.40f);
        glVertex3f(1.25f, 0.01f, 0.30f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.81f, 0.32f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.45f, 0.01f, 0.30f);
        glVertex3f(1.45f, 0.01f, 0.40f);
        glVertex3f(1.95f, 0.01f, 0.40f);
        glVertex3f(1.95f, 0.01f, 0.30f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.81f, 0.32f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(2.15f, 0.01f, 0.30f);
        glVertex3f(2.15f, 0.01f, 0.40f);
        glVertex3f(2.65f, 0.01f, 0.40f);
        glVertex3f(2.65f, 0.01f, 0.30f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.81f, 0.32f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(2.85f, 0.01f, 0.30f);
        glVertex3f(2.85f, 0.01f, 0.40f);
        glVertex3f(3.35f, 0.01f, 0.40f);
        glVertex3f(3.35f, 0.01f, 0.30f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.81f, 0.32f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(3.55f, 0.01f, 0.30f);
        glVertex3f(3.55f, 0.01f, 0.40f);
        glVertex3f(3.85f, 0.01f, 0.40f);
        glVertex3f(3.85f, 0.01f, 0.30f);
        glEnd();
    }
else {
    // z-axis
    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glVertex3f(0.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(4.0f, 0.0f, -0.1f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(0.0f, 0.1f, -0.1f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glVertex3f(4.0f, 0.0f, -0.1f);
    glVertex3f(4.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glVertex3f(4.0f, 0.1f, 0.0f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(0.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.0f, -0.1f);
    glEnd();
    glPopMatrix();

     
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glVertex3f(0.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(4.0f, 0.0f, -0.1f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(0.0f, 0.1f, -0.1f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glVertex3f(4.0f, 0.0f, -0.1f);
    glVertex3f(4.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, 0.0f);
    glEnd();
     
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glVertex3f(4.0f, 0.1f, 0.0f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();
      
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(0.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.1f, -0.1f);
    glVertex3f(4.0f, 0.0f, -0.1f);
    glEnd();
    glPopMatrix();

     
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.8f);
    glVertex3f(4.0f, 0.0f, 0.8f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.81f, 0.32f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.1f, 0.01f, 0.30f);
    glVertex3f(0.1f, 0.01f, 0.40f);
    glVertex3f(0.55f, 0.01f, 0.40f);
    glVertex3f(0.55f, 0.01f, 0.30f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.81f, 0.32f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.75f, 0.01f, 0.30f);
    glVertex3f(0.75f, 0.01f, 0.40f);
    glVertex3f(1.25f, 0.01f, 0.40f);
    glVertex3f(1.25f, 0.01f, 0.30f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.81f, 0.32f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.45f, 0.01f, 0.30f);
    glVertex3f(1.45f, 0.01f, 0.40f);
    glVertex3f(1.95f, 0.01f, 0.40f);
    glVertex3f(1.95f, 0.01f, 0.30f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.81f, 0.32f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(2.15f, 0.01f, 0.30f);
    glVertex3f(2.15f, 0.01f, 0.40f);
    glVertex3f(2.65f, 0.01f, 0.40f);
    glVertex3f(2.65f, 0.01f, 0.30f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.81f, 0.32f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(2.85f, 0.01f, 0.30f);
    glVertex3f(2.85f, 0.01f, 0.40f);
    glVertex3f(3.35f, 0.01f, 0.40f);
    glVertex3f(3.35f, 0.01f, 0.30f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.81f, 0.32f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(3.55f, 0.01f, 0.30f);
    glVertex3f(3.55f, 0.01f, 0.40f);
    glVertex3f(3.85f, 0.01f, 0.40f);
    glVertex3f(3.85f, 0.01f, 0.30f);
    glEnd();
    glPopMatrix();

    }
   
}


// draw the liverpool pavilion 
void drawPavilion() {
    glColor3f(1, 1, 1);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glutSolidCylinder(0.7, 0.3, 100, 100);
    glTranslatef(0, 0, -1.0);
    glutSolidCylinder(0.7, 0.1, 100, 100);
    glTranslatef(0, 0, -1.0);
    glTranslatef(0, 0, 1.0);
    glutSolidCylinder(0.5, 1.5, 100, 100);
    glTranslatef(0, 0, 0.0);
    glScalef(1.0, 1.0, 0.83);
    glutSolidSphere(0.6, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    
    for (int i = 0; i <= 8; i++) {
        glPushMatrix();
        glRotatef(40*i, 0, 0, 1);
        glTranslatef(0.63, 0, -1.0);
        glutSolidCylinder(0.05, 1.5, 100, 100);
        glPopMatrix();
    }
    glPopMatrix();

}

// draw center building
void drawCB() {
    glPushMatrix();
        drawTrapezoid(6.0f, 1.5f, 1.0f, 1.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);   
        glTranslatef(0.0f, 2.0f, 0.0f);
        drawTrapezoid(4.0f, 1.5f, 1.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.5f, 0.65f, 0.82f);
        glTranslatef(0.0f, 4.0f, 0.0f);
        glutSolidCube(4.5f);//glass
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.4f, 0.55f, 0.72f);
        glTranslatef(0.0f, 4.0f, 0.0f);
        glutWireCube(4.51f);//glass
    glPopMatrix();

    //wire xy
    for (int i = 0; i <= 10; i++) {
        glPushMatrix();
        
        glColor3f(0.3f, 0.45f, 0.62f);
        glTranslatef(0.0f, 4.0f, -2.25f);
        glTranslatef(0.0f, 0.0f, i*0.45f);
        glBegin(GL_LINE_LOOP);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-2.26, -2.26);
        glVertex2f(2.26, -2.26);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex2f(2.26, 2.26);
        glVertex2f(-2.26, 2.26);
        glEnd();
        glPopMatrix();
    }
    //wire xz
    for (int i = 0; i <= 10; i++) {
        glPushMatrix();
        glColor3f(0.3f, 0.45f, 0.62f);
        glTranslatef(0.0f, 1.35f + i * 0.45f, 0.0f); 
        glBegin(GL_LINE_LOOP);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-2.26, 0.0, -2.26);  // ÐÞ¸Äy×ø±êÎª0.0
        glVertex3f(2.26, 0.0, -2.26);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(2.26, 0.0, 2.26);
        glVertex3f(-2.26, 0.0, 2.26);
        glEnd();
        glPopMatrix();
    }

    glPushMatrix();
        glColor3f(0.78f,0.48,0.52);
        glTranslatef(0.0f, 4.0f, 0.0f);
        glPushMatrix();
            glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);  
            glVertex3f(-2.4f, -0.53f, 2.4f);
            glVertex3f(-2.4f, -2.5f, 2.4f);
            glVertex3f(2.4f, -2.5f, 2.4f);
            glVertex3f(2.4f, -1.4f, 2.4f);
            glEnd();
            glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);  
            glVertex3f(2.4f, -1.4f, 2.4f);
            glVertex3f(2.4f, -2.5f, 2.4f);
            glVertex3f(2.4f, -2.5f, -2.4f);
            glVertex3f(2.4f, -0.6f, -2.4f);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);  
            glVertex3f(-2.4f, 2.5f, 2.4f);
            glVertex3f(-2.4f, -0.1f, 2.4f);
            glVertex3f(-0.3f, -0.1f, 2.4f);
            glVertex3f(-0.3f, 2.5f, 2.4f);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);  
            glVertex3f(2.5f, 2.5f, 2.4f);
            glVertex3f(2.5f, -0.5f, 2.4f);
            glVertex3f(2.5f, -0.5f, 0.1f); 
            glVertex3f(2.5f, 2.5f, 0.6f);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(2.5f, -0.5f, 0.1f);
            glVertex3f(2.5f, 1.4f, -1.0f);
            glVertex3f(2.5f, 1.4f, 0.9f);
            glEnd();
            glBegin(GL_QUADS);
            glVertex3f(2.5f, 2.5f, -0.1f);
            glVertex3f(2.5f, 2.5f, -2.4f);
            glVertex3f(2.5f, 0.4f, -2.4f); 
            glVertex3f(2.5f, 0.4f, -0.1f);
            glEnd();
            glBegin(GL_QUADS);
            glVertex3f(2.5f, 2.5f, 0.0f);
            glVertex3f(2.5f, 2.5f, -2.4f);
            glVertex3f(2.5f, -0.5f, 0.1f); 
            glVertex3f(2.5f, -0.5f, 0.6f);
            glEnd();
            glBegin(GL_QUADS);
            glVertex3f(2.5f, 1.0f, -1.1f);
            glVertex3f(2.5f, 1.0f, -2.4f);
            glVertex3f(2.5f, -0.2f, -2.4f);  
            glVertex3f(2.5f, -0.2f, -1.8f);
            glEnd();
            glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glVertex3f(2.5f, 2.5f, -2.4f);
            glVertex3f(2.5f, -0.2f, -2.4f);
            glVertex3f(0.2f, -0.2f, -2.4f);  
            glVertex3f(0.2f, 2.5f, -2.4f);
            glEnd();

            glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glVertex3f(-2.4f, 2.5f, 2.4f);
            glVertex3f(-2.4f, -0.1f, 2.4f);
            glVertex3f(-2.4f, -0.1f, 0.4f);  
            glVertex3f(-2.4f, 2.5f, 0.4f);
            glEnd();

            glBegin(GL_QUADS);
            glVertex3f(-2.4f, 2.0f, 2.4f);
            glVertex3f(-2.4f, -0.1f, 2.4f);
            glVertex3f(-2.4f, -0.1f, 0.4f);  
            glVertex3f(-2.4f, 2.0f, 0.4f);
            glEnd();

            glBegin(GL_QUADS);
            glVertex3f(-2.4f, 2.0f, 1.4f);
            glVertex3f(-2.4f, -0.8f, 1.4f);
            glVertex3f(-2.4f, -0.8f, -1.4f);  
            glVertex3f(-2.4f, 2.0f, -1.4f);
            glEnd();

            glPushMatrix();
                glTranslatef(0.0f, 0.0f, -2.5f);
                glBegin(GL_QUADS);
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-2.4f, 2.5f, 2.4f);
                glVertex3f(-2.4f, -0.1f, 2.8f);
                glVertex3f(-2.4f, -0.2f, 0.0f);  
                glVertex3f(-2.4f, 2.5f, 0.0);
                glEnd();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-2.83f, 0.0f, 0.0f);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glBegin(GL_QUADS);
                glVertex3f(2.5f, 2.5f, -2.4f);
                glVertex3f(2.5f, -0.2f, -2.4f);
                glVertex3f(0.5f, -0.2f, -2.4f);  
                glVertex3f(0.5f, 2.5f, -2.4f);
                glEnd();
                glBegin(GL_QUADS);
                glVertex3f(2.5f, -0.2f, -2.4f);
                glVertex3f(2.0f, -2.5f, -2.4f);
                glVertex3f(1.0f, -2.5f, -2.4f);  
                glVertex3f(1.0f, -0.2f, -2.4f);
                glEnd();
                glBegin(GL_QUADS);
                glVertex3f(2.0f, -0.57f, -2.4f);
                glVertex3f(2.0f, -2.5f, -2.4f);
                glVertex3f(0.4f, -2.5f, -2.4f);  
                glVertex3f(0.4f, -0.57f, -2.4f);
                glEnd();
            glPopMatrix();

            glNormal3f(0.0f, 0.0f, -1.0f);
            glBegin(GL_QUADS);
            glVertex3f(2.4f, -0.6f, -2.4f);
            glVertex3f(2.4f, -2.5f, -2.4f);
            glVertex3f(0.2f, -2.5f, -2.4f);  
            glVertex3f(0.2f, -0.6f, -2.4f);
            glEnd();

        glPopMatrix();

        glPushMatrix();
            glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);  
            glVertex3f(0.3f, -0.5f, 2.4f);
            glVertex3f(0.1f, 2.5f, 2.4f);
            glVertex3f(2.5f, 2.5f, 2.4f);
            glVertex3f(2.5f, -0.5f, 2.4f);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);  
            glVertex3f(-2.4f, -0.53f, 2.4f);
            glVertex3f(-2.4f, -2.5f, 2.4f);
            glVertex3f(-2.4f, -2.5f, -2.4f);
            glVertex3f(-2.4f, -0.53f, -2.4f);
            glEnd();
        glPopMatrix();


        //roof
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);  
        glVertex3f(-2.4f, 2.5f, 2.4f);
        glVertex3f(-2.4f, 2.5f, 0.35f);
        glVertex3f(-0.3f, 2.5f, 0.35f);
        glVertex3f(-0.3f, 2.5f, 2.4f);
        glEnd();

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -2.5f);
        glBegin(GL_QUADS);
        glVertex3f(-2.4f, 2.5f, 2.4f);
        glVertex3f(-2.4f, 2.5f, 0.1f);
        glVertex3f(-0.3f, 2.5f, 0.1f);
        glVertex3f(-0.3f, 2.5f, 2.4f);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.5f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex3f(-2.4f, 2.5f, 2.4f);
        glVertex3f(-2.4f, 2.5f, 0.45f);
        glVertex3f(-0.0f, 2.5f, 0.45f);
        glVertex3f(-0.0f, 2.5f, 2.4f);
        glEnd();
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -2.5f);
        glBegin(GL_QUADS);
        glVertex3f(-2.4f, 2.5f, 2.4f);
        glVertex3f(-2.4f, 2.5f, 0.1f);
        glVertex3f(-0.0f, 2.5f, 0.1f);
        glVertex3f(-0.0f, 2.5f, 2.4f);
        glEnd();
        glPopMatrix();
        glPopMatrix();

        //front logo
        glPushMatrix();
        glNormal3f(0.0f, 0.0f, 1.0f);
        drawLogo();
        glTranslatef(0.0f, 0.0f, 0.1f);
        drawLogo();
        glPopMatrix();

        //back logo
        glPushMatrix();
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTranslatef(0.0f, 0.0f, -4.9f);
        drawLogo();
        glTranslatef(0.0f, 0.0f, -0.1f);
        drawLogo();
        glPopMatrix();

        //logo on the  left
        glPushMatrix();
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glRotatef(90,0,1,0);
        glTranslatef(0.0f, 0.0f, -5.0f);
        drawLogo();
        glPopMatrix();
           

    glPopMatrix();

}
//draw foundation building 
void drawFB() {
    glPushMatrix();
    //main structure
    glEnable(GL_NORMALIZE);
        glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glScalef(0.7, 1.0, 3.0);
            glutSolidCube(4.0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0, 1.0, 0.0);
            glColor3f(1.0, 1.0, 1.0);
            glScalef(0.7, 1.0, 1.0);
            glutSolidCube(4.0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0, 3.1, 0.0);
            glColor3f(0.3, 0.3, 0.3);
            glScalef(0.7, 0.05, 1.0);
            glutSolidCube(4.0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.0, -1.0, 0.0);
            glColor3f(1.0, 1.0, 1.0);
            glScalef(1.5, 1.0, 1.0);
            glutSolidCube(3.0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.0, 0.55, 0.0);
            glColor3f(0.3, 0.3, 0.3);
            glScalef(1.5, 0.05, 1.0);
            glutSolidCube(3.0);
        glPopMatrix();
        //wings
        glPushMatrix();
            glTranslatef(3.0, -0.1, 5);
            glColor3f(1.0, 1.0, 1.0);
            glScalef(2.5, 2.7, 1.0);
            glutSolidCube(1.5);
        glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0,0.0,-10);
                glPushMatrix();
                glTranslatef(3.0, -0.1, 5);
                glColor3f(1.0, 1.0, 1.0);
                glScalef(2.5, 2.7, 1.0);
                glutSolidCube(1.5);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(5.0, -0.1, 6);
            glColor3f(1.0, 1.0, 1.0);
            glScalef(1.0, 2.7, 2.5);
            glutSolidCube(1.5);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(5.0, -0.1, -6);
            glColor3f(1.0, 1.0, 1.0);
            glScalef(1.0, 2.7, 2.5);
            glutSolidCube(1.5);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(1.0, -2.1, 0);
        glColor3f(1.0, 1.0, 1.0);
        glRotatef(135, 0, 0, 1);
        glScalef(1.0, 1.0, 2.5);
        glutSolidCube(2);
        glPopMatrix();

        //slogan
        glPushMatrix();
        glScalef(1.2, 1.2, 1.2);
        glPushMatrix();
        glTranslatef(1.2, 2.2, 0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(0.09, 0.36, 0.66);
        glRotatef(180, 0, 0, 1);
        drawHalfCircle(0.2f, 100);
        glTranslatef(-0.2f, -0.2f, 0.0f);
        glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.2f, 0.0f);
        glVertex3f(0.4f, 0.2f, 0.0f);
        glVertex3f(0.4f, 0.0f, 0.0f);
        glEnd();
        glPopMatrix();
        glPopMatrix();
        glDisable(GL_NORMALIZE); //to make sure the light is not effected by scale function


        //Texture
        glPushMatrix();
            //front
            glColor3f(1.0f, 1.0f, 1.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTranslatef(0, 0, 5.01);
            glDisable(GL_COLOR_MATERIAL);
            glEnable(GL_TEXTURE_2D);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth2, imageheight2, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[2]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 2.0f, 1.0f);
            glTexCoord2f(2, 0.0);glVertex3f(1.1f, 2.0f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(1.1f, -2.0f, 1.0f);
            glTexCoord2f(0.0, 2.0);glVertex3f(-1.1f, -2.0f, 1.0f);
            glEnd();
            glTranslatef(3.0, -0.1, -0.2);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 1.9f, 1.0f);
            glTexCoord2f(0.0, 2);glVertex3f(1.1f, 1.9f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(1.1f, -1.9f, 1.0f);
            glTexCoord2f(2, 0.0);glVertex3f(-1.1f, -1.9f, 1.0f);
            glEnd();
            glTranslatef(0.0, 0.0, -1.6);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 1.9f, 1.0f);
            glTexCoord2f(0.0, 2);glVertex3f(1.1f, 1.9f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(1.1f, -1.9f, 1.0f);
            glTexCoord2f(2, 0.0);glVertex3f(-1.1f, -1.9f, 1.0f);
            glEnd();
            glTranslatef(0.0, 0.0, 1.6);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTranslatef(2.0, 0.1, 2.09);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-0.7f, 1.9f, 1.0f);
            glTexCoord2f(2.0, 0);glVertex3f(0.7f, 1.9f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(0.7f, -1.9f, 1.0f);
            glTexCoord2f(0, 2.0);glVertex3f(-0.7f, -1.9f, 1.0f);
            glEnd();
        glPopMatrix();
        //back
        glPushMatrix();
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTranslatef(0, 0, -7.01);
            glDisable(GL_COLOR_MATERIAL);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 2.0f, 1.0f);
            glTexCoord2f(2, 0.0);glVertex3f(1.1f, 2.0f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(1.1f, -2.0f, 1.0f);
            glTexCoord2f(0.0, 2.0);glVertex3f(-1.1f, -2.0f, 1.0f);
            glEnd();
            glTranslatef(3.0, -0.1, 0.2);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 1.9f, 1.0f);
            glTexCoord2f(0.0, 2);glVertex3f(1.1f, 1.9f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(1.1f, -1.9f, 1.0f);
            glTexCoord2f(2, 0.0);glVertex3f(-1.1f, -1.9f, 1.0f);
            glEnd();
            glTranslatef(0.0, 0.0, 1.6);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 1.9f, 1.0f);
            glTexCoord2f(0.0, 2);glVertex3f(1.1f, 1.9f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(1.1f, -1.9f, 1.0f);
            glTexCoord2f(2, 0.0);glVertex3f(-1.1f, -1.9f, 1.0f);
            glEnd();
            glTranslatef(0.0, 0.0, -1.6);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTranslatef(2.0, 0.1, -2.09);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(-0.7f, 1.9f, 1.0f);
            glTexCoord2f(2.0, 0);glVertex3f(0.7f, 1.9f, 1.0f);
            glTexCoord2f(2, 2);glVertex3f(0.7f, -1.9f, 1.0f);
            glTexCoord2f(0, 2.0);glVertex3f(-0.7f, -1.9f, 1.0f);
            glEnd();
        glPopMatrix();
        //left
        glPushMatrix();
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTranslatef(-2.41, 1.0, 0);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 2.0f, -1.9f);
            glTexCoord2f(1, 0.0);glVertex3f(1.0f, 2.0f, 1.9f);
            glTexCoord2f(1, 1);glVertex3f(1.0f, -2.0f, 1.9f);
            glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, -2.0f, -1.9f);
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTranslatef(-2.41, 0, -4.0);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 1.8f, -1.9f);
            glTexCoord2f(2, 0.0);glVertex3f(1.0f, 1.8f, 1.9f);
            glTexCoord2f(2, 2);glVertex3f(1.0f, -1.8f, 1.9f);
            glTexCoord2f(0.0, 2.0);glVertex3f(1.0f, -1.8f, -1.9f);
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTranslatef(-2.41, 0, 4.0);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 1.8f, -1.9f);
            glTexCoord2f(2, 0.0);glVertex3f(1.0f, 1.8f, 1.9f);
            glTexCoord2f(2, 2);glVertex3f(1.0f, -1.8f, 1.9f);
            glTexCoord2f(0.0, 2.0);glVertex3f(1.0f, -1.8f, -1.9f);
            glEnd();
        glPopMatrix();

        //right
        glPushMatrix();
            glNormal3f(1.0f, 0.0f, 0.0f);
                glTranslatef(0.5, 0, 0.0);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 1.8f, -4.0f);
                glTexCoord2f(3, 0.0);glVertex3f(1.0f, 1.8f, 4.0f);
                glTexCoord2f(3, 3);glVertex3f(1.0f, -1.8f, 4.0f);
                glTexCoord2f(0.0, 3.0);glVertex3f(1.0f, -1.8f, -4.0f);
                glEnd();

                glTranslatef(4.3, 0, -6.0);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 1.8f, -1.5f);
                glTexCoord2f(2, 0.0);glVertex3f(1.0f, 1.8f, 1.5f);
                glTexCoord2f(2, 2);glVertex3f(1.0f, -1.8f, 1.5f);
                glTexCoord2f(0.0, 2.0);glVertex3f(1.0f, -1.8f, -1.5f);
                glEnd();

                glTranslatef(0, 0, 12.0);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 1.8f, -1.5f);
                glTexCoord2f(2, 0.0);glVertex3f(1.0f, 1.8f, 1.5f);
                glTexCoord2f(2, 2);glVertex3f(1.0f, -1.8f, 1.5f);
                glTexCoord2f(0.0, 2.0);glVertex3f(1.0f, -1.8f, -1.5f);
                glEnd();

        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
        glPopMatrix();
        


    glPopMatrix();
}


// draw the single building for North Campus Complex
void drawS() {
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth0, imageheight0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
    // top
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);   
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.1, 0.0); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.1, 0.1); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0, 0.1); glVertex3f(-1.0f, 1.0f, 1.0f);

    glNormal3f(0.0f, -1.0f, 0.0f);   
    glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0, 0.5);glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.1, 0.5);glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.1, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);

    glNormal3f(0.0f, 0.0f, 1.0f);   
    glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 1.1f, 1.0f);
    glTexCoord2f(0.0, 2);glVertex3f(1.1f, 1.1f, 1.0f);
    glTexCoord2f(1, 2);glVertex3f(1.1f, -1.1f, 1.0f);
    glTexCoord2f(1, 0.0);glVertex3f(-1.1f, -1.1f, 1.0f);

    glNormal3f(0.0f, 0.0f, -1.0f);   
    glTexCoord2f(0.0, 0.0);glVertex3f(-1.1f, 1.1f, -1.0f);
    glTexCoord2f(0.0, 2);glVertex3f(1.1f, 1.1f, -1.0f);
    glTexCoord2f(1, 2);;glVertex3f(1.1f, -1.1f, -1.0f);
    glTexCoord2f(1, 0.0);glVertex3f(-1.1f, -1.1f, -1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth1, imageheight1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);   
    glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0, 4);glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1, 4);glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);

    glNormal3f(1.0f, 0.0f, 0.0f);   
    glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0, 4);glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1, 4);glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


//draw the Science building
void drawSBuilding() {
    glPushMatrix();
    //SA
    glTranslatef(0.0, 1.5f, 0.0);
    glColor3f(1,1,1);
    glScalef(1.0f, 1.5f, 3.5f);
    drawS();
    //SB
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, 0.0f);
    drawS();
    glPopMatrix();
    //SC
    glPushMatrix();
    glTranslatef(-8.0f, 0.0f, 0.0f);
    drawS();
    glPopMatrix();
    //SD
    glPushMatrix();
    glTranslatef(-12.0f, 0.0f, 0.0f);
    drawS();
    glPopMatrix();
    glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    //corridors
    glPushMatrix();
    glTranslatef(-2.0f, 0.6f, -2.0f);
    glScalef(2.5f,0.7f,0.6f);
    glColor3f(0.4f, 0.55f, 0.72f);
    glutSolidCube(2.0);
    glTranslatef(0.0, 1.1, 0);
    glScalef(1.0f, 0.1f, 1.0f);
    glColor3f(0.99, 0.55, 0.41);
    glutSolidCube(2.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.0f, 0.6f, -2.0f);
    glScalef(2.5f, 0.7f, 0.6f);
    glColor3f(0.4f, 0.55f, 0.72f);
    glutSolidCube(2.0);
    glTranslatef(0.0, 1.1, 0);
    glScalef(1.0f, 0.1f, 1.0f);
    glColor3f(0.99, 0.55, 0.41);
    glutSolidCube(2.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0f, 0.6f, -2.0f);
    glScalef(2.5f, 0.7f, 0.6f);
    glColor3f(0.4f, 0.55f, 0.72f);
    glutSolidCube(2.0);
    glTranslatef(0.0, 1.1, 0);
    glScalef(1.0f, 0.1f, 1.0f);
    glColor3f(0.99, 0.55, 0.41);
    glutSolidCube(2.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f,0.0f,4.0f);
        glPushMatrix();
        glTranslatef(-2.0f, 0.6f, -2.0f);
        glScalef(2.5f, 0.7f, 0.6f);
        glColor3f(0.4f, 0.55f, 0.72f);
        glutSolidCube(2.0);
        glTranslatef(0.0, 1.1, 0);
        glScalef(1.0f, 0.1f, 1.0f);
        glColor3f(0.99, 0.55, 0.41);
        glutSolidCube(2.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-5.0f, 0.6f, -2.0f);
        glScalef(2.5f, 0.7f, 0.6f);
        glColor3f(0.4f, 0.55f, 0.72f);
        glutSolidCube(2.0);
        glTranslatef(0.0, 1.1, 0);
        glScalef(1.0f, 0.1f, 1.0f);
        glColor3f(0.99, 0.55, 0.41);
        glutSolidCube(2.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-10.0f, 0.6f, -2.0f);
        glScalef(2.5f, 0.7f, 0.6f);
        glColor3f(0.4f, 0.55f, 0.72f);
        glutSolidCube(2.0);
        glTranslatef(0.0, 1.1, 0);
        glScalef(1.0f, 0.1f, 1.0f);
        glColor3f(0.99, 0.55, 0.41);
        glutSolidCube(2.0);
        glPopMatrix();

    glPopMatrix();
    glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);

}

//draw tree. the code comes from lab
void drawTree() {
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(0.6, 0.65, 0.60);
    glColor3f(0.0, 0.7, 0.1);
    glutSolidIcosahedron();
    glColor3f(0.0, 0.0, 0.0);
    glutWireIcosahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(0.15, 0.5, 0.15);
    glColor3f(0.7, 0.3, 0.0);
    glutSolidCube(1);
    glColor3f(0.0, 0.0, 0.0);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();
}

//draw bush. Just a tree without trunk
void drawBush() {
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(0.6, 0.65, 0.60);
    glColor3f(0.0, 0.7, 0.1);
    glutSolidIcosahedron();
    glColor3f(0.0, 0.0, 0.0);
    glutWireIcosahedron();
    glPopMatrix();
}

// display function to combine objects together
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, centerX,centerY,centerZ, 0.0, 1.0, 0.0);  //Setting the viewpoint position and orientation
    
    glTranslatef(cameraX, cameraY, cameraZ);  // Pan to camera position
    glRotatef(angleX, 1.0, 0.0, 0.0);        //rotation 
    glRotatef(angleY, 0.0, 1.0, 0.0);        
    glTranslatef(-cameraX, -cameraY, -cameraZ);  
    
    // light position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    //draw objects
    glEnable(GL_COLOR_MATERIAL);
    
    glColor3f(0.6f, 0.55f, 0.57f);
    drawPlane();

    for (int i = 0; i < 5; ++i) {
        drawMan(manPosition_x[i], manOnGround, manPosition_z[i]);
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCB();

    //draw SB
    glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslatef(6.5f, 0.0f, 12.0f);
    drawSBuilding();
    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);

    //draw PB\MB\EE...
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    //EE
    glTranslatef(-6.0f, 1.5f, 25.0f);
    glScalef(1.0f, 1.5f, 3.5f);
    drawS();
    //corridor between EE and EB
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glTranslatef(0, -0.3, 1.1);
    glScalef(0.3, 2, 0.1);
    glutSolidCube(0.8);
    glTranslatef(0, 0, 8);
    glutSolidCube(0.8);
    glPopMatrix();
    glColor3f(0.4f, 0.55f, 0.72f);
    glTranslatef(0.0f, 0.5f, 1.5f);
    glScalef(0.3, 1.0, 1.0);
    glutSolidCube(1.0);
    glTranslatef(0.0f, 0.51f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(1.0, 0.05, 0.99);
    glutSolidCube(1.0);
    glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //EB
    glTranslatef(0.0f,0.0f, 3.0f);
    drawS();
   
    glTranslatef(5.0f, 0.0f, 0.0f);
    drawS();
    //MB
    glTranslatef(4.0f, 0.0f, 0.0f);
    drawS();
    //MA
    glTranslatef(3.0f, 0.0f, 0.0f);
    drawS();
    //PB
    glPushMatrix();
    glTranslatef(-2.0f, -0.5f, -2.5f);
    glEnable(GL_NORMALIZE);
    glScalef(1.2, 0.3, 0.4);
    drawS();
    glDisable(GL_NORMALIZE);
    glPopMatrix();
    glTranslatef(0.0f, 0.0f, -3.0f);
    drawS();
    glTranslatef(1.0f, 0.0f, 0.0f);
    
    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);

    //draw FB
    glPushMatrix();
    glTranslatef(12.0f, 2.0f, -2.0f);
    drawFB();
    glPopMatrix();

    //draw road
    glPushMatrix();
    glTranslatef(-8.0f,0.01f,4.5f);
    drawRoad(0);
    for (int i = 0; i <= 2; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawRoad(0);
    }
    glTranslatef(4.0f, 0.0f, 0.8f);
    drawRoad(1);
    glTranslatef(0.0f, 0.0f, -4.0f);
    drawRoad(1);
    glTranslatef(0.0f, 0.0f, -4.0f);
    drawRoad(1);
    glTranslatef(0.0f, 0.0f, 12.0f);
    drawRoad(1);
    for (int i = 0; i <= 6; i++) {
        glTranslatef(0.0f, 0.0f, 4.0f);
        drawRoad(1);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8.0f, 0.01f, 6.5f);
    drawRoad(0);
    for (int i = 0; i <= 2; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawRoad(0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8.0f, 0.01f, 17.5f);
    drawRoad(0);
    for (int i = 0; i <= 5; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawRoad(0);
    }
    glTranslatef(-24.0f,0.0f,1.3f);
    drawRoad(0);
    for (int i = 0; i <= 5; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawRoad(0);
    }
    glPopMatrix();

    //road in the middle of EB and MB
    glPushMatrix();
    glTranslatef(1.0f, 0.01f, 35.5f);
    
    drawRoad(1);
    glTranslatef(0.0f, 0.0f, 4.0f);
    drawRoad(1);
    glTranslatef(0.0f, 0.0f, 4.0f);
    drawRoad(1);
    glPopMatrix();

    // draw trees
    glPushMatrix();
    glTranslatef(-7.0f, 0.0f, 6.0f);
    drawTree();
    for (int i = 0; i <= 2; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawTree();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7.0f, 0.0f, 17.0f);
    drawTree();
    for (int i = 0; i <= 2; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawTree();
    }
    glTranslatef(-12.0f, 0.0f, 1.5f);
    drawTree();
    for (int i = 0; i <= 2; i++) {
        glTranslatef(4.0f, 0.0f, 0.0f);
        drawTree();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.8f, 0.0f, 30.0f);
    drawTree();
    glTranslatef(1.5f, 0.0f, 0.0f);
    drawTree();
    glTranslatef(1.5f, 0.0f, 0.0f);
    drawTree();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.0f, 0.2, 29.0f);
    drawPavilion();
    glPopMatrix();


    //drawbush
    glEnable(GL_NORMALIZE);
    glPushMatrix();
    glTranslatef(4.0f, 0.3f, 24.0f);
    glScalef(0.8, 0.8, 0.8);
    drawBush();
    glTranslatef(0.0f, 0.0f, -1.5f);
    glScalef(1.8, 1.8, 1.8);
    drawBush();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-4.0f, 0.3f, 24.0f);
    glScalef(0.8, 0.8, 0.8);
    drawBush();
    glTranslatef(0.0f, 0.0f, -1.5f);
    glScalef(1.8, 1.8, 1.8);
    drawBush();
    glTranslatef(0.0f, 0.0f, 3);
    drawBush();
    glPopMatrix();
    glDisable(GL_NORMALIZE);

    //draw flags
    glPushMatrix();
    glTranslatef(7.1f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    draw1Flag(10);
    glTranslatef(1.0f, 0.0f, 0.0f);
    draw1Flag(10);
    glTranslatef(-2.0f, 0.0f, 0.0f);
    draw1Flag(10);
    glPopMatrix();

    //draw cars
    for (int i = 0; i < 3; ++i) {
        drawCar(carPosition_x[i], 0, carPosition_z[i]);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    
   
    glDisable(GL_COLOR_MATERIAL);

    glutSwapBuffers();   
}

// window reshape
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f); 
    glMatrixMode(GL_MODELVIEW);
}

//ketboard interactions. mainly used for move the position of camera and light source
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        cameraX = 0.0f, cameraY = 3.0f, cameraZ = 25.0f;
        centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
        angleX = 0.0f, angleY = 0.0f;
    case 'w':
        cameraZ -= 0.4f * cos(angleY * PI / 180);
        centerZ -= 0.4f * cos(angleY * PI / 180);

        cameraX += 0.4f * sin(angleY * PI / 180);
        centerX += 0.4f * sin(angleY * PI / 180);

        //printf("cor: (%.2f, %.2f)\n", 0.5f * cos(angleY * PI / 180), 0.5 * sin(angleY * PI / 180));
        break;
    case 's':
        cameraZ += 0.4f * cos(angleY * PI / 180);
        centerZ += 0.4f * cos(angleY * PI / 180);
        cameraX -= 0.4f * sin(angleY * PI / 180);
        centerX -= 0.4f * sin(angleY * PI / 180);
        //printf("cor: (%.2f, %.2f)\n", 0.5f * cos(angleY * PI / 180), 0.5 * sin(angleY * PI / 180));
        break;
    case 'a':
        cameraZ += 0.4f * cos((angleY+90) * PI / 180);
        centerZ += 0.4f * cos((angleY + 90) * PI / 180);
        cameraX -= 0.4f * sin((angleY + 90) * PI / 180);
        centerX -= 0.4f * sin((angleY + 90) * PI / 180);
        break;
    case 'd':
        cameraZ -= 0.4f * cos((angleY + 90) * PI / 180);
        centerZ -= 0.4f * cos((angleY + 90) * PI / 180);
        cameraX += 0.4f * sin((angleY + 90) * PI / 180);
        centerX += 0.4f * sin((angleY + 90) * PI / 180);
        break;
    case 'q':
        if (lightPosition[0] >= 6.2) {
            lightPosition[0] = 6.2;
            break;
        }
        if (lightPosition[2] >= 6.6) {
            lightPosition[2] = 6.6;
            break;
        }
        lightPosition[0]+=0.2;
        lightPosition[2]+=0.2;
        //printf("Light Position: (%.2f, %.2f, %.2f, %.2f)\n", lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);
        break;
    case 'e':
        
        //printf("Light Position: (%.2f, %.2f, %.2f, %.2f)\n", lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);
        if (lightPosition[0] <= -2.2) {
            lightPosition[0] = -2.2;
            break;
        }
        if (lightPosition[2] <= -6.0) {
            lightPosition[2] = -6.0;
            break;
        }
        lightPosition[0] -= 0.2;
        lightPosition[2] -= 0.2;
        break;
    case ' ':
        cameraY += 0.5f;
        centerY += 0.5f;
        break;
    case 'c':
        cameraY -= 0.5f;
        centerY -= 0.5f;
        break;
    case 27:  // ESC
        exit(0);
        break;  
   
    
    }
    glutPostRedisplay();  
}

//mouse interactions. used for rotate the camera
void mouseMotion(int x, int y) {
    if (lastMouseX == -1 || lastMouseY == -1) {
        lastMouseX = x;
        lastMouseY = y;
        return;
    }

    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    angleX += deltaY * 0.1f;
    angleY += deltaX * 0.1f;
   
    lastMouseX = x;
    lastMouseY = y;

    //printf("Angle: (%.2f, %.2f)\n", angleX, angleY);

    glutPostRedisplay();   
}

//mouse button interactions. when the button is down, activate the mouse motion functions.
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
    }
    
}

//main functions.
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("XJTLU SIP north campus");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);
    glutIdleFunc(when_in_mainloop);
    glutTimerFunc(time_interval, manAni, 1);
    glutTimerFunc(time_interval, OnTimerFlag, 2);
    glutTimerFunc(time_interval, carAni, 1);

    glutMainLoop();

    return 0;
}

