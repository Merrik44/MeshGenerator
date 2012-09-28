#ifndef DEBUG_H
#define DEBUG_H

#include "vmath.h"
#include "vector"
#include "string"
#include "mesh.h"

enum Colours { WHITE, BLUE, RED, GREEN, CYAN, YELLOW, MAGENTA, BLACK, LIGHT_GREY, DARK_GREY };

struct Line
{
public:
    Vector3f start, end;
    Colours colour;
};
struct DbgPoint
{
public:
    Vector3f position;
    Colours colour;
};
struct Text
{
public:
    Vector3f position;
    Colours colour;
    std::string text;
};

class Mesh;

class Debug
{
public:
    static float RandomFloat()
    {
        int granularity =10000;
        return (random()%granularity)/(float)granularity;
    }
};

static std::vector<Line> lines;
static std::vector<DbgPoint> points;
static std::vector<Mesh*> models;
static std::vector<Text> textToDraw;


void DrawLine( Vector3f start, Vector3f end);
void DrawRay( Vector3f origin, Vector3f direction);
void DrawPoint( Vector3f start);


void AddLine( Vector3f start, Vector3f end, Colours colour);
void AddRay( Vector3f origin, Vector3f direction, Colours colour);
void AddPoint( Vector3f position, Colours colour);
void AddModel( std::string filepath, float scale, Vector3f position);

void SetColour( Colours colour );
void Print(std::string message );
void DebugDraw();
void DebugClear();
void DrawText( std::string txt, Vector3f pos, Colours colour );
void DrawText( double value, Vector3f pos, Colours colour);
void AddText( std::string txt, Vector3f pos, Colours colour );
void AddText( double value, Vector3f pos, Colours colour);

#endif // DEBUG_H
