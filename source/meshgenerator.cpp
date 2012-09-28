#include "meshgenerator.h"
#include "stack"
#include "vmath.h"
#include "iostream"
#include "vector"
#include "debug.h"
#include "glwidget.h"
#include "math.h"
#include "algorithm"
#include "jointgenerator.h"
#include "mainwindow.h"

using namespace std;


#define radToDegree 57.2957795
int Count = 3;
int countdown = 3;
int faces;
float SubDivControl = 0.5f;

bool display4Texture = false;
bool displayMesh = true;
bool displayGraph = true;
bool tightJoints = true;
bool alternatingTriangulation = false;
int noOfSides = 8;

void InterpolateTexCoordsAccrosRemainingFaces( Mesh* model )
{
    for( uint i = 0; i < model->triangles.size(); i++ )
    {
        Face* face = model->triangles[i];
        if( face->texCoords[0] != Vector2f(0,0) || face->texCoords[1] != Vector2f(0,0) ||
                face->texCoords[2] != Vector2f(0,0) || face->texCoords[3] != Vector2f(0,0)) // face is already parameterised so continue
            continue;

       // AddLine( face->vertices[0]->position,  face->vertices[1]->position, YELLOW);
       //  AddLine( face->vertices[1]->position,  face->vertices[2]->position, YELLOW);
       //   AddLine( face->vertices[2]->position,  face->vertices[0]->position, YELLOW);

        // find longest edge with tex coords
        for( uint j = 0; j < face->edges.size(); j++ )
        {

            Edge* edge = face->edges[j];
            if(edge->faces[0] == NULL || edge->faces[1] == NULL  )
                continue;

            // ------ find opposite face ----------
            Face* oppositeFace = NULL;

            if(edge->faces[0] == face ) oppositeFace = edge->faces[1] ;
            if(edge->faces[1] == face ) oppositeFace = edge->faces[0] ;

            // ------ check that it has tex coords ------
            if( oppositeFace->texCoords[0] == Vector2f(0,0) && oppositeFace->texCoords[1] == Vector2f(0,0) &&
                    oppositeFace->texCoords[2] == Vector2f(0,0) && oppositeFace->texCoords[3] == Vector2f(0,0))

                continue;

            // ------ find corrosponding verts in opposite face ----------
            for( uint a = 0; a < 3; a++ )
            {
                face->texCoords[a] = Vector2f(-1, -1);
                for( uint k = 0; k < 3; k++ )
                {

                    if( oppositeFace->vertices[k] == face->vertices[a] )
                    {
                        face->texCoords[a] = oppositeFace->texCoords[k] ;
                        AddPoint( oppositeFace->vertices[k]->position, RED);
                    }
                }
            }

            // --- interpolate the missing coord ---
            int indexOfMissingCoord = -1;
            for( uint a = 0; a < 3; a++ )
            {

                if( face->texCoords[a] == Vector2f(-1, -1) )
                    indexOfMissingCoord = a;
            }



            int bIndex = (indexOfMissingCoord + 1)%3;
            int cIndex = (indexOfMissingCoord + 2)%3;

            Vector2f& texA =face->texCoords[indexOfMissingCoord];
            Vector2f& texB =face->texCoords[bIndex];
            Vector2f& texC =face->texCoords[cIndex];

            Vertex* vertA =face->vertices[indexOfMissingCoord];
            Vertex* vertB =face->vertices[bIndex];
            Vertex* vertC =face->vertices[cIndex];

            Vector3f BA = vertA->position - vertB->position;
            Vector3f BC =  vertC->position - vertB->position;
            Vector3f CA =  vertA->position - vertC->position;

            float lengthBA = BA.length();
           float lengthBC = BC.length();
           float lengthCA = CA.length();

            BA/=lengthBA;
            BC /=lengthBC;
            CA /=lengthCA;
            float theta = acos(BC.dotProduct(BA));
            //float angleC = acos(CA.dotProduct(-BC));

            Vector2f bc = texC - texB;


            float length_bc = BC.length();
            float length_ba = (lengthBA*length_bc)/lengthBC;

            bc /= length_bc;
            bc *= length_ba;

            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            Vector2f ba;
            ba.x = bc.x*cosTheta - bc.y*sinTheta;
            ba.y = bc.x*sinTheta + bc.y*cosTheta;

            texA = texB + ba;
            //Vector2f zero(0,0);
            //face->SetTextureCoords( texA, texB, texC, zero  );
            break;
        }

    }
}


