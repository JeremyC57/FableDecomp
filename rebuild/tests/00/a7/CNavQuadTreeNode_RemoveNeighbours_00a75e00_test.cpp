// Test: CNavQuadTreeNode::RemoveNeighbours() recurses into TL, TR, BL, BR in that order.
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CANavQuadTreeNode { public:
    int Id;
    virtual ~CANavQuadTreeNode() { }
    virtual void RemoveNeighbours() { g_order[g_count++] = Id; }
};
class CNavQuadTreeNode : public CANavQuadTreeNode { public:
    virtual void RemoveNeighbours();
    CANavQuadTreeNode* TLNode;
    CANavQuadTreeNode* TRNode;
    CANavQuadTreeNode* BLNode;
    CANavQuadTreeNode* BRNode;
};
void CNavQuadTreeNode::RemoveNeighbours()
{
    TLNode->RemoveNeighbours();
    TRNode->RemoveNeighbours();
    BLNode->RemoveNeighbours();
    BRNode->RemoveNeighbours();
}
int main()
{
    CANavQuadTreeNode tl, tr, bl, br;
    tl.Id = 1; tr.Id = 2; bl.Id = 3; br.Id = 4;
    CNavQuadTreeNode node;
    node.Id = 0;
    node.TLNode = &tl; node.TRNode = &tr; node.BLNode = &bl; node.BRNode = &br;
    CANavQuadTreeNode* p = &node;
    p->RemoveNeighbours();
    if (g_count != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4) { std::printf("00a75e00_TEST FAIL\n"); return 1; }
    std::printf("00a75e00_TEST PASS\n");
    return 0;
}