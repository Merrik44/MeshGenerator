#ifndef LSTSKELETON_H
#define LSTSKELETON_H
#include "string"
#include "vector"
#include "vmath.h"
#include "branchnode.h"

class LstSkeleton
{
public:
    LstSkeleton(std::string filepath);
    ~LstSkeleton( );
    void LoadLstFile(std::string filepath );
    void Draw();
    bool TryCollapseBranch( BranchNode* branch );
    void CalculateBranchOffsets( BranchNode& branch);
    void SimplifyGraph();
    float scale;
    std::string filepath;

    BranchNode* root;
    std::vector< BranchNode* > branches;
    std::vector< BranchNode* > branchTips;

};

#endif // LSTSKELETON_H
