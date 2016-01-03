#ifndef GLHELPER_H
#define GLHELPER_H

#ifdef WIN32
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "SE3/SE3.h"

void glVertex(const pi::Point3d& pt);
void glVertex(const pi::Point3f& pt);

void glMultMatrix(const pi::SE3f& pose);
#endif // GLHELPER_H
