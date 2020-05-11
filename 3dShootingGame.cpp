// Anju Puthenveetil
// cs5410 hw3
// date 12/08/2017
#include "RGBpixmap.h"
#include<GLUT/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//#include <gl/glut.h>
RGBpixmap pix;
#define PI 3.14159265
int count1 = 0;
float cannonAngleX = 0;
float cannonAngleY = 0;
float sphereAngleX = 0;
float sphereAngleY = 0;
bool shakeOnspace = 0;
bool startMoving = 0;
bool collisionDetect = 0;
float previousAngle = 0;
float moveOnShake = 2;
float moveSphere = 0;
float angleVariant = 0;
float cannonAngle = 0;
float x,y,z = 0.0;
float displacementV[3][3];
int positionHit[3][3];
int teapotCount = 9;

//<<<<<<<<<<<<<<< wall >>>>>>>>>>>>>>>>
void wallLength(double thickness)
{ // draw thin wall with top = xy-plane, corner at origin
    glPushMatrix();
    glTranslated(1.0, 1.0,0.0);
    glScaled(2.0, 2.0, thickness);
    glutSolidCube(1.0);
    glPopMatrix();
}

GLUquadric* qobj = gluNewQuadric();

//<<<<<<<<<<<<<<< create cylinder >>>>>>>>>>>>>>>>
void createCylinder(){
    glPushMatrix();
    glTranslated(1, 0.08, 2.0);
    glRotated(cannonAngleX,0.0,1.0,0.0);
    glRotated(cannonAngleY,1.0,0.0,0.0);
    gluCylinder(qobj, 0.05, 0.05, 1.0, 30, 30);
    glPopMatrix();
    
}



//<<<<<<<<<<<<<<< move cylinder >>>>>>>>>>>>>>>>

void moveCylinder(float angleX,float angleY){
    if(moveOnShake<2.5){
        glPushMatrix();
        moveOnShake = moveOnShake + 0.005;
        glTranslated(1,0.08,moveOnShake);
        glRotated(angleX,0.0,1.0,0.0);
        glRotated(angleY,1.0,0.0,0.0);
        gluCylinder(qobj, 0.05, 0.05, 1.0, 30, 30);
        glPopMatrix();
        /*}else{
         glPushMatrix();
         moveOnShake = moveOnShake - 0.005;
         glTranslated(1,0.08,moveOnShake);
         glRotated(angleX,0.0,1.0,0.0);
         glRotated(angleY,1.0,0.0,0.0);
         gluCylinder(qobj, 0.05, 0.05, 1.0, 30, 30);
         glPopMatrix();
         
         if(moveOnShake<2){
         
         moveOnShake = 2;
         glPushMatrix();
         glTranslated(1,0.08,moveOnShake);
         glRotated(angleX,0.0,1.0,0.0);
         glRotated(angleY,1.0,0.0,0.0);
         gluCylinder(qobj, 0.05, 0.05, 1.0, 30, 30);
         glPopMatrix();
         
         shakeOnspace = 0; // will not move till next space bar
         
         
         }*/
        
    }else {
        moveOnShake = 2;
        shakeOnspace = 0;
    }
    
}


//<<<<<<<<<<<<<<<<<<<<<<collision detection>>>>>>>>>>>>>>>>>>//

void collisionDetection(float x,float y,float z){
    for(int i = 0; i <= 2;i++){
        for(int j = 0;j<=2;j++){
            if(positionHit[i][j] != 1){
                //check for collision
                int teapotX = displacementV[i][j];
                int teapotY = 0.5*j+0.5;
                int teapotZ = 1;
                //checking if collision occured for each teapot
                float x_square = (x - teapotX)*(x-teapotX);
                float y_square = (y - teapotY)*(y - teapotY);
                float z_square = (z - teapotZ)*(z - teapotZ);
                if(x_square+y_square+z_square <= 0.08*0.08){
                    positionHit[i][j] = 1;
                    teapotCount--;
                    collisionDetect = 1;
                    j = 3; i = 3;
                }}
            
        }
    }
}

//<<<<<<<<< create and move the sphere >>>>>>/

