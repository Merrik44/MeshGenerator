#include "mesh.h"
#include "iostream"
#include "fstream"
#include "debug.h"

#include "sstream"
#include <QtOpenGL>
#include <stdlib.h>
#include <math.h>


using namespace std;

bool displayWireFrame = false;

bool smoothShading = false;

Mesh::Mesh(string fileName)
{
    scale = 1.0f;
    Load(fileName);
}

Mesh::Mesh()
{
    scale = 1.0f;
}

void Mesh::Load( std::string fileName)
{
    path = fileName;
    cout << "Loading model: " << fileName << endl;

    ifstream myfile(fileName.c_str());
    std::vector<Vector2f> textureCoords;

    if (myfile.is_open())
    {
        string line;
        string token;

        while ( myfile.good() )
        {
            token = "";
            getline (myfile,line);
            stringstream sstream(line);
            sstream >> token;
            if( token == "g")
            {
                sstream >> name;
            }
            else if( token == "v")  // vertex
            {
                float a, b,c ;
                sstream >> token;
                a = atof(token.c_str());
                sstream >> token;
                b = atof(token.c_str());
                sstream >> token;
                c = atof(token.c_str());

                int index = vertices.size();
                vertices.push_back( new Vertex(a,b,c, index));
            }
            else if( token == "vt")  // tex coord
            {
                float a, b,c ;
                sstream >> token;
                a = atof(token.c_str());
                sstream >> token;
                b = atof(token.c_str());



                textureCoords.push_back( Vector2f(a,b));
            }
            else if( token == "f")  // face
            {
                int a, b,c, d =-1;
                Vector2f texA, texB, texC, texD;
                QString qToken;
                QStringList components;

                sstream >> token;



                qToken = QString(token.c_str());
                components = qToken.split(QRegExp("/"));
                if(components.count() == 3 )
                {
                    cout<< components.value(1).toInt()-1 << endl;
                    texA = textureCoords[ components.value(1).toInt()-1];
                }


                a = atof(token.c_str()) - 1;
                Vertex* vertA = vertices[a];

                sstream >> token;
                qToken = QString(token.c_str());
                components = qToken.split(QRegExp("/"));
                if(components.count() == 3 )
                    texB = textureCoords[ components.value(1).toInt()-1];
                cout<< components.value(1).toInt()-1 << endl;
                b = atof(token.c_str()) - 1;
                Vertex*  vertB = vertices[b];

                sstream >> token;
                qToken = QString(token.c_str());
                components = qToken.split(QRegExp("/"));
                if(components.count() == 3 )
                    texC = textureCoords[ components.value(1).toInt()-1];
                cout<< components.value(1).toInt()-1 << endl;
                c = atof(token.c_str()) - 1;
                Vertex*  vertC = vertices[c];

                sstream >> ws;
                if( sstream.good() ) // if there is a 4th vertex
                {
                    sstream >> token;
                    qToken = QString(token.c_str());
                    components = qToken.split(QRegExp("/"));
                    if(components.count() == 3 )
                        texD = textureCoords[ components.value(1).toInt()-1];
                    cout << texA  << texB  << texC  << texD << endl;
                    d = atof(token.c_str()) - 1;
                    Vertex*  vertD = vertices[d];

                    Face* face = new Face(vertA, vertB, vertC, vertD);
                    face->SetTextureCoords(texA, texB, texC, texD);
                    quads.push_back(face);
                }
                else
                {
                    Face* face = new Face(vertA, vertB, vertC);
                    face->SetTextureCoords(texA, texB, texC, texD);
                    triangles.push_back(face);
                }

            }
        }
        myfile.close();

        for( int i = 0; i < textureCoords.size(); i++)
            cout << textureCoords[i] << endl;
        StoreMeshState();
        CalculateNormals();
        ReconstructMeshDataStructure( );

    }

    else cout << "Unable to open file" << fileName <<endl;
}

int Mesh::GetFaceCount( )
{
    int count = 0;

    count += triangles.size();
    count += quads.size();
    //cout << count << endl;
    return count;
}

