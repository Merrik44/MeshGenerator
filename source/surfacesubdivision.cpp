#include "surfacesubdivision.h"
#include "vector"
#include "mainwindow.h"

using namespace std;

Vertex*  CreateMidPoint( Edge* edge, int verticesArrayIndex)
{
    //Face* faces[2];
    Vertex** vertices = edge->vertices;
  //  Vertex* vertices
     Face** faces = edge->faces;

    //Edge points are constructed on each edge. These points are three eighths of the sum of the
    //two end points of the edge plus one eighth of the sum of the two other points that form the
    //two triangles that share the edge in question.

     Vector3f mid;
     //std::cout << "midV " <<  std::endl;
     // if not a boundary
     if( faces[0] != NULL && faces[1] != NULL )
     {

         // find other points
         Vertex* A = NULL;
         Vertex* B = NULL;
         for ( int i = 0; i < 3; i++)
         {
             if( faces[0]->vertices[i] != vertices[0] && faces[0]->vertices[i] != vertices[1] )
                 A = faces[0]->vertices[i];

             if( faces[1]->vertices[i] != vertices[0] && faces[1]->vertices[i] != vertices[1] )
                 B = faces[1]->vertices[i];

         }
         mid += (vertices[0]->position + vertices[1]->position)*3.0f/8.0f + (A->position + B->position)*1/8.0f;
     }
     else
         mid = (vertices[0]->position + vertices[1]->position)/2.0f; // boundary condition

    // midPoint
    edge->midPoint  = new Vertex(mid.x, mid.y, mid.z, verticesArrayIndex);

    return edge->midPoint;
}



extern MainWindow* mainWindowRef;
void ApplyLoopSubvision( Mesh* model, int levels)
{
    if( levels == 0)
        return;


    vector< Vertex*>& vertices = model->vertices;
    vector< Face*>& triangles = model->triangles;
    vector< Edge*>& edges = model->edges;

    for( int n = 0; n < levels; n++ )
    {

        mainWindowRef->progressBarUpdate(((n+1)*100)/(levels));
       // mainWindowRef.
        int oldVerticesLength = vertices.size();
        // step 1: generate the midpoints

        for( uint i = 0; i < edges.size(); i++ )
        {

            Edge* edge =  edges[i];
            int index = vertices.size();
            Vertex* midpoint = CreateMidPoint(edge,index) ;

            vertices.push_back(midpoint );
        }

        // ----------- split the texture coordinates linearly ---------
        for( int i = 0; i < triangles.size(); i++ )
        {
            Face* face = triangles[i];

            for( uint a = 0; a < 3; a++ )
            {
                // find the edge index associated with the vertices
                int b = (a+1)%3;
                Vector2f A  = face->texCoords[a];
                Vector2f B  = face->texCoords[b];
                Vector2f midTexCoord = (A + B)/2.0f;

                face->midTexCoords[a] = midTexCoord;


            }

        }

        // step 2: create the new faces. NB Specifically for triangles/loop

        int oldLength = triangles.size(); // have to store this because we are adding faces to the vector
        for( int i = 0; i < oldLength; i++ )
        {
            Face* oldFace;
            oldFace = triangles[i];

            for( uint j = 0; j < 3; j++ )
            {

                int a = j;
                int c = (j+1)%3;
                Edge* AB = oldFace->edges[a];
                Edge* BC = oldFace->edges[c];
                Vertex* A = AB->midPoint;
                Vertex* B;
                Vertex* C = BC->midPoint;

                if( AB->vertices[0] == BC->vertices[0]  ) // find the shared vertex
                    B = AB->vertices[0];

                else if( AB->vertices[1] == BC->vertices[0]  ) // find the shared vertex
                    B = AB->vertices[1];

                else if( AB->vertices[1] == BC->vertices[1]  ) // find the shared vertex
                    B = AB->vertices[1];

                else if( AB->vertices[0] == BC->vertices[1]  ) // find the shared vertex
                    B = AB->vertices[0];

                int b;
                if( oldFace->vertices[0] == B)
                    b = 0;
                else if( oldFace->vertices[1] == B)
                    b = 1;
                else if( oldFace->vertices[2] == B)
                    b = 2;

                Vector2f texCoordA = oldFace->midTexCoords[a];
                Vector2f texCoordC = oldFace->midTexCoords[c];
                Vector2f texCoordB = oldFace->texCoords[b];
                Vector2f texCoordD;

                Face* newFace = new Face(A, B, C);

                newFace->SetTextureCoords(texCoordA,texCoordB,texCoordC, texCoordD );
                newFace->calculateNormal();
                triangles.push_back(newFace);

            }

            // set the oldface to be in the center
            // set the oldface to be in the center
            for( int j = 0; j < 3; j++ )
            {
                oldFace->vertices[j] = oldFace->edges[j]->midPoint;
                oldFace->texCoords[j] = oldFace->midTexCoords[j];
            }

        }
        //  cout << "step2"  << endl;
        // Vertex points are constructed for each old vertex. A given vertex has n neighbor vertices.
        // The new vertex point is one minus n times s times the old vertex, plus s times the sum of the
        //neighboring vertices, where s is a scaling factor. For n equal to three, s is three sixteenths.
        // For n greater than three, s is 1/n (5/8 - (3/8 + 1/4 cos(2π / n))2)

        for( int j = 0; j < oldVerticesLength; j++ )
        {
            Vertex* vertex = vertices[j];

            // check if it lies on a boundary
            vector < Edge* > edgesWithOneFace;
            for( uint k = 0; k < vertex->edges.size(); k++ )
            {
                if( vertex->edges[k]->faces[1] == NULL )
                    edgesWithOneFace.push_back(vertex->edges[k] );
            }

            if( edgesWithOneFace.size() != 0 ) // boundary case
            {
                Vector3f averageSum;
                for( uint k = 0; k < edgesWithOneFace.size(); k++ )
                {
                    if(edgesWithOneFace[k]->vertices[0] != vertex )
                        averageSum += edgesWithOneFace[k]->vertices[0]->position;
                    else
                        averageSum += edgesWithOneFace[k]->vertices[1]->position;


                }

                averageSum /= edgesWithOneFace.size();
                vertex->position = vertex->position*3.0f/4.0f + averageSum/4.0f;

            }
            else // regular case
            {
                float s;
                int n = vertex->neighbours.size();

                if( n== 3)
                    s = 3/16.0f;
                else
                {
                    float alpha = (3.0f/8 + (1.0f/4) * cos(2.0f*PI / n));
                    alpha *= alpha;
                    s = (1.0f/n)*(5.0f/8 - alpha );
                }
                vertex->position = vertex->position*( 1 - n*s);

                Vector3f sum;
                for( int k = 0; k < n; k++ )
                    sum += vertex->neighbours[k]->position;

                vertex->position += sum*s;
            }

        }
        model->ClearNeighourAndEdgeData();
        model->ReconstructMeshDataStructure();

    }
model->CalculateNormals();
}
