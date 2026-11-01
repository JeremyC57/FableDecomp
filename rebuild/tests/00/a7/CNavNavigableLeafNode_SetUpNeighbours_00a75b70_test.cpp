// Behaviour test for CNavNavigableLeafNode::SetUpNeighbours (0x00A75B70): the neighbour on side S is told to set up its
// neighbours with this leaf on the opposite side (0<->1, 2<->3).
#include <cstdio>
enum ENavQuadTreeNeighbours { NAV_NEIGHBOUR_0 = 0, NAV_NEIGHBOUR_1 = 1, NAV_NEIGHBOUR_2 = 2, NAV_NEIGHBOUR_3 = 3 };
class CANavQuadTreeNode {
    friend class CNavNavigableLeafNode;
public:
    CANavQuadTreeNode() : LastSide(-1), LastLeaf(0), Calls(0) { }
    virtual ~CANavQuadTreeNode() { }
    int LastSide;
    CANavQuadTreeNode* LastLeaf;
    int Calls;
protected:
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf)
    {
        LastSide = side;
        LastLeaf = leaf;
        ++Calls;
    }
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours, CANavQuadTreeNode*) { }
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    void Run(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node) { SetUpNeighbours(side, node); }
protected:
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node);
};
void CNavNavigableLeafNode::SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node)
{
    ENavQuadTreeNeighbours opposite = NAV_NEIGHBOUR_0;
    switch (side) {
    case NAV_NEIGHBOUR_3: opposite = NAV_NEIGHBOUR_2; break;
    case NAV_NEIGHBOUR_2: opposite = NAV_NEIGHBOUR_3; break;
    case NAV_NEIGHBOUR_1: opposite = NAV_NEIGHBOUR_0; break;
    case NAV_NEIGHBOUR_0: opposite = NAV_NEIGHBOUR_1; break;
    }
    node->SetUpNeighboursWithLeaf(opposite, this);
}
int main()
{
    static const int expected[4] = { 1, 0, 3, 2 };   // retail jump table 0x00A75BD0
    for (int side = 0; side < 4; ++side) {
        CNavNavigableLeafNode leaf;
        CANavQuadTreeNode neighbour;
        leaf.Run((ENavQuadTreeNeighbours)side, &neighbour);
        if (neighbour.Calls != 1 || neighbour.LastSide != expected[side] || neighbour.LastLeaf != &leaf || leaf.Calls != 0) {
            std::printf("00a75b70_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00a75b70_TEST PASS\n");
    return 0;
}