void CreateLoop( int sides, float radius, Matrix3f rotation, Vector3f offset, float texY,
                 Mesh* model, vector< vector< Vertex*>*>& vertexLoops, vector< vector< Vector2f>* >& texLoops )
{
    vector<Vertex*>* loop = new vector<Vertex*>(sides);
    vector<Vector2f>* texLoop = new vector<Vector2f>(sides);

    for (int i = 0; i < sides; i++)
    {
        float interval = PI*2/sides;
        float x = cos( interval*i);
        float z = sin( interval*i);

        Vector3f position(x, 0, z);



        position *= radius;
        position = rotation*position;

        position += offset;


        (*loop)[i] = new Vertex( position, model->vertices.size() );

        model->vertices.push_back( (*loop)[i] );

        float texX = (float)i/(sides);

        float tileY = 1;
        (*texLoop)[i] = Vector2f(   texX, texY*tileY );
    }

    vertexLoops.push_back(loop);
    texLoops.push_back(texLoop);
}

void GenerateFaces(  Mesh* model, vector< vector< Vertex*>*>& vertexLoops, vector< vector< Vector2f>* >& texLoops  )
{
    int sides = noOfSides;


    for( uint m = 0; m <vertexLoops.size() -1; m++ )
        //for( int m = 0; m < 1; m++ )
    {

        vector< Vertex*>& startLoop = *vertexLoops[m];
        vector< Vertex*>& endLoop = *vertexLoops[m+1];
        vector< Vector2f>&  startTexLoop = *texLoops[m];
        vector< Vector2f>&  endTexLoop = *texLoops[m+1];
        for (int i = 0; i < sides; i++)
        {


            Vertex* A = startLoop[i];
            Vertex* B = startLoop[(i+1)%sides];
            Vertex* C = endLoop[i];
            Vertex* D = endLoop[(i+1)%sides];

            Vector2f texA   = startTexLoop[i];
            Vector2f texB   = startTexLoop[(i+1)%sides];
            Vector2f texC   = endTexLoop[i];
            Vector2f texD   = endTexLoop[(i+1)%sides];
            Vector2f zero;

            // handle wrapping textures
            if( i == sides - 1)
            {
                texB.x +=1;
                texD.x +=1;
            }

            if( texC.y == 0)
            {
                texC.y =1;
                texD.y =1;
            }


            Face * ABC ;
            Face * BCD ;
            if(alternatingTriangulation && i%2 == 1)
            {
                ABC = new Face( B, A, D ) ;
                BCD = new Face( A, C, D ) ;


                ABC->SetTextureCoords( texB,  texA, texD, zero);
                BCD->SetTextureCoords( texA,texC, texD, zero);
            }
            else
            {
                ABC = new Face( B, A, C ) ;
                BCD = new Face( B, C, D ) ;


                ABC->SetTextureCoords( texB,  texA, texC, zero);
                BCD->SetTextureCoords( texB,texC, texD, zero);
            }

            model->triangles.push_back( ABC );
            model->triangles.push_back( BCD );
        }

    }

}

// TO DO: offset radius when the positions are offset