GLUquadric* qobj1 = gluNewQuadric();
void moveSphereFunc(float AngleX,float AngleY){
    if(moveSphere < 2.5){
        glPushMatrix();
        //the initial position where the sphere is created
        float xT = (moveSphere)*sin(sphereAngleX*PI/180);
        float yT = (moveSphere)*cos((90-sphereAngleY)*PI/180);
        
        //translating the sphere according to the angle of the cannon
        
        if(AngleX>=0){
            if(previousAngle >= sphereAngleX){
                glTranslated(1-xT, 0.08+yT, 2.0-moveSphere);
                if(collisionDetect == 0)
                    collisionDetection(1-xT, 0.08+yT, 2.0-moveSphere);
            }else{
                glTranslated(1+xT, 0.08+yT, 2.0-moveSphere);
                if(collisionDetect == 0)
                    collisionDetection(1-xT, 0.08+yT, 2.0-moveSphere);
            }
        }else{
            if(previousAngle <= sphereAngleX){
                glTranslated(1-xT, 0.08+yT, 2.0-moveSphere);
                if(collisionDetect == 0)
                    collisionDetection(1-xT, 0.08+yT, 2.0-moveSphere);
            }else{
                glTranslated(1+xT, 0.08+yT, 2.0-moveSphere);
                if(collisionDetect == 0)
                    collisionDetection(1-xT, 0.08+yT, 2.0-moveSphere);
            }
        }
        previousAngle = AngleX;
        
        gluSphere(qobj1,0.05,100,100);
        glPopMatrix();
        moveSphere = moveSphere + 0.005;
    }
    
}

//<********** print count left ************>
void displayCountLeft( char *s){
    //   while(*s){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*s);
    //  }
}

void myInit(){
    // set the camera and perspective
    //front look
    // glViewport(0, 0, 640, 480);
    glEnable(GL_TEXTURE_2D);
    pix.makeCheckerboard();
    // pix.readBMPFile("wood1.bmp");
    // pix.setTexture(2001);            // create texture
    
    glViewport(-640, -480, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluPerspective(15.7, 64/48, 0.7, 15);
    //gluPerspective(45, 64/48, 0.7, 15);
    
    gluLookAt(1.0, 1.0, 9.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0);
    //set properties of the surface material
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    
    
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    // set the light source properties
    GLfloat lightIntensity[] = { 8.0f, 8.0f, 8.0f, 1.0f };
    GLfloat light_position[] = { 3.0f,1.0f, 4.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
    
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            positionHit[i][j] = 0; // 0 if teapot is not hit, 1 if hit, -1 if finished spinning
        }
    }
    
    
}
void reset(){
    for(int i = 0;i<=2;i++){
        for(int j=0;j<=2;j++){
            positionHit[i][j] = 0;
            teapotCount = 9;
        }
        
        count1 = 0;
        cannonAngleX = 0;
        cannonAngleY = 0;
        sphereAngleX = 0;
        sphereAngleY = 0;
        shakeOnspace = 0;
        startMoving = 0;
        collisionDetect = 0;
        previousAngle = 0;
        moveOnShake = 2;
        moveSphere = 0;
        angleVariant = 0;
        cannonAngle = 0;
        x,y,z = 0.0;
    }
}
//<<<<<<<<<<<<<<<<<<<<<keyboard function>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(int key, int mouseX, int mouseY){
    int incrementAngle = 5;
    switch(key){
        case GLUT_KEY_UP:
            
            
            if(cannonAngleY+incrementAngle<=90){
                cannonAngleY = cannonAngleY+incrementAngle;
            }
            break;
        case GLUT_KEY_DOWN:
            
            if(cannonAngleY-incrementAngle>= 0 ){
                cannonAngleY = cannonAngleY-incrementAngle;
            }
            break;
        case GLUT_KEY_LEFT:
            
            if(cannonAngleX+incrementAngle<63.5){
                cannonAngleX = cannonAngleX+incrementAngle;
            }
            
            break;
        case GLUT_KEY_RIGHT:
            
            if(cannonAngleX-incrementAngle>-63.5){
                cannonAngleX = cannonAngleX-incrementAngle;
            }
            break;
            
            
        case 32:
            //make a moving sphere and move the cannon
            shakeOnspace = 1;
            startMoving = 1;
            
            break;
        case 121://y
            if(teapotCount == 0){
                reset();
            }
            break;
        case 89://Y
            if(teapotCount == 0){
                reset();
            }
            break;
        case 78: // N
            if(teapotCount == 0){
                exit(0);
            }
            
        case 110:
            //n
            if(teapotCount == 0){
                exit(0);
            }
            
            
    }
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
    
    
}


//<<<<<<<<<<display text>>>>>>>>>

void outputText(string S){
    
    
    glRasterPos3f(1.0f, 1.7f,1.0f);
    
    char* c = new char[S.length() + 1];
    strcpy(c, S.c_str());
    while(*c != '\0')
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        *c++;
    }
}

