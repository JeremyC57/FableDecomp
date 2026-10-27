#include <cstdio>
#include <cstring>
#include <cmath>
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
class CANavQuadTreeNode {
public:
    C2DVector Position; unsigned char QuadTreeLevel; unsigned char NavLayer; bool LeafNode; bool Blocked;
    CANavQuadTreeNode(const C2DVector& p, unsigned char l, unsigned char n) : Position(p), QuadTreeLevel(l), NavLayer(n), LeafNode(true), Blocked(false) { }
    virtual ~CANavQuadTreeNode() { }
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
public:
    CNavBlockedQuadTreeNode(unsigned char l, unsigned char n) : CANavQuadTreeNode(C2DVector(-1.0f, -1.0f), l, n) { Blocked = true; }
};

int main()
{
    CNavBlockedQuadTreeNode b(0, 0);
    if (b.Position.X != -1.0f || b.Position.Y != -1.0f || b.QuadTreeLevel != 0 || b.NavLayer != 0 || !b.LeafNode || !b.Blocked) do { std::printf("012243d0_TEST FAIL\n"); return 1; } while (0);
    std::printf("012243d0_TEST PASS\n");
    return 0;
}