void Mesh::StoreMeshState()
{
    // make a backup of vertices
    for ( uint i =0; i < verticesBackup.size(); i++)
        delete verticesBackup[i];

    //  verticesBackup.reserve( vertices.size() );
    verticesBackup.clear();

    for ( uint i =0; i < vertices.size(); i++)
        verticesBackup.push_back( new Vertex( vertices[i] ) );




    // make a backup of triangles
    for ( uint i =0; i < trianglesBackup.size(); i++)
        delete trianglesBackup[i];

    // trianglesBackup.reserve( triangles.size() );
    trianglesBackup.clear();


    for ( uint i =0; i < triangles.size(); i++)
    {
        int a = triangles[i]->vertices[0]->index;
        Vertex * A = verticesBackup[a];

        int b = triangles[i]->vertices[1]->index;
        Vertex * B = verticesBackup[b];

        //cout << triangles[i]->vertices[1]->position << endl;
        // cout << B->position << endl;

        int c = triangles[i]->vertices[2]->index;
        Vertex * C = verticesBackup[c];

        Face* ABC = new Face(A, B, C);
        ABC->id = triangles[i]->id;

        ABC->SetTextureCoords( triangles[i]->texCoords[0], triangles[i]->texCoords[1], triangles[i]->texCoords[2], triangles[i]->texCoords[3]);

        trianglesBackup.push_back(ABC);

    }




    // make a backup of quads
    for ( uint i =0; i < quadsBackup.size(); i++)
        delete quadsBackup[i];
    quadsBackup.clear();

    // quadsBackup.reserve( quads.size() );

    for ( uint i =0; i < quads.size(); i++)
    {
        int a = quads[i]->vertices[0]->index;
        Vertex * A = verticesBackup[a];

        int b = quads[i]->vertices[1]->index;
        Vertex * B = verticesBackup[b];

        int c = quads[i]->vertices[2]->index;
        Vertex * C = verticesBackup[c];

        int d = quads[i]->vertices[3]->index;
        Vertex * D = verticesBackup[d];

        Face* backUpFace =new Face(A, B, C, D);
        backUpFace->SetTextureCoords( quads[i]->texCoords[0], quads[i]->texCoords[1], quads[i]->texCoords[2], quads[i]->texCoords[3]);
        quadsBackup.push_back(backUpFace);
    }
}

void Mesh::RestoreMeshState()
{
    if( verticesBackup.size() == 0)
        cout << "Unable to restor mesh, no state save" << endl;

    // make a backup of vertices
    for ( uint i =0; i < vertices.size(); i++)
        delete vertices[i];
    vertices.clear();

    //vertices.reserve( verticesBackup.size() );

    for ( uint i =0; i < verticesBackup.size(); i++)
        vertices.push_back( new Vertex( verticesBackup[i] ) );


    // make a backup of triangles
    for ( uint i =0; i < triangles.size(); i++)
        delete triangles[i];
    triangles.clear();

    //  triangles.reserve( trianglesBackup.size() );

    for ( uint i =0; i < trianglesBackup.size(); i++)
    {
        int a = trianglesBackup[i]->vertices[0]->index;
        Vertex * A = vertices[a];

        int b = trianglesBackup[i]->vertices[1]->index;
        Vertex * B = vertices[b];

        int c = trianglesBackup[i]->vertices[2]->index;
        Vertex * C = vertices[c];

        Face* ABC = new Face(A, B, C);
        ABC->id = trianglesBackup[i]->id;
        ABC->SetTextureCoords( trianglesBackup[i]->texCoords[0], trianglesBackup[i]->texCoords[1], trianglesBackup[i]->texCoords[2], trianglesBackup[i]->texCoords[3]);

        triangles.push_back(ABC);

    }

    // make a backup of quads
    for ( uint i =0; i < quads.size(); i++)
        delete quads[i];
    quads.clear();

    // quads.reserve( quadsBackup.size() );

    for ( uint i =0; i < quadsBackup.size(); i++)
    {
        int a = quadsBackup[i]->vertices[0]->index;
        Vertex * A = vertices[a];

        int b = quadsBackup[i]->vertices[1]->index;
        Vertex * B = vertices[b];

        int c = quadsBackup[i]->vertices[2]->index;
        Vertex * C = vertices[c];

        int d = quadsBackup[i]->vertices[2]->index;
        Vertex * D = vertices[d];

        Face* newFace =new Face(A, B, C, D);
        newFace->SetTextureCoords( quadsBackup[i]->texCoords[0], quadsBackup[i]->texCoords[1], quadsBackup[i]->texCoords[2], quadsBackup[i]->texCoords[3]);
        quadsBackup.push_back(newFace);

        quads.push_back(newFace);
    }
}