void GenerateBranch( Mesh* model, BranchNode* branch )
{
    int sides = noOfSides;

    // ------- If the branch has no children, then merge the tip points --------
    //    if( branch->children.size() == 0)
    //        branch->endRadius = 0;


    // ------- generate boundary loop vertices -------

    vector< vector< Vector2f>* > texLoops;
    vector< vector< Vertex*>*> vertexLoops;

    Vector3f offset;
    float ratio = 0;
    float radius;
    float averageRadius =(branch->startRadius + branch->endRadius)/2.0f;
    int noOfSegments = ceil(branch->length/(averageRadius*PI));

    //CreateLoop( sides, branch->startRadius, branch->rotation, branch->startPosition, 0, model, vertexLoops, texLoops );

    Vector3f startPos = branch->startPosition + branch->direction*branch->startOffset;
    Vector3f endPos = branch->endPosition - branch->direction*branch->endOffset;
    //   cout << branch->endOffset << endl;
    //float startRadius = branch->startRadius

    // ---------- Clamp the control factor ------------
    float controlFactor = min(SubDivControl, 0.5f);
    controlFactor = max( 0.01f, controlFactor);

    // ======================================

    for( int i = 0; i < noOfSegments+1; i++)
    {
        ratio = (float)i/((float)noOfSegments);

        float vTexCoordinate = 0;
        if(i%2 == 0 )
            vTexCoordinate = 0.5f;

        offset =  startPos + (endPos -  startPos)*ratio;

        radius = branch->startRadius + (branch->endRadius - branch->startRadius )*ratio;

        CreateLoop( sides, radius, branch->rotation,offset, vTexCoordinate, model, vertexLoops, texLoops );
    }
    // ======================================

    // ---------- create the starting loop -------------
    // CreateLoop( sides, branch->startRadius, branch->rotation, startPos, 0, model, vertexLoops, texLoops );


    // ---------- create the middle control loops -------------
    //    if(controlFactor < 0.5f   )
    //    {
    //        ratio = controlFactor;
    //        offset =  startPos + (endPos -  startPos)*ratio;

    //        radius = branch->startRadius + (branch->endRadius - branch->startRadius )*ratio;

    //        CreateLoop( sides, radius, branch->rotation, offset, ratio, model, vertexLoops, texLoops );
    //    }
    //    ratio = 0.5f;
    //    offset =  startPos + (endPos -  startPos)*ratio;
    //    radius = branch->startRadius + (branch->endRadius - branch->startRadius )*ratio;

    //    CreateLoop( sides, radius, branch->rotation, offset, ratio, model, vertexLoops, texLoops );

    //    // ---------- if the factor is below 0.5f the create a secont, mirrored loop -------------
    //    if(controlFactor < 0.5f   )
    //    {

    //        ratio = 1-controlFactor;
    //        offset =  startPos + (endPos -  startPos)*ratio;
    //        radius = branch->startRadius + (branch->endRadius - branch->startRadius )*ratio;

    //        CreateLoop( sides, radius, branch->rotation, offset, ratio, model, vertexLoops, texLoops );
    //    }

    //    // ---------- create the end loop -------------
    //    CreateLoop( sides, branch->endRadius, branch->rotation, endPos, 1, model, vertexLoops, texLoops );

    // ======================================


    // ------------- generate faces --------------
    GenerateFaces(   model,vertexLoops, texLoops );

    model->ClearNeighourAndEdgeData();
    model->ReconstructMeshDataStructure();



    // ------ Assign start and end loops to branches ------
    vector< Vertex*>& startVertices = branch->startVertices;
    vector< Vertex*>&  endVertices = branch->endVertices;

    vector< Vertex*>  start =  *vertexLoops[0];
    startVertices.clear();
    for( unsigned int i = 0; i < start.size(); i++)
        startVertices.push_back(start[i]);

    vector< Vertex*>  end =  *vertexLoops[vertexLoops.size() - 1];
    endVertices.clear();
    for( unsigned int i = 0; i < end.size(); i++)
        endVertices.push_back(end[i]);

    // ---- reverse loop joint creation algorithm so that all the boundaries have the same clockwise ordering ---
    std::reverse(endVertices.begin(), endVertices.end());
}


