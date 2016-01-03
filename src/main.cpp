#include <iostream>
#include <math.h>

#include "glHelper.h"
#include "GL_Object.h"

using namespace std;
using namespace pi;

#define Solid 1
#define Line 2
#define Point 3
#define DisAssembly 4
#define Assembly 5

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLfloat zPin = 0.0f;
static GLfloat zBlock = 0.0f;

int	   MouseStartX;
int    MouseStartY;
int	   MouseX;
int    MouseY;

Scence scence;

void CalculateNormal(GLfloat *dVertex1, GLfloat *dVertex2, GLfloat *dVertex3, GLfloat *dNormal)
{
    GLfloat dVector1[3],dVector2[3];
    dVector1[0]=dVertex2[0]-dVertex1[0];
    dVector1[1]=dVertex2[1]-dVertex1[1];
    dVector1[2]=dVertex2[2]-dVertex1[2];
    dVector2[0]=dVertex3[0]-dVertex1[0];
    dVector2[1]=dVertex3[1]-dVertex1[1];
    dVector2[2]=dVertex3[2]-dVertex1[2];

    dNormal[0]=dVector1[1]*dVector2[2]-dVector1[2]*dVector2[1];
    dNormal[1]=dVector1[2]*dVector2[0]-dVector1[0]*dVector2[2];
    dNormal[2]=dVector1[0]*dVector2[1]-dVector1[1]*dVector2[0];

    GLfloat dNormalLength=sqrt(dNormal[0]*dNormal[0]+dNormal[1]*dNormal[1]+dNormal[2]*dNormal[2]);
    if (dNormalLength!=0.0)
    {
        dNormal[0]=dNormal[0]/dNormalLength;
        dNormal[1]=dNormal[1]/dNormalLength;
        dNormal[2]=dNormal[2]/dNormalLength;
    }
    else
    {
        dNormal[0]=0.0;
        dNormal[1]=0.0;
        dNormal[2]=1.0;
    }

}

void TimerFunc(int value)
{
    glutPostRedisplay();
    glutTimerFunc(100, TimerFunc, 1);
}

//Menu Callback function
void ProcessMenu(int value)
{
    switch(value)
    {
    case 1:
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Rotate about x and y axes
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 0.0f, 1.0f);

    // Render just the Thread of the nut

    glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-45.0f,0.0f,0.0f,1.0f);

    scence.draw();
    // Swap buffers
    glutSwapBuffers();
}

void SetupRC()
{
    GLfloat  ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat  diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat  specular[] = { 0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat  lightPos[] = { -50.0f, 200.0f, 200.0f, 1.0f};
    GLfloat  specref[] =  { 0.6f, 0.6f, 0.6f, 1.0f};

    glEnable(GL_DEPTH_TEST);    // Hidden surface removal
    glEnable(GL_CULL_FACE);     // Do not calculate inside of solid object
    glFrontFace(GL_CCW);
    
    // Enable lighting
    glEnable(GL_LIGHTING);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);

    // Position and turn on the light
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a moderate shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,64);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

///////////////////////////////////////////////////////////////////////////////
// Process arrow keys
void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        xRot-= 5.0f;

    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;

    if(key > 356.0f)
        xRot = 0.0f;

    if(key < -1.0f)
        xRot = 355.0f;

    if(key > 356.0f)
        yRot = 0.0f;

    if(key < -1.0f)
        yRot = 355.0f;

    // Refresh the Window
    glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
    GLfloat nRange = 100.0f;

    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h)
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange*2.0f, nRange*2.0f);
    else
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange*2.0f, nRange*2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Motion(int x,int y)
{
    MouseX = x;
    MouseY = y;
}

void Idle(void)
{
    yRot+=0.05*(MouseX-MouseStartX);
    xRot+=0.05*(MouseY-MouseStartY);
    glutPostRedisplay();
}

void Mouse(int button,int state,int x,int y)
{
    MouseStartX = MouseX = x;
    MouseStartY = MouseY = y;

    if (state == GLUT_DOWN)//rotate
    {
        glutIdleFunc(Idle);
    }

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,450);
    glutInitWindowPosition(250,100);
    glutCreateWindow("BezierKnife");

    scence.knife=new Knife(10,50);
    scence.path =new KnifePath();
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Solid",1);
    glutAddMenuEntry("Lines",2);
    glutAddMenuEntry("Points",3);
    glutAddMenuEntry("DisAssembly",4);
    glutAddMenuEntry("Assembly",5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(ChangeSize);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);

    glutTimerFunc(5, TimerFunc, 1);

    SetupRC();
    glutMainLoop();

    return 0;
}