//<<<<<<<<<<<<<<<<<<<<< displaySolid >>>>>>>>>>>>>>>>>>>>>>
void displaySolid(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    // clear the screen
    mat_diffuse[0] = 0.4f, mat_diffuse[1] = 0.7f, mat_diffuse[2] = 0.2f, mat_diffuse[3] = 0.0f;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    wallLength(0.08);                  // wall #1: in xy-plane
    mat_diffuse[0] = 0.5f, mat_diffuse[1] = 0.2f, mat_diffuse[2] = 0.3f, mat_diffuse[3] = 0.0f;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    glPushMatrix();                     // wall #2: in xz-plane
    glRotated(90.0,1.0,0.0 ,0.0);
    wallLength(0.08);
    glTranslated(0.0, 0.0, -2.0);       // wall #3: in xy-plane
    wallLength(0.08);
    glPopMatrix();
    
    glPushMatrix();
    glRotated(-90.0,0.0,1.0 ,0.0);      // wall #4: in yz-plane
    wallLength(0.08);
    glTranslated(0.0, 0.0, -2.0);
    wallLength(0.08);                   // wall #5: in yz-plane
    glPopMatrix();
    
    mat_diffuse[0] = 0.5f, mat_diffuse[1] = 0.5f, mat_diffuse[2] = 0.2f, mat_diffuse[3] = 0.0f;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    
    float stepSize = 0.0048;
    float step;
    float reposition = -0.08f;
    // //displaying the count on screen and text on Screen
    
    mat_diffuse[0] = 1.0f, mat_diffuse[1] = 1.0f, mat_diffuse[2] = 1.0f, mat_diffuse[3] = 1.0f;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    if(teapotCount>0){
        std::string s= std::to_string(teapotCount);
        outputText(s);
    }else{
        outputText("Continue: Y - N");
    }
    
    
    
    mat_diffuse[0] = 0.4f, mat_diffuse[1] = 0.2f, mat_diffuse[2] = 0.2f, mat_diffuse[3] = 0.0f;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    
    //Creating teapots and moving them as per the requirements
    for (int i = 0; i<3;i++)
    {
        for (int j = 0;j<3;j++)
        {
            mat_diffuse[0] = 0.5f, mat_diffuse[1] = 0.5f, mat_diffuse[2] = 0.5f, mat_diffuse[3] = 0.0f;
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            
            
            
            //create movement on cannon
            if(shakeOnspace == 1){
                moveCylinder(cannonAngleX, cannonAngleY);
                sphereAngleX = cannonAngleX;
                sphereAngleY = cannonAngleY;
                
            }else{
                createCylinder();
                
            }
            mat_diffuse[0] = 0.1f, mat_diffuse[1] = 0.5f, mat_diffuse[2] = 0.5f, mat_diffuse[3] = 0.0f;
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            //make a moving sphere
            if (startMoving == 1 && moveSphere < 2.5){
                moveSphereFunc(sphereAngleX,sphereAngleY);
                
            }else{
                moveSphere = 0;
                startMoving = 0;
                collisionDetect = 0;
            }
            
            
            
            //set colour for each teapot
            mat_diffuse[0] = i, mat_diffuse[1] = j, mat_diffuse[2] = 1.0f, mat_diffuse[3] = 0.0f;
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            
            //begin tea pot drawing
            glPushMatrix();
            if(count1== 0) {    //  setting initial position of teapots
                displacementV[i][j] = 0.5*i+0.5;
            }
            //setting values for direction of teapots
            if(j==1){
                step = - 1*stepSize;
                reposition = 2.08f;
            }else {
                step =  stepSize;
                reposition = -0.08f;
            }
            
            displacementV[i][j] = displacementV[i][j] + step;
            glTranslated(displacementV[i][j], 0.5*j + 0.5, 1.0);
            
            glRotated(20* angleVariant + count1,0,1.0,0); // rotate teapots
            if(positionHit[i][j] == 0){
                
                glutSolidTeapot(0.08);        // draw the teapot
            }
            if( displacementV[i][j] >= 2 && j!=1){
                displacementV[i][j] = reposition;
            }
            if( displacementV[i][j] <= 0 && j==1){
                displacementV[i][j] = reposition;
            }
            
            
            angleVariant =  angleVariant+0.005;
            glPopMatrix();
            
            
        }
    }
    count1++;
    
    
    //glBindTexture(GL_TEXTURE_2D, 2001);
    //glPushMatrix();
    //glBegin(GL_QUADS);
    //glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);
    //glTexCoord2f(0.0, 1.0); glVertex3f(2.0f, 0.0f, 0.0f);
    //glTexCoord2f(1.0, 0.0); glVertex3f( 2.0f, 2.0f, 0.0f);
    //glTexCoord2f(1.0, 1.0); glVertex3f( 0.0f, 2.0f, 0.0f);
    //glEnd();
    // glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
    
    
    
}
//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Teapot Game");
    myInit();
    glutDisplayFunc(displaySolid);
    glutSpecialFunc(myKeyboard);
    glEnable(GL_LIGHTING); // enable the light source
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); // for hidden surface removal
    glEnable(GL_NORMALIZE); // normalize vectors for proper shading
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);  // background is light gray
    glutMainLoop();
    return 0;
}