Vertex* mergeAwithB2( Vertex* A, Vertex* B, int bWeight )
{
    // ------- Add A to B ---------
    B->position*=bWeight;
    B->normal*=bWeight;


    B->position += A->position;
    B->normal += A->normal;

    // ------ Normalize B, which now represents the merged point-----
    B->position /= bWeight +1;
    B->normal /= bWeight +1;


    // ----- update the faces that were pointing at A to point to B ----
    for( uint j = 0; j < A->faces.size(); j++)
    {
        Face* face = A->faces[j];
        for( uint k = 0; k < 4; k++)
        {
            if( face->vertices[k] == A)
                face->vertices[k] = B;
        }

    }

    return B;
}



Vector3f TrimIncomingBranches( vector< vector< Vertex* >* >& boundaries, vector< Vector3f >& loopNormals, vector< float >& offsets )
{

    int length = loopNormals.size();

    // --------------- find the furthest offset ------------

    float maxOffset = 0;

    for (int a = 0; a < length; a++)
        maxOffset = max(maxOffset,offsets[a]);

    maxOffset *= 2.05f; // make it a little larger fo luck


    // --------------- for each branch ------------
    for (int a = 0; a < length; a++)
    {
        // --------------off set everything-----------

        vector< Vertex* >& loop = *boundaries[a];

        for (uint i = 0; i < loop.size(); i++)
        {
            Vertex* vertex = loop[i];
            vertex->finalPosition = vertex->position;
            // vertex->position += maxTranslation;
        }
    }

    // --------- calculate loop center -------------------

    vector<Vector3f> loopCenters;
    Vector3f geometricCenter;
    for (unsigned int i = 0; i < boundaries.size(); i++)
    {
        Vector3f loopCenter;
        vector< Vertex* >& loop = *boundaries[i];
        for (unsigned int j = 0; j < loop.size(); j++)
            loopCenter += loop[j]->position;

        loopCenter /= loop.size();
        loopCenters.push_back(loopCenter);


        geometricCenter += loopCenter;
    }

    // ---------- calculate geometric center -----------
    geometricCenter /= boundaries.size();

    // ----------- rotate loops so that they are orthoganal to the geo center -------------------
    for (unsigned int i = 0; i < boundaries.size(); i++)
    {

        Vector3f oldNormal = loopNormals[i];

        Vector3f newNormal = loopCenters[i] - geometricCenter;
        if( newNormal == Vector3f(0,0,0))
            continue;
        else
            newNormal.normalize();


        // ---- update rotation matrix ----
        Vector3f axis = newNormal.crossProduct( oldNormal);
        axis.normalize();
        float dot = newNormal.dotProduct( oldNormal);
        float angle = acos(dot);

        if(dot == 0)
            axis = Vector3f(1, 0, 0);
        if(dot == 1)
            axis = Vector3f(1, 0, 0);


        angle*= radToDegree;

        Quatf rotQuat = Quatf::fromAxisRot(axis, -angle);
        Matrix3f rotation = rotQuat.rotMatrix();

        vector< Vertex* >& loop = *boundaries[i];

        //  Vector3f translation = direction* offsets[a]*1.01f;
        Vector3f maxTranslation = newNormal* maxOffset*1.01;

        AddLine(geometricCenter+ maxTranslation, geometricCenter, BLACK );
        for (unsigned int j = 0; j < loop.size(); j++)
        {
            Vertex* A = loop[j];
            A->position -= loopCenters[i]; // translate to origin
            A->position = rotation *A->position; // rotate
            A->position   += geometricCenter; // move to geo center
            A->position += maxTranslation;

        }


    }

    return geometricCenter;

}

