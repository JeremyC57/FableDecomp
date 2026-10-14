#include <cstdio>
// Model of CANavQuadTreeNode::CANavQuadTreeNode @ 0x00A76370: position, level, layer, leaf = true, blocked = false.
struct C2DVector { float X, Y; C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {} C2DVector(float x, float y) : X(x), Y(y) {} };
static int g_base;
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() {} CBaseClassNonCopyable() { ++g_base; } };
struct Node : CBaseClassNonCopyable {
    C2DVector Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked;
    Node(const C2DVector& p, unsigned char level, unsigned char layer) : CBaseClassNonCopyable(), Position(p), QuadTreeLevel(level), NavLayer(layer), LeafNode(true), Blocked(false) {}
};
int main()
{
    Node n(C2DVector(1.5f, -2.0f), 3, 7);
    if (g_base != 1 || n.Position.X != 1.5f || n.Position.Y != -2.0f || n.QuadTreeLevel != 3 || n.NavLayer != 7 || !n.LeafNode || n.Blocked) { std::printf("00a76370_TEST FAIL\n"); return 1; }
    std::printf("00a76370_TEST PASS\n"); return 0;
}
