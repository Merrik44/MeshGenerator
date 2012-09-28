#include "jointgenerator.h"
#include "stack"
#include "vmath.h"
#include "iostream"
#include "vector"
#include "debug.h"
#include "glwidget.h"
#include "math.h"
#include "algorithm"


using namespace std;

extern int faces, Count;
bool enableProgressControl = true;



Vertex* weightedmMergeAwithedB( Vertex* A, Vertex* B, Mesh* model, Mesh* jointModel )
{


    // AddPoint( B->position, GREEN);
    //AddPoint( A->position, BLUE);

    // ------- Add A to B ---------
    B->position*= B->weight;
    B->normal*=B->weight;


    B->position += A->position*A->weight;
    B->normal += A->normal*A->weight;

    // ------ Normalize B, which now represents the merged point-----
    B->position /= B->weight +A->weight;
    B->normal /= B->weight +A->weight;
    B->weight += A->weight;

    B->loopID += A->loopID;


   // A->position = B->position;


    ////////////////////////////////////////////////gafffffffffffffffffffffffffffffffffffffsorjgsoeg   Remember to add quads
    // ----- update the faces that were pointing at A to point to B ----
    for( uint j = 0; j < model->triangles.size(); j++)
    {
        Face* face = model->triangles[j];
        for( uint k = 0; k < 4; k++)
        {
            if( face->vertices[k] == A)
                face->vertices[k] = B;
        }

    }

    for( uint j = 0; j < jointModel->triangles.size(); j++)
    {
        Face* face = jointModel->triangles[j];
        for( uint k = 0; k < 4; k++)
        {
            if( face->vertices[k] == A)
                face->vertices[k] = B;
        }

    }

//    for( int j = B->neighbours.size() -1 ; j >=0; j--)
//    {
//        if( B->neighbours[j] == A )
//            B->neighbours.erase(j + B->neighbours.begin());
//    }

//     for( int j = B->edges.size() -1 ; j >=0; j--)
//     {
//         if( B->edges[j]->vertices[0] == A )
//               B->edges[j]->vertices[0] = B;

//        if( B->edges[j]->vertices[1] == A )
//             B->edges[j]->vertices[1] = B;

//     }

//    for( int j = model->vertices.size() -1 ; j >=0; j--)
//    {
//        if( model->vertices[j] == A )
//        {
//            //delete model->vertices[j];
//            model->vertices.erase(model->vertices.begin() + j);
//        }
//    }

//    for( int j = jointModel->vertices.size() -1 ; j >=0; j--)
//    {
//        if( jointModel->vertices[j] == A )
//        {
//           // delete jointModel->vertices[j];
//            jointModel->vertices.erase(jointModel->vertices.begin() + j);
//        }
//    }

//delete A;
    return B;
}


void getRootBoundaryLoopsAndRootVertices(vector< vector<Vertex*>* >& branches, vector<vector<Vertex*>* >& rootBoundaryLoops,  vector<Vertex*>& rootVertices)
{

    for (unsigned int j = 0; j < branches.size(); j++)
    {
        vector<Vertex*>*& boundary =  branches[j];
        vector<Vertex*>* branchRootVerts = new vector<Vertex*>();
        for (unsigned int k = 0; k < boundary->size(); k++)
        {
            Vertex * vertex = (*boundary)[k];
            branchRootVerts->push_back(vertex);
            rootVertices.push_back(vertex);
        }

        // neighbors are need for calculating the visible
        int len = branchRootVerts->size();
        for (int i = 0; i < len; i++)
        {
            Vertex* left = (*branchRootVerts)[(i + 1 + len) % len];
            Vertex* right = (*branchRootVerts)[(i - 1 + len) % len];
            Vertex* current = (*branchRootVerts)[i];
            current->neighbours.push_back(left);
            current->neighbours.push_back(right);
        }


        rootBoundaryLoops.push_back(branchRootVerts);
    }

}

// ---- would like to implement quick sort at some point ----
void SortByLength( vector< Edge*>& edges )
{

    for( uint i = 0; i < edges.size(); i++)
    {
        Vector3f edgesVecA = edges[i]->vertices[0]->position - edges[i]->vertices[1]->position;
        float minSqrLength = edgesVecA.lengthSq();
        int minIndex = i;

        for( uint j = i+1; j < edges.size(); j++)
        {
            Vector3f edgesVecB = edges[j]->vertices[0]->position - edges[j]->vertices[1]->position;
            float sqLength = edgesVecB.lengthSq();
            if( sqLength < minSqrLength )
            {
                minIndex = j;
                minSqrLength = sqLength;
            }
        }

        Edge* temp = edges[i];
        edges[i] = edges[minIndex];
        edges[minIndex] = temp;
    }


}

