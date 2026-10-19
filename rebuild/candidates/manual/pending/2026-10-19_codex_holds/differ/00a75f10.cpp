// Grade: HELD(bytes match retail) -- test has no virtual dispatch; argument type differs from PDB (ENavQuadTreeNeighbours) | Codex source 00/a7/CNavQuadTreeNode_SetUpNeighboursWithLeaf_00a75f10.cpp, flags , pass pattern 00a75f10_TEST PASS
// CNavQuadTreeNode::SetUpNeighboursWithLeaf(CNavQuadTreeNode::ENavQuadTreeNeighbours,CANavQuadTreeNode*) -- connect two children to a supplied leaf on the selected side
// Retail: Fable.exe @ 0x00A75F10, 0x52 bytes. Four enum cases map each side to its two adjacent children.
// Leaf: SetUpNeighboursWithLeaf
struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual bool MakeRegion(void*, void*); virtual bool Add(void*, void*);
    virtual void v14(); virtual void v15();
    virtual void SetNeighbour(long side, CANavQuadTreeNode* node);
};
struct CNavQuadTreeNode : CANavQuadTreeNode {
    unsigned char pad[0xc];
    CANavQuadTreeNode* child[4];
    virtual void SetUpNeighboursWithLeaf(long side, CANavQuadTreeNode* leaf);
};
void CNavQuadTreeNode::SetUpNeighboursWithLeaf(long side, CANavQuadTreeNode* leaf)
{
    switch (side) {
    case 0: child[1]->SetNeighbour(2, leaf); child[3]->SetNeighbour(2, leaf); break;
    case 1: child[0]->SetNeighbour(3, leaf); child[2]->SetNeighbour(3, leaf); break;
    case 2: child[0]->SetNeighbour(1, leaf); child[1]->SetNeighbour(1, leaf); break;
    case 3: child[2]->SetNeighbour(0, leaf); child[3]->SetNeighbour(0, leaf); break;
    default: break;
    }
}