void  Mesh::CalculateNormals( )
{
    for ( uint i =0; i < quads.size(); i++)
        quads[i]->calculateNormal();
    for ( uint i =0; i < triangles.size(); i++)
        triangles[i]->calculateNormal();


    // reset vertex normals
    for ( uint i =0; i < vertices.size(); i++)
        vertices[i]->normal = Vector3f();

    // calculate Vertex Normals
    for ( uint i =0; i < quads.size(); i++)
        quads[i]->addNormalToVertices();
    for ( uint i =0; i < triangles.size(); i++)
        triangles[i]->addNormalToVertices();


    for ( uint i =0; i < vertices.size(); i++)
        vertices[i]->normal.normalize();

}

void Mesh::ReconstructMeshDataStructure( )
{

    //cout<< "normals" <<endl;
    // calculate Face Normals


    // construct the data stucture
    ConstructGraphFromFaces(quads);
    ConstructGraphFromFaces(triangles);

}

void Mesh::ConstructGraphFromFaces( vector < Face* >& faces)
{
    for ( int i =0; i < (int)faces.size(); i++)
    {
        Face* face = faces[i];

        // is it a triangle or a quad
        int sides = 4;
        if(  face->vertices[3] == NULL)
            sides = 3 ;

        for( int j = 0; j< sides; j++ )
        {
            Vertex* A =  face->vertices[j];
            Vertex* B =  face->vertices[(j+1)%sides];

            // add face
            A->faces.push_back(face );

            //  add neigbours and construct edges
            vector<Vertex*>::iterator result =   find( A->neighbours.begin(), A->neighbours.end(), B );             ;
            if( result == A->neighbours.end() )
            {
                A->neighbours.push_back(B);
                B->neighbours.push_back(A);

                // create a new edge
                Edge* edge = new Edge(A, B, face);
                face->edges.push_back(edge);
                A->edges.push_back( edge );
                B->edges.push_back( edge );
                edges.push_back(edge);

            }
            else
            {
                // edge already exists, just need to find it and tell it about this face
                for ( uint k =0; k < A->edges.size(); k++)
                {
                    Edge* edge =  A->edges[k];
                    if(edge->vertices[0] == B || edge->vertices[1] == B )
                    {
                        edge->faces[1] = face;
                        face->edges.push_back(edge);
                    }
                }
            }
        }

    }

    for ( int i =0; i < (int)faces.size(); i++)
    {
        Face* face = faces[i];
        std::vector<Edge*> edgesCopy( face->edges);

        int edgeCount = 3;
        if( face->vertices[3] != NULL)
            edgeCount = 4;
        face->edges.clear();
        face->edges.resize(edgeCount);
        for ( int a =0; a < edgeCount; a++)
        {
            int b = (a+1)%edgeCount;
            Vertex* A = face->vertices[a];
            Vertex* B = face->vertices[b];

            for ( uint i = 0; i < edgesCopy.size(); i++)
            {
                if(edgesCopy[i]->ConsistsOf(A,B) )
                {
                    //       cout<< edges.size() << " " << a <<endl;
                    face->edges[a] = edgesCopy[i];
                    break;
                }
            }
        }


    }
}


Mesh::~Mesh()
{
    cout << "BOOM! model was blown to bits (destroyed)"<< endl;
    for (uint i = 0; i < vertices.size(); i++)
        delete vertices[i];

    for (int i = 0; i < (int)triangles.size(); i++)
    {
        delete triangles[i];
        triangles[i] = NULL;
    }
    for (int i = 0; i < (int)quads.size(); i++)
        delete quads[i];
    for (int i = 0; i < (int)edges.size(); i++)
    {
        delete edges[i];
        edges[i] = NULL;
    }
    triangles.clear();
    edges.clear();


    // delete backups
    for ( uint i =0; i < verticesBackup.size(); i++)
        delete verticesBackup[i];
    for ( uint i =0; i < trianglesBackup.size(); i++)
        delete trianglesBackup[i];
    for ( uint i =0; i < quadsBackup.size(); i++)
        delete quadsBackup[i];
}