bool DoStringsShareCharacters(string A, string B)
{
    bool sharedCharFound = false;
    for( uint k =0; k<  B.length(); k++ )
    {
        if( A.find_first_of( B[k], 0) != -1)
        {
            sharedCharFound = true;
            break;
        }
    }

    return sharedCharFound;
}

int counter2 = 300;
bool CollapseJoint(Mesh* jointModel, Mesh* model)
{


    //  cout << "s---" <<endl;
    bool AtleastOneCollapse = false;
    SortByLength(jointModel->edges);
    vector< Face* > facesToDelete;
    for (uint i = 0; i < jointModel->edges.size(); i++ )
    {




        counter2--;
        if( counter2 < 0)
            break;

        if( counter2 == 0 )
        {
            for (uint r = 0; r < jointModel->edges.size(); r++ )
                 AddLine(jointModel->edges[r]->vertices[0]->position, jointModel->edges[r]->vertices[1]->position, CYAN);
        }

        Edge* edge = jointModel->edges[i];



        Vertex* A = edge->vertices[0];
        Vertex* B = edge->vertices[1];


        if( DoStringsShareCharacters( A->loopID, B->loopID ) )
            continue;

        //AddLine(A->position, B->position, GREEN );
        Face* faceA = edge->faces[0];
        Face* faceB = edge->faces[1];

        if( faceA== NULL || faceB == NULL )
            continue;
        if( faceA->markedForDeletion )
            continue;

        if( faceB->markedForDeletion )
            continue;


        if( counter2 == 0)
        {

         //   AddPoint(B->position, BLACK);
            AddLine( A->position, B->position, GREEN);
          //  break;
        }
       else
        {
            // ---- collapse ----
            // continue;
            B  = weightedmMergeAwithedB( A, B, model, jointModel );
            AtleastOneCollapse = true;
        }
       // cout << counter2 << endl;



//        for( uint k = 0; k<  A->edges.size(); k++)
//        {
//            if( counter2 == 0)
//            {

//                AddLine(A->edges[k]->vertices[0]->position, A->edges[k]->vertices[1]->position, YELLOW);
//            }
//            if( A->edges[k]->vertices[0] == A )
//            {
//                A->edges[k]->vertices[0] = B;
//                 if( counter2 == 0)
//                 {
//                      AddPoint(A->edges[k]->vertices[1]->position, GREEN);
//                 }
//            }

//            if( A->edges[k]->vertices[1] == A )
//            {
//                A->edges[k]->vertices[1] = B;
//                if( counter2 == 0)
//                {
//                    AddPoint(A->edges[k]->vertices[0]->position, GREEN);
//                }
//            }
//        }

        // ----- Find all Tris that were pointing to A and tell the to point at B ----
//        for( uint k = 0; k<  A->faces.size(); k++)
//        {
//            for( int l = 0; l < 3; l++ )
//            {
//                if( A->faces[k]->vertices[l] == A )
//                {
//                    A->faces[k]->vertices[l] = B;
//                }
//            }
//        }



        //
        for (uint k = 0; k < jointModel->edges.size(); k++ )
        {
            if( jointModel->edges[k]->vertices[0] == A )
                jointModel->edges[k]->vertices[0] = B;

            if( jointModel->edges[k]->vertices[1] == A )
                jointModel->edges[k]->vertices[1] = B;
        }
          //   AddLine(jointModel->edges[r]->vertices[0]->position, jointModel->edges[r]->vertices[1]->position, CYAN);

        if( edge->faces[0]->markedForDeletion == false)
        {

            // ---- delete face and adjacent faces -----
            facesToDelete.push_back(edge->faces[0]);
            edge->faces[0]->markedForDeletion = true;

        }
        if( edge->faces[1]->markedForDeletion == false)
        {

            // ---- delete face and adjacent faces -----
            facesToDelete.push_back(edge->faces[1]);
            edge->faces[1]->markedForDeletion = true;
        }

        //jointModel->ClearNeighourAndEdgeData();
      //  jointModel->ReconstructMeshDataStructure();
     if( counter2 == 0)
        break;

    }

    for (int i = jointModel->triangles.size() -1; i >= 0; i-- )
    {
        Face* face = jointModel->triangles[i];
        for (int m = facesToDelete.size() -1; m >= 0; m-- )
        {
            if( face == facesToDelete[m] )
            {
                jointModel->triangles.erase(jointModel->triangles.begin() + i);
                facesToDelete.erase(facesToDelete.begin() + m);
                delete face;
                break;
            }
        }
    }

    // cout << "---" <<endl;

    return AtleastOneCollapse;

}

