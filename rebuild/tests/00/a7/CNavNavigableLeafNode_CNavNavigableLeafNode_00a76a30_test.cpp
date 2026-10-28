#include <cstdio>
// Behaviour test for CNavNavigableLeafNode::CNavNavigableLeafNode(pos, level, layer, preferability) (0x00A76A30).
struct V2 { float X, Y; };
struct Node {
    V2 Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked;
    Node(const V2& p, unsigned char l, unsigned char n) : Position(p), QuadTreeLevel(l), NavLayer(n) { LeafNode = true; Blocked = false; }
};
struct SV { void* Start; signed char Size; SV() : Start(0), Size(0) { } };
struct Leaf : Node {
    void* PRegion; SV Neighbours; unsigned char PreferabilityCost;
    Leaf(const V2& p, unsigned char l, unsigned char n, unsigned char c) : Node(p, l, n), PRegion(0), PreferabilityCost(c) { }
};
int main()
{
    V2 p = { 3.0f, 4.0f };
    Leaf f(p, 2, 5, 9);
    bool ok = f.Position.X == 3.0f && f.Position.Y == 4.0f && f.QuadTreeLevel == 2 && f.NavLayer == 5 && f.LeafNode && !f.Blocked
           && f.PRegion == 0 && f.Neighbours.Start == 0 && f.Neighbours.Size == 0 && f.PreferabilityCost == 9;
    std::printf(ok ? "00a76a30_TEST PASS\n" : "00a76a30_TEST FAIL\n");
    return ok ? 0 : 1;
}