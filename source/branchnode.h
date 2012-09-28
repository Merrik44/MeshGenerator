#ifndef BRANCH_H
#define BRANCH_H

#include "vmath.h"
#include "vector"
#include "mesh.h"

class BranchNode
{
public:

    Vector3f startPosition;
    Vector3f endPosition;
    float startRadius;
    float endRadius;
    float length;
    std::vector<BranchNode*> children;
    BranchNode* parent;
    Matrix3f rotation;
    Vector3f direction;
    std::vector< Vertex*> startVertices;
    std::vector< Vertex*>  endVertices;

    float startOffset;
    float endOffset;
    float tightStartOffset;
    float tightEndOffset;



    BranchNode( Vector3f StartPosition,  Matrix3f Rotation , float StartRadius, float EndRadius, float Length, BranchNode* Parent)
    {
        startPosition = StartPosition;
        startRadius = StartRadius;
        endRadius = EndRadius;
        length = Length;
        parent = Parent;
        rotation =Rotation;
        if( parent != NULL )
            parent->children.push_back(this);

        // calculate end pos
        direction = rotation*Vector3f( 0, 1, 0 );;
        endPosition = direction*length + startPosition;

        startOffset = endOffset = 0;
    }
};


#endif // BRANCH_H