void RetriangulateSharpEdges(Mesh* jointModel)
{
  //  int counter = Count;
    for ( int m =0; m < 30; m++ )
    {
        float gratesAbs = 1;
        Edge* sharpestEdge = NULL;
 //cout << "dpt "  << endl;
        for (uint i = 0; i < jointModel->edges.size(); i++ )
        {
            Edge* edge = jointModel->edges[i];
            Face* A = edge->faces[0];
            Face* B = edge->faces[1];

            if(A == NULL || B== NULL)
                continue;

            //        if(A->reTriangulated|| B->reTriangulated)
            //            continue;

            float dot =( A->normal.dotProduct(B->normal ));
           //

            if(dot < gratesAbs)
            {
                //                Vertex* c = NULL;
                //                Vertex* d = NULL;
                //                for ( int j = 0; j < 3; j++  )
                //                {
                //                    if( A->vertices[j] != a && A->vertices[j] != b )
                //                        c = A->vertices[j];

                //                    if( B->vertices[j] != a && B->vertices[j] != b )
                //                        d = B->vertices[j];
                //                }
                gratesAbs = dot;
                sharpestEdge = edge;
            }
        }

        if( sharpestEdge != NULL)
        {
//cout << "dpt " << gratesAbs << endl;
            Edge* edge = sharpestEdge;
            Face* A = edge->faces[0];
            Face* B = edge->faces[1];
              AddLine(edge->vertices[0]->position,  edge->vertices[1]->position,  MAGENTA);
            // AddPoint( (A->vertices[0]->position + A->vertices[1]->position + A->vertices[2]->position )/3.0f, MAGENTA );
            // AddPoint( (B->vertices[0]->position + B->vertices[1]->position + B->vertices[2]->position )/3.0f, MAGENTA );
//break;
            Vertex* a = edge->vertices[0];
            Vertex* b = edge->vertices[1];
            Vertex* c = NULL;
            Vertex* d = NULL;
            for ( int j = 0; j < 3; j++  )
            {
                if( A->vertices[j] != a && A->vertices[j] != b )
                    c = A->vertices[j];

                if( B->vertices[j] != a && B->vertices[j] != b )
                    d = B->vertices[j];
            }
            Vector3f cd = d->position - c->position;
            Vector3f ca = a->position - c->position;
            Vector3f cb = b->position - c->position;

            Vector3f norm1= cd.crossProduct(ca);
                    norm1.normalize();
              Vector3f norm2= cb.crossProduct(cd);

              norm2.normalize();

              float newdot =norm1.dotProduct(norm2);
              cout << newdot << endl;

              if(newdot < gratesAbs)
              {
                  for (uint i = 0; i < jointModel->edges.size(); i++ )
                  {
                      if( jointModel->edges[i] ==sharpestEdge );
                        jointModel->edges.erase(jointModel->edges.begin() + i);
                  }
                  continue;
              }
            //AddLine(c->position, d->position, RED);

            // ----- change faces-------
            A->SetVertices(a, d, c );
            B->SetVertices(b, c, d );

            A->reTriangulated = true;
            B->reTriangulated = true;

            // ----- delete old faces ---------
            //                for ( int j = 0; j <  A->edges.size(); j++  )
            //                    A->edges[j] = NULL;

            //                for ( int j = jointModel->triangles.size() - 1; j <= 0; j--  )
            //                {
            //                    if( jointModel->triangles[j] == A )
            //                        delete jointModel->triangles[j];

            //                    if( jointModel->triangles[j] == B )
            //                        delete jointModel->triangles[j];

            //                }

            jointModel->ClearNeighourAndEdgeData();
            jointModel->ReconstructMeshDataStructure();
            jointModel->CalculateNormals();



        }

        //          if(counter <= 0)
        //              break;
        //          counter--;
    }


}