void Mesh::ClearNeighourAndEdgeData()
{

    // clear edge data
    for ( unsigned int i = 0; i < triangles.size(); i++)
        triangles[i]->edges.clear();
    for ( unsigned int i = 0; i < quads.size(); i++)
        quads[i]->edges.clear();

    for ( unsigned int i = 0; i < edges.size(); i++)
    {
        delete edges[i];
        edges[i] = NULL;
    }

    edges.clear();


    // clear face data
    for ( int i = 0; i < (int)vertices.size(); i++ )
    {
        vertices[i]->neighbours.clear();
        vertices[i]->edges.clear();
        vertices[i]->faces.clear();
    }
}

extern bool display4Texture;
extern bool displayMesh;
void Mesh::Draw()
{
    //CalculateNormals();
    //DebugClear();
    SetColour( LIGHT_GREY );
    glPushMatrix();
    {
        glTranslatef( position.x,  position.y,  position.z);
        if( displayMesh)
        {
            glBegin( GL_TRIANGLES );
            {
                for ( int j = 0; j < (int)triangles.size(); j++ ) // for each triangle
                {
                    Face* tri = triangles[j];


                    for ( int k = 0; k < 3; k++ )
                    {

                        //Vector3f normal = tri->vertices[k]->normal;
                        Vector3f normal ;
                        if(smoothShading)
                            normal = tri->vertices[k]->normal;
                        else
                            normal = tri->normal;

                        glNormal3f( normal.x, normal.y, normal.z );


                        if( display4Texture)
                        {

                            Vector2f texCoord = tri->texCoords[k];
                            glTexCoord2f(texCoord.x, texCoord.y);
                        }

                        Vector3f vert = tri->vertices[k]->position;

                        vert *= scale;
                        vert+=position;
                        glVertex3f( vert.x, vert.y, vert.z );
                    }
                }
            }
            glEnd();

            glBegin( GL_QUADS );
            {
                for ( int j = 0; j < (int)quads.size(); j++ ) // for each triangle
                {
                    const Face* quad = quads[j];


                    for ( int k = 0; k < 4; k++ )
                    {

                        if( display4Texture)
                        {

                            Vector2f texCoord = quad->texCoords[k];
                            glTexCoord2f(texCoord.x, texCoord.y);
                        }

                        Vector3f normal ;
                        if(smoothShading)
                            normal = quad->vertices[k]->normal;
                        else
                            normal = quad->normal;
                        glNormal3f( normal.x, normal.y, normal.z ); // this needs to be fixed
                        // glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
                        Vector3f vert = quad->vertices[k]->position;
                        vert *= scale;
                        vert+=position;
                        glVertex3f( vert.x, vert.y, vert.z );


                    }
                }

                glEnd();
            }
        }
        if( displayWireFrame)
        {
            SetColour(RED);
            for ( uint i =0; i < edges.size(); i++)
                DrawLine(edges[i]->vertices[0]->position*scale, edges[i]->vertices[1]->position*scale);
        }
        //            for ( uint i =0; i < vertices.size(); i++)
        //                DrawPoint(vertices[i]->position *scale);

        for ( uint i =0; i < vertices.size(); i++)
        {
    //        break;
            Vertex* A = vertices[i];

            bool unique = true;
            for ( uint j =0; j < A->textureCoords.size(); j++)
            {
                Vector2f texCoordA = A->textureCoords[j];
                for ( uint k = j+1; k < A->textureCoords.size(); k++)
                {
                    Vector2f texCoordB = A->textureCoords[k];
                    if(texCoordA!=texCoordB )
                    {


                        unique = false;
                        break;
                    }
                }
            }

if(!unique)
            for ( uint j =0; j < A->textureCoords.size(); j++)
            {
                Vector2f texCoord = A->textureCoords[j];
                //                stringstream ss;
                //                ss <<


                DrawText( texCoord.toString(),A->position, (Colours)(j+2) );
              //  else
                //    DrawText( texCoord.toString(),A->position , GREEN );
            }
            //            if( vertices[i]->faces.size() == 0)
            //                continue;
            //            if( vertices[i]->isSeam == true)
            //                SetColour(GREEN);
            //            else
            //                SetColour(BLUE);



         //   DrawPoint( vertices[i]->position*scale );
        }
        // DrawRay(vertices[i]->position,vertices[i]->normal*0.05f );
    }
    glPopMatrix();
}




