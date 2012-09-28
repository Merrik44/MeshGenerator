#include "debug.h"

#include <QtOpenGL>
#include "iostream"

using namespace std;


void Print(string message )
{
    cout << message << endl;
}

void AddLine( Vector3f start, Vector3f end, Colours colour)
{
    Line line;
    line.end = end;
    line.start = start;
    line.colour = colour;
    lines.push_back( line);

}

void AddRay( Vector3f origin, Vector3f direction, Colours colour)
{
    Line line;
    line.end = origin + direction;
    line.start = origin;
    line.colour = colour;
    lines.push_back( line);

}
void AddPoint( Vector3f position, Colours colour)
{
    DbgPoint point;
    point.position= position;
    point.colour= colour;
    points.push_back(point);
}

void DebugClear()
{
    lines.clear();
    points.clear();
    textToDraw.clear();
    for ( unsigned int i = 0; i < models.size(); i++ )
    {
        delete models[i];
    }

    models.clear();
}

void DrawText( string txt, Vector3f pos, Colours colour)
{
    SetColour( colour );
    //glColor3f(1.0, 1.0, 0.0);
    QPainterPath path;
    glDisable(GL_LIGHTING);
    path.addText(QPointF(0, 0), QFont("Arial", 1), QString(txt.c_str()));
    QList<QPolygonF> poly = path.toSubpathPolygons();
    for (QList<QPolygonF>::iterator i = poly.begin(); i != poly.end(); i++){
        glBegin(GL_LINE_LOOP);
        for (QPolygonF::iterator p = (*i).begin(); p != i->end(); p++)
        {
            GLfloat x = p->rx()*0.004f;
            GLfloat y = -p->ry()*0.004f;
            glVertex3f(x + pos.x, y + pos.y, pos.z);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
}


void AddText( double value, Vector3f pos, Colours colour)
{
    glPushMatrix();
    stringstream ss;
    ss << value;
    string txt = ss.str();

    Text message;
    message.text = txt;
    message.position = pos;
    message.colour = colour;

    textToDraw.push_back(message);
}


void AddText( string text, Vector3f pos, Colours colour)
{
    Text message;
    message.text = text;
    message.position = pos;
    message.colour = colour;

    textToDraw.push_back(message);
}

void DrawText( double value, Vector3f pos, Colours colour)
{
    glPushMatrix();
    stringstream ss;
    ss << value;
    string txt = ss.str();

   SetColour( colour );

    QPainterPath path;
    glDisable(GL_LIGHTING);
    path.addText(QPointF(0, 0), QFont("Arial", 1), QString(txt.c_str()));
    QList<QPolygonF> poly = path.toSubpathPolygons();
    for (QList<QPolygonF>::iterator i = poly.begin(); i != poly.end(); i++){
        glBegin(GL_LINE_LOOP);
        for (QPolygonF::iterator p = (*i).begin(); p != i->end(); p++)
        {
            GLfloat x = p->rx()*0.004f;
            GLfloat y = -p->ry()*0.004f;
            glVertex3f(x + pos.x, y + pos.y, pos.z);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void DebugDraw()
{ // glDisable(GL_DEPTH_TEST);
    //  glDisable(GL_DIFFUSE);
    glDisable(GL_LIGHTING);
    for ( unsigned int i = 0; i < lines.size(); i++ )
    {
        SetColour(lines[i].colour);
        DrawLine( lines[i].start, lines[i].end);
    }

    for ( unsigned int i = 0; i < textToDraw.size(); i++ )
    {
        DrawText(textToDraw[i].text, textToDraw[i].position, textToDraw[i].colour);
    }
    for ( unsigned int i = 0; i < points.size(); i++ )
    {
        SetColour(points[i].colour);
        DrawPoint( points[i].position );
    }
    glEnable(GL_LIGHTING);

    //     glEnable(GL_DIFFUSE);
    // glEnable(GL_DEPTH_TEST);
    for ( unsigned int i = 0; i < models.size(); i++ )
    {

        models[i]->Draw();
    }
}

void AddModel( string filepath, float scale, Vector3f position)
{
    Mesh* model = new Mesh(filepath);
    model->scale = scale;
    model->position = position;
    models.push_back(model);

    cout << "Draw" << endl;

}

void DrawLine( Vector3f start, Vector3f end)
{
    glLineWidth(2);
     glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z );
    glVertex3f(end.x, end.y, end.z );
    glEnd( );
      glEnable(GL_LIGHTING);
}


void DrawRay( Vector3f origin, Vector3f direction)
{
    glLineWidth(2);
     glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3f(origin.x, origin.y, origin.z );
    direction += origin;
    glVertex3f(direction.x, direction.y, direction.z );
    glEnd( );
      glEnable(GL_LIGHTING);
}

void DrawPoint( Vector3f start)
{
     glDisable(GL_LIGHTING);
    //SetColour(RED);
    //glColor4( 0.95f, 0.207, 0.031f, 1.0f );
    //glColor4( colour );
    glEnable( GL_POINT_SMOOTH );
    glPointSize( 6.0 );

    glBegin(GL_POINTS);
    glVertex3f(start.x, start.y, start.z );
    glEnd( );
      glEnable(GL_LIGHTING);
}

void SetColour( Colours colour )
{
    GLfloat glCol[] = { 0, 0, 0, 1 };
    switch( colour )
    {

    case RED:
        glCol[0] =1;
        break;
    case BLUE:
        glCol[2] =1;
        break;
    case GREEN:
        glCol[1] =1;
        break;
    case CYAN:
        glCol[1] =1; glCol[2] =1;
        break;
    case MAGENTA:
        glCol[0] =1; glCol[2] =1;
        break;
    case YELLOW:
        glCol[0] =1; glCol[1] =1;
        break;
    case WHITE:
        glCol[0] =1; glCol[1] =1; glCol[2] =1;
        break;
    case BLACK:
        break;
    case LIGHT_GREY:
        glCol[0] =0.7f; glCol[1] =0.7f; glCol[2] =0.7f;
        break;
    case DARK_GREY:
        glCol[0] =0.3f; glCol[1] =0.3f; glCol[2] =0.3f;

        break;

    }
    //    glCol[0] *= 1.5f;
    //    glCol[1]  *= 1.5f;
    //    glCol[2] *= 1.5f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glCol);
    glColor3f(glCol[0], glCol[1], glCol[2]);

}
