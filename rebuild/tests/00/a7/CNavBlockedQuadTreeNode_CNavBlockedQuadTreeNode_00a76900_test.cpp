#include <cstdio>
// Behaviour test for CNavBlockedQuadTreeNode copy constructor (0x00A76900).
struct V2 { float X, Y; };
struct Node {
    V2 Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked;
    Node(const V2& p, unsigned char l, unsigned char n) : Position(p), QuadTreeLevel(l), NavLayer(n) { LeafNode = true; Blocked = false; }
};
struct BlockedNode : Node {
    BlockedNode(const V2& p, unsigned char l, unsigned char n) : Node(p, l, n) { Blocked = true; }
    BlockedNode(const BlockedNode& o) : Node(o.Position, o.QuadTreeLevel, o.NavLayer) { Blocked = true; }
};
int main()
{
    V2 p = { 1.5f, -2.0f };
    BlockedNode a(p, 3, 7);
    a.LeafNode = false;                      // the copy re-derives the flags, it does not copy them
    BlockedNode b(a);
    bool ok = b.Position.X == 1.5f && b.Position.Y == -2.0f && b.QuadTreeLevel == 3 && b.NavLayer == 7 && b.LeafNode && b.Blocked;
    std::printf(ok ? "00a76900_TEST PASS\n" : "00a76900_TEST FAIL\n");
    return ok ? 0 : 1;
}