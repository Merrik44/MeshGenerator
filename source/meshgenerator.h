#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include "lstskeleton.h"
#include "mesh.h"
#include "branchnode.h"
#include "vector"



Mesh* generateMesh( std::vector< BranchNode*>& branches);
Vertex* mergeAwithB2( Vertex* A, Vertex* B, int bWeight );

#endif // MESHGENERATOR_H
