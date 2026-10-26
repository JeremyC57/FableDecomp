#include <cstdio>
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
static int g_init = 0;
struct Base {
    C2DVector Position; unsigned char QuadTreeLevel; unsigned char NavLayer; bool LeafNode; bool Blocked;
    Base(const C2DVector& p, unsigned char l, unsigned char n) : Position(p), QuadTreeLevel(l), NavLayer(n), LeafNode(true), Blocked(false) { }
};
struct Node : Base {
    Base* TL; Base* TR; Base* BL; Base* BR;
    Node(const C2DVector& p, unsigned char l, unsigned char n, int tree) : Base(p, l, n), TL(0), TR(0), BL(0), BR(0) { Initialise(tree); }
    void Initialise(int tree) { g_init = tree; }
};
int main()
{
    Node n(C2DVector(1.5f, 2.5f), 3, 4, 99);
    if (n.Position.X != 1.5f || n.Position.Y != 2.5f || n.QuadTreeLevel != 3 || n.NavLayer != 4 || !n.LeafNode || n.Blocked ||
        n.TL || n.TR || n.BL || n.BR || g_init != 99) { std::printf("00a7a570_TEST FAIL\n"); return 1; }
    std::printf("00a7a570_TEST PASS\n");
    return 0;
}