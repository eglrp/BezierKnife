#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include "glHelper.h"

#include <fstream>

struct GL_Object
{
    virtual void draw()=0;
};


class Knife:public GL_Object
{
public:
    Knife(float r=1.,float l=5.):radius(r),length(l),displayMode(Solid),list(0),changed(true)
    {
        objCylinder = gluNewQuadric();
    }

    ~Knife()
    {
        glDeleteLists(list,1);
    }

    virtual void draw()
    {
        if(!list)
            list=glGenLists(1);

        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glMultMatrix(pose);

        if(changed)
        {

            glNewList(list,GL_COMPILE_AND_EXECUTE);
            glColor3ub(100,100,255);
            if(displayMode==Solid)
            {
                glutSolidSphere(radius,50,50);
                glTranslatef(0,0,length);
                glutSolidTorus(0,radius,50,50);
//                glutSolidCone(radius,length,50,100);
            }
            else
            {
                glutWireSphere(radius,50,50);
//                glutWireCone(radius,length,50,100);
                glTranslatef(0,0,length);
                glutWireTorus(0,radius,50,50);
            }
            glTranslatef(0,0,-length);
            gluCylinder(objCylinder,radius,radius,length,32,5);
            glEndList();
        }

        glPopMatrix();
    }

    enum KnifeMode
    {
        Solid=0,Wired=1
    } displayMode;

    pi::SE3f    pose;
    float       radius,length;
    bool        changed;
    GLuint      list;
    GLUquadricObj *objCylinder;
};

class KnifePath:public GL_Object
{
public:
    KnifePath():idx_now(0) {}
    KnifePath(const char* filename):idx_now(0)
    {

    }

    void loadPoints(const char* filename)
    {
        std::ifstream ifs(filename);
        pi::Point3f pt;
        while(ifs.is_open()&&ifs.get())
        {
            ifs>>pt;
            points.push_back(pt);
        }
    }

    void setPoints(const std::vector<pi::Point3f>& pts)
    {
        points=pts;
    }

    pi::Point3f getPoint(int velocity=1)
    {
        idx_now+=velocity;
        int size=points.size();
        if(size==0) return pi::Point3f();

        idx_now=idx_now%size;
        return points[idx_now];
    }

    virtual void draw()
    {
        if(!points.size()) return;
        glColor3ub(255,0,0);
        glBegin(GL_LINE);
        for(int i=0,iend=points.size();i<iend;i++)
            glVertex(points[i]);
        glEnd();
    }

private:
    int                      idx_now;
    std::vector<pi::Point3f> points;
};

class Surface:public GL_Object
{
public:
    Surface(int c,const std::vector<pi::Point3f>& pts)
        :cols(c),points(pts),drawControlPoints(true),pNurb(NULL),mode(GLU_FILL)
    {

    }

    virtual void draw()
    {
        if(!points.size()) return;
        if(drawControlPoints)
        {
            // Large Red Points
            glPointSize(5.0f);
            glColor3ub(255,0,0);

            // Draw all the points in the array
            glBegin(GL_POINTS);
            for(int i=0,iend=points.size();i<iend;i++)
                    glVertex(points[i]);
            glEnd();
        }

        //draw suface
    }

//    // NURBS callback error handler
//    void CALLBACK NurbsErrorHandler(GLenum nErrorCode)
//    {
//        char cMessage[64];

//#ifdef WIN32
//        // Extract a text message of the error
//        strcpy(cMessage,"NURBS error occured: ");
//        strcat(cMessage,gluErrorString(nErrorCode));

//        // Display the message to the user
//        MessageBox(NULL,cMessage,NULL,MB_OK | MB_ICONEXCLAMATION);
//#endif
//    }

    bool drawNurbsSurface()
    {
        if(!pNurb)
        {
            pNurb=gluNewNurbsRenderer();

            // Install error handler to notify user of NURBS errors
//            gluNurbsCallback(pNurb, GLU_ERROR, NurbsErrorHandler);

            gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 10.0f);//定义显示误差精度
            // Uncomment the next line and comment the one following to produce a
            // wire frame mesh.
            //gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
            gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, mode);//填充
        //	gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, (GLfloat)GLU_OUTLINE_POLYGON);//定义属性
        //	gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, (GLfloat)GLU_OUTLINE_PATCH);//定义属性
        }
        // Draw in Blue
        glColor3ub(0,0,220);

//        // Save the modelview matrix stack
//        glMatrixMode(GL_MODELVIEW);
//        glPushMatrix();

//        // Rotate the mesh around to make it easier to see
//        glRotatef(330.0f, 1.0f,0.0f,0.0f);

        // Render the NURB
        // Begin the NURB definition
        gluBeginSurface(pNurb);

        // Evaluate the surface
//        gluNurbsSurface(pNurb,	// pointer to NURBS renderer
//            8, ,			// No. of knots and knot array u direction
//            8, ,			// No. of knots and knot array v direction
//            cols * 3,		// Distance between control points in u dir.
//            3,					// Distance between control points in v dir.
//            (float*)points.data(), // Control points
//            cols, points.size()/cols,	// u and v order of surface
//            GL_MAP2_VERTEX_3);		// Type of surface

        // Done with surface
        gluEndSurface(pNurb);

//        // Show the control points
//        DrawPoints();

//        // Restore the modelview matrix
//        glPopMatrix();
    }

    bool getPoint(int x,int y,pi::Point3f& pt)
    {
        int idx=y*cols+x;
        if(idx>=points.size()) return false;
        pt=points[idx];
        return true;
    }

    int                         cols,rows;//Point number of each line
    std::vector<pi::Point3f>    points;//Control points
    bool                        drawControlPoints;
    GLUnurbsObj                 *pNurb;
    GLfloat                     mode;//GLU_OUTLINE_POLYGON GLU_FILL
};

class Scence:public GL_Object
{
public:
    Scence():velocity(1),path(NULL),knife(NULL),suface(NULL){}
    virtual void draw(){

        if(!path) return;

        path->draw();

        if(!knife) return;

        knife->pose.get_translation()=path->getPoint(velocity);
        knife->draw();
    }

    Surface*        suface;
    KnifePath*      path;
    Knife*          knife;
    int             velocity;
};
#endif // GL_OBJECT_H