extern int faces, Count;
void MergeTwoBondaries(vector<Vertex*>& loopA, vector<Vertex*>& loopB )
{


    int counter =Count;
    // cout <<  Count << endl;
    // make copies
    vector< Vertex* > loop1(loopA);
    vector< Vertex* > loop2(loopB);
    vector< Vertex* > newLoop;
    // --- this assumes that both loops have the same number of vertices ---
    int iterations= loop1.size();
    for ( int i = 0; i < iterations; i++)
    {
        int closestVertexPair[2] = { -1, -1 };
        float closestSqrDistance = 10000000;
        for ( unsigned int a = 0; a < loop1.size(); a++)
        {
            Vertex* A = loop1[a];
            for ( unsigned int b = 0; b < loop2.size(); b++)
            {
                Vertex* B = loop2[b];

                float sqrDist = (A->position -B->position).lengthSq();
                if( sqrDist < closestSqrDistance )
                {
                    closestSqrDistance = sqrDist;
                    closestVertexPair[0] = a;
                    closestVertexPair[1] = b;
                }
            }
        }

        // escape case
        if(closestVertexPair[0] == -1 )
        {
            cout << "No closest vert found" << endl;
            continue;
        }

        // -- merge vertices ---
        int indexA = closestVertexPair[0];
        int indexB = closestVertexPair[1];
        Vertex* A = loop1[indexA ];
        Vertex* B = loop2[indexB ];

        loop1.erase( loop1.begin() + indexA);
        loop2.erase( loop2.begin() + indexB);

        //    AddLine(A->position, B->position, RED);

        B = mergeAwithB2(A, B, 1);
        newLoop.push_back(B);

        //        cout << counter << endl;
        if( counter < 0)
            return;
        counter--;

    }

    // ---- project onto sphere ---
    //  for

}


MainWindow* mainWindowRef;
Mesh* generateMesh( vector<BranchNode*>& branches)
{
    // ------------------ debug stuff ---------------
    countdown = Count;
    DebugClear();
    if(branches.size() == 0  )
        return NULL;

    // ------------------ create new model ---------------
    Mesh* model = new Mesh();
    stack<BranchNode* > branchStack;

    BranchNode* current;

    // ------------------ create trunk ---------------

    branchStack.push( branches[0] );
    GenerateBranch(model,  branches[0] );


    // ------------------ process the rest of the tree ---------------

    int branchesProcessed =0;
    int noOfBranches = branches.size();
    cout << "processing" << endl;


    while( branchStack.size() > 0 )
    {
        current = branchStack.top();
        branchStack.pop();

        branchesProcessed++;
        mainWindowRef->progressBarUpdate(( branchesProcessed*100)/noOfBranches );

        // ------- if the branch has no children, close tip ----


        // ------ generate a mesh for each child and add it to the stack -------

        for (unsigned int i = 0; i < current->children.size(); i++)
        {
            GenerateBranch(model, current->children[i]);
            branchStack.push(current->children[i]);
        }


        // --------------- create the joint ---------------------------------
        if( current->children.size() != 0)
        {
            model->ClearNeighourAndEdgeData();
            model->ReconstructMeshDataStructure();

            // ----------- prepare the neccesary data for trimming ---------------

            vector< Vector3f > loopNormals;
            vector< float > offsets;
            vector< vector< Vertex* >* > boundaries;


            boundaries.push_back(&current->endVertices);
            loopNormals.push_back(-current->direction);
            offsets.push_back(current->endOffset );


            for (unsigned int i = 0; i < current->children.size(); i++)
            {
                boundaries.push_back( &current->children[i]->startVertices);
                loopNormals.push_back(current->children[i]->direction);
                offsets.push_back(current->children[i]->startOffset );
            }


            // --------- Trim the branches so that the dont intersect --------

            Vector3f geometricCenter = TrimIncomingBranches(boundaries, loopNormals, offsets);

            // --- If there is only one child, then simply merge the closest vertices ---
            if( current->children.size() == 1)
            {
                MergeTwoBondaries( *boundaries[0], *boundaries[1] );

            }

            if( current->children.size() > 1)
            {


                // --------- Finally construct the joint --------

                GenerateJoint(boundaries, geometricCenter,   model);



            }
        }
    }

    // ------ Store the mesh state for subdivision purposes -------
    model->ClearNeighourAndEdgeData();

    model->ReconstructMeshDataStructure();
    model->CalculateNormals();

    InterpolateTexCoordsAccrosRemainingFaces( model ); //-------------- This should probs run till stabl!!!
    model->StoreMeshState();

    // --------------- Clean the mesh ---------------

    //model->StoreTextureCoordInVerticesAndMarkSeams();


    return model;
}