Vertex* findVertexForEdge(Edge* rEdge, Vector3f boundaryCenter)
{


    Vertex* v1 = rEdge->vertices[0];
    Vertex* v2 = rEdge->vertices[1];

    // get vertices visible to both

    vector<Vertex*> sharedVerts;

    vector<Vertex*>& visible1 =  v1->visible;
    vector<Vertex*>& visible2 =  v2->visible;

    for ( unsigned int i = 0; i < visible1.size(); i++)
    {

        if( faces == 2 )
        {
            //    AddPoint( visible1[i]->finalPosition, BLUE );
            // AddPoint( v1->finalPosition, GREEN );
        }

        vector<Vertex*>::iterator result = find (visible2.begin(), visible2.end(), visible1[i]);
        if( result !=  visible2.end())
        {
            sharedVerts.push_back(visible1[i]);


        }



    }

    // cout << faces<< endl;

    Vertex* bestVert = NULL;
    float largestAngle = -1;

    // cout << "dots" << endl;
    for ( unsigned int i = 0; i < sharedVerts.size(); i++)
    {
        Vertex* rVert = sharedVerts[i];


        Vector3f edge1 = v1->position - rVert->position;
        Vector3f edge2 = v2->position - rVert->position;
        edge1.normalize();
        edge2.normalize();



        // check the angle between the normal of the polygon and the boundary normal
        Vector3f normal = -edge2.crossProduct( edge1);
        normal.normalize();

        Vector3f bCenterNormal = boundaryCenter - rVert->position;
        bCenterNormal.normalize();


        float dotNormals = normal.dotProduct(bCenterNormal);

        //  // Rule3: Find vert with largest opposite angle
        //    cout<< faces << endl;

        //if( faces == 1 )
        // AddRay( rVert->position,normal*0.05f, GREEN );

        if( faces == 2 )
        {
            //AddLine(rEdge->vertices[0]->position, rVert->position, BLACK);
            // AddLine(rEdge->vertices[1]->position, rVert->position, BLACK);
            //            AddLine(rEdge->vertices[1]->position,rEdge->vertices[0]->position, GREEN);
            //            AddPoint( rEdge->vertices[0]->position, GREEN );
            //            AddPoint( rEdge->vertices[1]->position, GREEN );


            //    AddText( dotNormals, rVert->position, RED );
        }

        //// Rule2:
        if (dotNormals > 0.00f)
            continue;






        float dot = edge1.dotProduct(edge2);

        float angle = acos( dot );


        //  if( faces == 2 )

        //  AddText( angle, rVert->position, RED );

        if (angle > largestAngle)
        {
            largestAngle = angle;
            bestVert = rVert;

        }
    }

    return bestVert;
}

int FindEdge(vector<Edge*>& allEdges, Vertex* A, Vertex* B)
{
    for( unsigned int i = 0; i < allEdges.size(); i++)
    {
        Edge* edge = allEdges[i];
        if( edge->vertices[0] == A && edge->vertices[1] == B )
            return i;


        if( edge->vertices[0] == B && edge->vertices[1] == A )
            return i;
    }

    return -1;
}



int FindVertex(vector<Vertex*>& vertices, Vertex* A)
{
    for( unsigned int i = 0; i < vertices.size(); i++)
    {
        if( vertices[i] == A)
            return i;

    }

    return -1;
}

extern bool tightJoints;

bool retriangulate = false;

