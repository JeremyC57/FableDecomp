// Test for CNavQuadTreeNode::SetUpNeighbours (0x00A75E30): a leaf neighbour goes to SetUpNeighboursWithLeaf on this node;
// otherwise each side pairs two of this node's children with the neighbour's two facing children (side passed down).
#include <cstdio>
struct Node {
    bool LeafNode; Node* TL; Node* TR; Node* BL; Node* BR;
    int withLeaf; int lastSide; Node* pairs[4]; int npairs;
    Node() : LeafNode(false), TL(0), TR(0), BL(0), BR(0), withLeaf(0), lastSide(-1), npairs(0) {}
    virtual ~Node() {}
    virtual void SetUpNeighboursWithLeaf(int, Node*) { ++withLeaf; }
    virtual void SetUpNeighbours(int side, Node* node)
    {
        if (node->LeafNode) { SetUpNeighboursWithLeaf(side, node); return; }
        switch (side) {
        case 3: TL->Record(3, node->TR); BL->Record(3, node->BR); break;
        case 2: TR->Record(2, node->TL); BR->Record(2, node->BL); break;
        case 1: TL->Record(1, node->BL); TR->Record(1, node->BR); break;
        case 0: BL->Record(0, node->TL); BR->Record(0, node->TR); break;
        }
    }
    void Record(int side, Node* n) { lastSide = side; pairs[npairs++ & 3] = n; }
};
int main()
{
    Node me, other, leaf, c[8];
    me.TL = &c[0]; me.TR = &c[1]; me.BL = &c[2]; me.BR = &c[3];
    other.TL = &c[4]; other.TR = &c[5]; other.BL = &c[6]; other.BR = &c[7];
    leaf.LeafNode = true;
    me.SetUpNeighbours(2, &leaf);
    if (me.withLeaf != 1) { std::printf("00a75e30_TEST FAIL\n"); return 1; }
    me.SetUpNeighbours(0, &other);
    if (c[2].pairs[0] != &c[4] || c[3].pairs[0] != &c[5] || c[2].lastSide != 0) { std::printf("00a75e30_TEST FAIL\n"); return 1; }
    me.SetUpNeighbours(3, &other);
    if (c[0].pairs[0] != &c[5] || c[2].pairs[1] != &c[7] || c[0].lastSide != 3) { std::printf("00a75e30_TEST FAIL\n"); return 1; }
    me.SetUpNeighbours(7, &other);
    if (c[0].npairs != 1 || me.withLeaf != 1) { std::printf("00a75e30_TEST FAIL\n"); return 1; }
    std::printf("00a75e30_TEST PASS\n");
    return 0;
}