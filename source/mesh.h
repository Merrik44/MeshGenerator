#ifndef MODEL_H
#define MODEL_H

#include "vmath.h"
#include <string>
#include "vector"
#include "iostream"
#include <algorithm>
#include "debug.h"


struct Edge;
struct Face;
struct Vertex;


class Debug;


struct Vertex
{
    std::vector<Edge*> edges;
    std::vector<Vertex*> neighbours;
    std::vector<Face*> faces;


    int index;
    Vector3f position;
    Vector3f normal;
    Vector2f textureCoordinate;
    std::vector<Vector2f> textureCoords;
    bool isSeam;


    // joint stuff
    std::vector<Vertex*> illegalVertices;
    std::vector<  std::vector< Vertex* >* > loops;
    std::string loopID;
    int loopIndex;
    Vector3f loopCenter;
    Vector3f finalPosition;
    Vector3f loopNormal;
    int weight;

    int Id;

    Vertex* previous;
    Vertex* next;

    std::vector<Vertex*> visible;
    std::vector<Vertex*> tabooVertices;


    Vertex( float a, float b, float c, int Index)
    {
        position.x = a;
        position.y = b;
        position.z = c;
        index = Index;
        loopID = "";
        loopIndex = -1;
        weight=1;
        previous = NULL;
        next = NULL;
    }

    Vertex( Vector3f Position, int Index)
    {
        position=Position;
        index = Index;
        loopID = "";
        weight=1;
        loopIndex = -1;
        previous = NULL;
        next = NULL;
    }

    Vertex( Vertex* vertexToCopy )
    {
        position = vertexToCopy->position;
        index = vertexToCopy->index;
        loopID = vertexToCopy->loopID;
        weight=vertexToCopy->weight;
        loopIndex = vertexToCopy->loopIndex;

    }

    ~Vertex()
    {
        edges.clear();
        neighbours.clear();
        faces.clear();
    }

    bool IsLegal( Vertex* B)
    {
        for( unsigned int i = 0; i < illegalVertices.size(); i++)
            if( illegalVertices[i] == B)
                return false;

        return true;
    }

    // Subtracts the taboo verts from the visible
    void applyTabooToVisible()
    {

        for( int i = visible.size() -1; i >= 0; i--)
        {
            Vertex* visibleVert = visible[i];
            std::vector<Vertex*>::iterator result = find (tabooVertices.begin(), tabooVertices.end(), visibleVert);
            if (result != tabooVertices.end()) // found
                visible.erase( visible.begin() + i );

        }

        tabooVertices.clear();
    }

    void calculateVisible(std::vector<Vertex*>& otherVerts)
    {
        visible.clear();

        // everything except itself
        for( int i = otherVerts.size() -1; i >= 0; i--)
        {

            if (otherVerts[i] == this)
                continue;

            visible.push_back(otherVerts[i]);
        }

    }

};

struct Face
{
    Vertex* vertices[4];
    std::vector<Edge*> edges;
    Vector2f texCoords[4];
    Vector2f midTexCoords[4];
    int id;
    Vector3f normal;
    bool reTriangulated;
    bool markedForDeletion;
    bool coordsHavebeenSet;

    Face( Vertex* a, Vertex* b, Vertex* c )
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = NULL;
        reTriangulated = false;
        markedForDeletion = false;
        Vector2f zero(0,0);

        SetTextureCoords(zero,  zero, zero,zero  );


    }

    Face( Vertex* a, Vertex* b, Vertex* c, Vertex* d )
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = d;
        reTriangulated = false;
        markedForDeletion = false;
        Vector2f zero(0,0);
        SetTextureCoords(zero,  zero, zero,zero  );

    }





    ~Face()
    {
        edges.clear();
        vertices[0] = NULL;
        vertices[1] = NULL;
        vertices[2] = NULL;
        vertices[3] = NULL;
    }

public:
    void calculateNormal( )
    {
        Vector3f A = vertices[0]->position;
        Vector3f B = vertices[1]->position;
        Vector3f C = vertices[2]->position;
        if (A!=B && B!=C && C!=A) // check fot=r zero area case
        {

        normal = -(A - B ).crossProduct( C- B);
        normal.normalize();
        }
        else
        {
            normal = Vector3f(0, 0, 0);

            std::cout << "warning: zero area face" << std::endl;

        }
    }

    void SetVertices( Vertex* a, Vertex* b, Vertex* c, Vertex* d = NULL  )
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = d;

    }

    void SetTextureCoords( Vector2f& a,   Vector2f& b, Vector2f& c, Vector2f& d  )
    {
        texCoords[0] = a;
        texCoords[1] = b;
        texCoords[2] = c;
        texCoords[3] = d;

    }

    void addNormalToVertices( )
    {
        for( int i = 0; i < 4; i++)
            if(vertices[i]!= NULL)
                vertices[i]->normal += normal;
    }
};

struct Edge
{
public:
    Face* faces[2];
    Vertex* vertices[2];
    Vertex* midPoint;
    int faceCount;

    Edge( Vertex* A, Vertex* B, Face* face1  )
    {
        vertices[0] = A;
        vertices[1] = B;
        faces[0] = face1;
        faces[1] = NULL;
        faceCount = 1;
    }

    Edge( Vertex* A, Vertex* B )
    {
        vertices[0] = A;
        vertices[1] = B;
        faces[0] = NULL;
        faces[1] = NULL;
        faceCount = 1;
    }

    ~Edge( )
    {
        faces[0] = NULL;
        faces[1] = NULL;
        vertices[0] = NULL;
        vertices[1] = NULL;
        midPoint = NULL;
    }

    bool AreEqual(Edge* other )
    {
        if( vertices[0] == other->vertices[0] &&  vertices[1] == other->vertices[1])
            return true;

        if( vertices[1] == other->vertices[0] &&  vertices[0] == other->vertices[1])
            return true;

        return false;
    }

    bool ConsistsOf(Vertex* A, Vertex* B)
    {
        if( vertices[0] == A &&  vertices[1] == B)
            return true;

        if( vertices[1] == A &&  vertices[0] == B)
            return true;

        return false;
    }

};


class Mesh
{
public:
    std::string name;
    std::string path;
    Vector3f position;
    float scale;

    std::vector< Vertex* > vertices;
    std::vector< Face* > triangles;
    std::vector< Face* > quads;
    std::vector< Edge* > edges;

    std::vector< Vertex* > verticesBackup;
    std::vector< Face* > trianglesBackup;
    std::vector< Face* > quadsBackup;

    Mesh();
    Mesh( std::string fileName);
    ~Mesh();
    void Draw();
    void Load( std::string fileName);
    void ReconstructMeshDataStructure();
    void ClearNeighourAndEdgeData();
    void ClearSubdivisions();
    int GetFaceCount( );
    void StoreMeshState();
    void RestoreMeshState();
    void ConstructGraphFromFaces( std::vector < Face* >& faces);
    void StoreTextureCoordInVerticesAndMarkSeams();
    void RecalculateSpecificNormals(std::vector < Face* >& faces, std::vector < Vertex* >& verticesToUpdate);
    void FillMissingFaces();
    void  CalculateNormals( );

};

#endif // MODEL_H