void Mesh::StoreTextureCoordInVerticesAndMarkSeams()
{
    cout << "ssss"  <<endl;
    for( uint i = 0; i < vertices.size(); i++ )
    {
        Vertex* vert = vertices[i];
        vert->textureCoordinate = Vector2f(-1, -1 );
        vert->isSeam = false;
    }

    for( uint i = 0; i < triangles.size(); i++ )
    {
        Face* face = triangles[i];
        if( face->texCoords[0] == Vector2f(0,0) && face->texCoords[1] == Vector2f(0,0)
                && face->texCoords[2] == Vector2f(0,0) && face->texCoords[3] == Vector2f(0,0)) // face is already parameterised so continue
            continue;


        int sides = 3;
        if(face->vertices[3] != NULL )
            sides =4;
        for( uint i = 0; i < sides; i++ )
        {
            Vertex* A = face->vertices[i];
            Vector2f texCoordA =  A->textureCoordinate;
            if( texCoordA.x == 1 ) texCoordA.x-= 1;
            if( texCoordA.y == 1 ) texCoordA.y-= 1;

            Vector2f texCoord =  face->texCoords[i];
            if( texCoord.x == 1 ) texCoord.x-= 1;
            if( texCoord.y == 1 ) texCoord.y-= 1;

       //     A->textureCoords.push_back(texCoord);


            if( A->textureCoordinate != Vector2f(-1, -1 ) ) // vertex has already been assigned by another face
            {

                if( texCoordA != texCoord) // the other face assigned a different coord, so this is a seam
                {
                  //  cout << "-----"  <<endl;
                   // cout << texCoordA  <<endl;
                  //  cout << texCoord << endl;
                    // mark all vertex as seam

                    A->isSeam = true;
                }
            }
            else
                A->textureCoordinate = texCoord; // else the vertex has not yet been assigned a coor, so assign this one
        }
    }
}


void Mesh::FillMissingFaces()
{

    for ( unsigned int i = 0; i < edges.size(); i++)
    {
        Edge* edge = edges[i];

        if( edge->faces[1] == NULL )
        {
            vector< Edge* > possibleLeftEdges;
            vector< Edge* > possibleRightEdges;

            Vertex* vert1 =edge->vertices[0];
            Vertex* vert2 =edge->vertices[1];
            // cout << "--33--" << endl;
            for( unsigned int j = 0; j < vert1->edges.size(); j++)
            {
                Edge* other = vert1->edges[j];
                if( edge == other )
                    continue;

                if( other->faces[1] == NULL  )
                    possibleLeftEdges.push_back(other);
            }
            for( unsigned int j = 0; j < vert2->edges.size(); j++)
            {
                Edge* other = vert2->edges[j];
                if( edge == other )
                    continue;

                if( other->faces[1] == NULL  )
                    possibleRightEdges.push_back(other);
            }


            // --- try find a valid combo of left and right ---
            for ( unsigned int l = 0; l < possibleLeftEdges.size(); l++ )
            {
                Edge* A = possibleLeftEdges[l];
                for ( unsigned int r = 0; r < possibleRightEdges.size(); r++ )
                {
                    Edge* B = possibleRightEdges[r];
                    Vertex* vert3 = NULL;


                    // --- if the share a vertex then they form a triangle ---

                    if( B->vertices[0] ==  A->vertices[0] )
                        vert3 = B->vertices[0];

                    else if( B->vertices[1] ==  A->vertices[1] )
                        vert3 = B->vertices[1];

                    else if( B->vertices[1] ==  A->vertices[0] )
                        vert3 = B->vertices[1];

                    else if( B->vertices[0] ==  A->vertices[1] )
                        vert3 = B->vertices[0];

                    // --- create the face ---
                    if (vert3 != NULL )
                    {
                        Face* newFace = new Face(vert2,  vert1, vert3 );
                        A->faces[1] = newFace;
                        B->faces[1] = newFace;
                        edge->faces[1] = newFace;
                        triangles.push_back(newFace);
                    }

                }
            }
        }

    }
}