void GenerateJoint(vector< vector<Vertex*>* >& branches, Vector3f center, Mesh* model)
{
    bool quit = false;

    faces = Count;

    // ------------------- data structures -----------------
    vector< vector<Vertex*>* > rootBoundaryLoops; // dont forget to delete the internal vector
    vector<Vertex*> rootVertices;
    vector<Edge*> distinceEdges;
    vector<Edge*> allEdgesCreated;

    // ----------------- get the root vertices and the the root loops ----------------
    getRootBoundaryLoopsAndRootVertices(branches, rootBoundaryLoops, rootVertices);

    Mesh* jointModel = new Mesh();
    for (unsigned int k = 0; k < rootVertices.size(); k++)
        jointModel->vertices.push_back( rootVertices[k] );


    // ------------------ initialize the visible vertices to everything -------------
    for (unsigned int k = 0; k < rootVertices.size(); k++)
    {
        rootVertices[k]->calculateVisible(rootVertices);


        rootVertices[k]->Id = k;
    }



    // ------------------- Initialize Edges----------------------------
    stringstream sstream;
    for (unsigned int i = 0; i < rootBoundaryLoops.size(); i++)
    {
        vector<Vertex*>& loop = *rootBoundaryLoops[i];
        for (unsigned int k = 0; k < loop.size(); k++)
        {
            // get the vertices
            Vertex* v1 = loop[k];
            Vertex* v2 = loop[(k + 1) % loop.size()];
            Edge* edge = new Edge (v1, v2);
            distinceEdges.push_back(edge);
            allEdgesCreated.push_back(edge);
            v1->loopIndex = i;
            sstream << k;
            sstream >> v1->loopID;
        }

    }

    // ------------------------- Begin triangulation-----------------------------


    // random loop, must eventually become a conditional. Just not sure what the condition is :P
    bool testNormals = true;

    for (int i = 0; i < 5; i++)
    {
        bool finished = true;
        for (int n = distinceEdges.size() - 1; n >= 0; n--)
        {
            if(distinceEdges[n]->faceCount < 2 )
            {
                finished = false;
                break;
            }
        }

        if( finished )
            break;

        if( quit )
            break;

        //   DebugClear();

        bool atLeastOnVertexFound = false;
        // ------------------ For every loop -----------------------
        for (unsigned int m = 0; m < rootBoundaryLoops.size(); m++)
        {
            vector<Vertex*>& loop = *rootBoundaryLoops[m];

            if( quit )
            {
                break;
            }
            // ------- calculate the loop normal for this iteration -------
            vector<Edge*> newEdgeLoop;

            for (unsigned int k = 0; k < loop.size(); k++)
            {

                if( quit )
                {
                    break;
                }

                // ----------- Select 2 vertices from the edge loop ---------------
                Vertex* v1 = loop[k];
                Vertex* v2 = loop[(k + 1) % loop.size()];




                // ----------- find the edge that consists of these vertices ---------------
                int edgeIndex1 = FindEdge(distinceEdges, v1, v2 );

                if(  edgeIndex1 == -1)
                {

                    cout << "negative edge" << v1->position << " | " << v2->position << " " << v1 << " " << v2 << endl;

                    continue;
                }



                Edge* edge1 = distinceEdges[edgeIndex1];
                newEdgeLoop.push_back(edge1);                           // we assume it will remain in the loop


                // ----------- if the edge already has 2 faces it is done ------------------
                if (edge1->faceCount > 1)
                {
                    v1->tabooVertices.push_back(v2);
                    v2->tabooVertices.push_back(v1);
                    continue;
                }

                // ------------------ apply taboo to update visible -----------------
                v1->applyTabooToVisible();
                v2->applyTabooToVisible();

                // -------------------- select a third vertex ---------------------------
                Vertex* v3 = findVertexForEdge(edge1, center);


                if (v3 == NULL)
                {
                //    cout << "no vert found" << endl;
                    continue;
                }
                atLeastOnVertexFound = true;

                if(enableProgressControl)
                {

                    //                    if (faces == 1)
                    //                    {
                    //                        //  AddLine(rEdge->vertices[1]->position,rEdge->vertices[0]->position, YELLOW);

                    //                        //cout << m << endl;
                    //                        // AddPoint(v2->position, YELLOW);
                    //                        //  AddPoint(v1->position, YELLOW);
                    //                      //  AddLine(v1->position, v3->position, YELLOW);
                    //                      //  AddLine(v2->position, v3->position, YELLOW);
                    //                       // break;
                    //                    }



                }


                // ------------- Check if edges already exist ----------------------
                int edgeIndex2 = FindEdge(distinceEdges, v1, v3 );
                int edgeIndex3 = FindEdge(distinceEdges, v3, v2 );

                // mem leak
                Edge* edge2 = new Edge(v1, v3);
                Edge* edge3 = new Edge(v3, v2);

                allEdgesCreated.push_back(edge2);
                allEdgesCreated.push_back(edge3);


                // ----------- if the edges arent found then instantiate them -------------
                if( edgeIndex2 != -1 )
                    edge2->faceCount = distinceEdges[edgeIndex2]->faceCount;

                if( edgeIndex3 != -1 )
                    edge3->faceCount = distinceEdges[edgeIndex3]->faceCount;


                // -------- either of the edges have 2 faces then they cant be used -----------------
                if ( edgeIndex2 != -1 && edge2->faceCount > 1)
                {
                    v1->tabooVertices.push_back(v3);
                    v3->tabooVertices.push_back(v1);
                    continue;
                }
                else if (  edgeIndex3 != -1 && edge3->faceCount > 1)
                {
                    v2->tabooVertices.push_back(v3);
                    v3->tabooVertices.push_back(v2);
                    continue;
                }

                // the polygon is valid. Thunder birds are go!
                jointModel->triangles.push_back( new Face(v1, v2, v3 ));

                // increment face counts or add the edges
                edge1->faceCount++;

                if (edgeIndex2 != -1)
                    distinceEdges[edgeIndex2]->faceCount++;
                else
                    distinceEdges.push_back(edge2);

                if (edgeIndex3 != -1)
                    distinceEdges[edgeIndex3]->faceCount++;
                else
                    distinceEdges.push_back(edge3);

                int edgeIndex1_newEdgeLoop = FindEdge(newEdgeLoop, v1, v2 );
                newEdgeLoop.erase(newEdgeLoop.begin() + edgeIndex1_newEdgeLoop);
                newEdgeLoop.push_back(edge2);
                newEdgeLoop.push_back(edge3);


                // ----------------- apply rule1 ---------------------
                for ( uint n = 0; n < rootBoundaryLoops.size(); n++)
                {
                    uint len = rootBoundaryLoops[n]->size();

                    if (n != m)
                    {
                        vector<Vertex*>& otherLoop = *rootBoundaryLoops[n];

                        int index = FindVertex( otherLoop, v3);
                        if (index != -1)
                        {
                            Vertex* left = otherLoop[(index - 1 + len) % len];
                            Vertex* right = otherLoop[(index + 1) % len];

                            if( faces == 2 )
                            {
                                //                            AddPoint(left->finalPosition, RED);
                                //                            AddPoint(v1->finalPosition, RED);
                                //                            AddPoint(right->finalPosition, BLACK);
                                //                            AddPoint(v2->finalPosition, BLACK);
                            }
                            v1->tabooVertices.push_back(left);
                            v2->tabooVertices.push_back(right);
                            left->tabooVertices.push_back(v1);
                            right->tabooVertices.push_back(v2);
                            //  }
                        }
                    }
                }

                if( enableProgressControl  )
                {
                    faces--;

                    if( faces == 1 )
                        quit = true;
                }

            }

            if( atLeastOnVertexFound == false)
                testNormals = false;




            // ------ reconstruct loop with from new edges -----------
            loop.clear();
            int length =  newEdgeLoop.size();
            for (int n = 0; n < length; n++)
            {
                Edge* left = newEdgeLoop[(n - 1 +length) % length];
                Edge* current = newEdgeLoop[n];
                Edge* right = newEdgeLoop[(n + 1) % length];

                // ------------- remove any edges that may have folded in on themselves ----------
                if (current->AreEqual(right) == false && current->AreEqual(left) == false)
                    loop.push_back(current->vertices[0]);
            }
        }
    }

    // --------- restore joint to tighrest form --------
    if( tightJoints )
    {
        for (unsigned int i = 0; i <branches.size(); i++)
        {
            vector< Vertex*>&  loop = *branches[i];
            for (unsigned int j = 0; j <loop.size(); j++)
                loop[j]->position = loop[j]->finalPosition;
        }
    }


    jointModel->ReconstructMeshDataStructure();
    jointModel->CalculateNormals();

    if( retriangulate)
    {
        RetriangulateSharpEdges(jointModel );
    }

//    int counter = Count;

    counter2 = Count;
    for( int n = 0; n < 20; n++)
    {
//break; ///////////////////////////////////////////////////////////////// ----------------------------------------------
       // if( counter <= 0 )
       //     break;
       model->ClearNeighourAndEdgeData();
       jointModel->ClearNeighourAndEdgeData();
       model->ReconstructMeshDataStructure();
       jointModel->ReconstructMeshDataStructure();
        bool unstable = CollapseJoint( jointModel, model );
       // cout << unstable << endl;
        if( unstable == false)
            break;

        if( n == 10)
            cout << "warning: too many joint collapse" << endl;
    }
    // cout << "EEEEE" << endl;
    //  for( uint i = 0; i < jointModel->edges.size(); i++)
    // AddLine( jointModel->edges[i]->vertices[0]->position, jointModel->edges[i]->vertices[1]->position, RED);


    // --------- Triangles from joint into main model --------
    for (unsigned int i = 0; i < jointModel->triangles.size(); i++)
        model->triangles.push_back( jointModel->triangles[i] );

    // ------------- Must clear these arrays before deleting the models as they exist in the main model ------
    jointModel->vertices.clear();
    jointModel->triangles.clear();
    jointModel->quads.clear();
    delete jointModel;

    for (unsigned int i = 0; i < rootBoundaryLoops.size(); i++)
        delete rootBoundaryLoops[i];

    for (unsigned int i = 0; i < allEdgesCreated.size(); i++)
        delete allEdgesCreated[i];
}
