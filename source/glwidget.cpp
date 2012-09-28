/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>


#include <math.h>
#include "vmath.h"
#include "glwidget.h"
#include "mesh.h"
#include "debug.h"
#include "string"
#include "meshgenerator.h"
#include "objexporter.h"
#include "QImage"
#include "surfacesubdivision.h"
#include "QDir"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

using namespace std;

int subdivisionLevel = 0;

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    yOffset =0;
    xOffset =0;
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    zoom = 1;

//    zoom = 0.0329676;
//    xOffset = 0.45f;
//yOffset = -0.790001f;
//xRot = 656;
//yRot = 3480;


    skeleton = NULL;
    model = NULL;
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(400, 400);
}
//! [4]

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

//! [5]
void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
//! [5]

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::ResetCamera()
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    yOffset =0;
    xOffset =0;
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::loadGLTextures()
{
  QImage t;
  QImage b;

  if ( !b.load( "../images/irish.jpg" ) )
  {
    b = QImage( 16, 16, (QImage::Format)32 );
    b.fill( Qt::green );
  }

  t = QGLWidget::convertToGLFormat( b );
  glGenTextures( 1, &texture[0] );
  glBindTexture( GL_TEXTURE_2D, texture[0] );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
}

//! [6]
void GLWidget::initializeGL()
{
    QDir myDir("../lst files/");
    lstFileList = myDir.entryList ();
    for( int i = 0; i < lstFileList.count(); i++ )
        cout << lstFileList.value(i).toStdString() << endl;

    LoadLST("../lst files/Dense.lst" );
    model = generateMesh( skeleton->branches);
//ApplyLoopSubvision(model, 3);
    //model = generateMesh( skeleton->root);
    // model = new Model("../obj models/icosahedron.obj");

    //  model = new Model("../obj models/TexturedTube.obj");
    // model = new Model("../obj models/openBox.obj");
    //setSubdivisionLevel(1 );

    //  model->position.y -= 0.4f;
    // model = new Model("../Obj/humanoid_quad.obj");
//odel->scale = 0.1f;


//    QGLFormat format;
//    format.setDoubleBuffer(true);
//    format.setDepth(false);
//    format.setAlpha(false);
//    format.setSampleBuffers(true);
//    format.setSamples(4);
//    this->setFormat(format);

    //qglClearColor(qtPurple.dark());
    glClearColor(0.98f,  0.98f,  0.98f, 1);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );


    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    loadGLTextures();
}
//! [6]


void GLWidget::LoadModel( std::string filepath )
{
    if( model != NULL )
        delete model;
    model = new Mesh(filepath);

}



void GLWidget::LoadLST( std::string filepath )
{
    QString path = QString(  filepath.c_str());
    QString name = path.split(QRegExp( QString("/") )).last();
    listIndex = lstFileList.indexOf(QRegExp(name ));
    if (listIndex < 0)
        listIndex = 0;



    cout << "43" << listIndex << endl;
   // cout << "sss" << filepath << endl;
    if( skeleton != NULL )
        delete skeleton;
    //  cout << "sss" << filepath << endl;
    skeleton = new LstSkeleton(filepath);
  cout << "sss" << filepath << endl;
}

void GLWidget::reloadLST( )
{
    string path = skeleton->filepath;
    if( skeleton != NULL )
        delete skeleton;
    skeleton = new LstSkeleton(path);

}

void GLWidget::ExportModelAsObj(string filepath)
{
    ExportModel( filepath, model );
}

void GLWidget::setSubdivisionLevel( int level )
{

    subdivisionLevel = level;
    if( model == NULL )
        return;


    model->RestoreMeshState();
    model->ClearNeighourAndEdgeData();
    model->ReconstructMeshDataStructure();
    model->CalculateNormals();

    ApplyLoopSubvision(model, level);
    updateGL();
}
void GLWidget::GenerateMeshFromLST()
{
    if(skeleton != NULL)
        model = generateMesh( skeleton->branches);


    setSubdivisionLevel(subdivisionLevel );
   // updateGL();
}

void DrawSquare()
{

}

void gluPerspective(double fovy,double aspect, double zNear, double zFar)
{
    // Start in projection mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}
//! [7]
void GLWidget::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0*zoom, 1, 0.1f, 100);


    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xOffset, yOffset, -10);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

//    cout << "DDDDD" << endl;
//    cout << zoom << endl;
//     cout << xOffset << endl;
//    cout << yOffset << endl;
//    cout << xRot << endl;
//    cout << yRot << endl;


    if( model != NULL)
        model->Draw();

    if( skeleton != NULL)
        skeleton->Draw();

    DebugDraw();


}
//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    Width = Height = side;

    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0*zoom, 1, 0.1f, 100);

    glMatrixMode(GL_MODELVIEW);
}
//! [8]

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        yOffset += -dy*0.005f;
        xOffset += dx*0.005f;
        updateGL();
    }
    lastPos = event->pos();


}
//! [10]

void GLWidget::wheelEvent(QWheelEvent *event)
{
    float sensitivity = 0.001f;
    int numDegrees = -event->delta();

    zoom += zoom*sensitivity*numDegrees;

    updateGL();
    event->accept();
}
