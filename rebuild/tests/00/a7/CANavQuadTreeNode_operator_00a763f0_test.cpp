// Behaviour test for 00a763f0: src/00a763f0.cpp copied verbatim; two nav quad-tree nodes are equal when their positions agree
// within 1e-4 per axis, the level/leaf/blocked/layer bytes match and their virtual keys match.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CANavQuadTreeNode::operator==(const CANavQuadTreeNode&) -- same position (within 1e-4), level, layer, flags and virtual key
// Leaf: ??8CANavQuadTreeNode
// Retry 10b: Position is a C2DVector member and the 1e-4 position test is its inline member helper (`Position.IsNear(other.Position)`);
// reached through the member's reference VC7.1 loads the other node's pointer before `fld [esi+4]` as retail (flat floats load it after).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A763F0, length 116 bytes.
// Callees: the nodes' virtual slot 5 (+0x14), called on the other node first, then on this one.
//
// Shape: `fabs(a - b) <= 0.0001f` per coordinate (`fsub ; fabs ; fcomp [eps] ; test ah,41h ; jp` to the false
// tail = the negated `<=`), then the four byte members compared in source order QuadTreeLevel (+0xc),
// LeafNode (+0xe), Blocked (+0xf), NavLayer (+0xd) with alternating al/dl loads, then the virtual key:
// the other node's value is fetched first into edi (`long key = other.GetKey();`) and compared as
// `GetKey() == key` (`cmp eax,edi`). The result is a full-register int (`mov eax,1` / `xor eax,eax`).
// The engine header CANavQuadTreeNode.h is data-only (a void* vptr word); the virtual call needs a
// polymorphic local model, members carry the header's names and offsets.
#include <math.h>
#pragma intrinsic(fabs)

class C2DVector { public:
    float X, Y;
    bool IsNear(const C2DVector& node) const { return fabs(X - node.X) <= 0.0001f && fabs(Y - node.Y) <= 0.0001f; }
};
class CANavQuadTreeNode {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long GetKey() const;                // slot 5 (+0x14)
    C2DVector Position;                         // +0x04
    unsigned char QuadTreeLevel;                // +0x0c
    unsigned char NavLayer;                     // +0x0d
    bool LeafNode;                              // +0x0e
    bool Blocked;                               // +0x0f
    int operator==(const CANavQuadTreeNode& other) const;
};

int CANavQuadTreeNode::operator==(const CANavQuadTreeNode& other) const
{
    if (Position.IsNear(other.Position)
        && QuadTreeLevel == other.QuadTreeLevel && LeafNode == other.LeafNode && Blocked == other.Blocked
        && NavLayer == other.NavLayer) {
        long key = other.GetKey();
        if (GetKey() == key)
            return 1;
    }
    return 0;
}
// ---- end source ----
void CANavQuadTreeNode::v00() {} void CANavQuadTreeNode::v01() {} void CANavQuadTreeNode::v02() {} void CANavQuadTreeNode::v03() {} void CANavQuadTreeNode::v04() {}

static long g_keys[2]; static const CANavQuadTreeNode* g_nodes[2];
long CANavQuadTreeNode::GetKey() const { return this == g_nodes[0] ? g_keys[0] : g_keys[1]; }
int main()
{
    CANavQuadTreeNode a, b;
    a.Position.X = 1.0f; a.Position.Y = 2.0f; a.QuadTreeLevel = 3; a.NavLayer = 1; a.LeafNode = true; a.Blocked = false;
    b = a; b.Position.X = 1.00005f;
    g_nodes[0] = &a; g_nodes[1] = &b; g_keys[0] = 9; g_keys[1] = 9;
    CHECK((a == b) == 1);
    b.Position.Y = 2.001f; CHECK((a == b) == 0); b.Position.Y = 2.0f;
    b.NavLayer = 2; CHECK((a == b) == 0); b.NavLayer = 1;
    b.Blocked = true; CHECK((a == b) == 0); b.Blocked = false;
    g_keys[1] = 8; CHECK((a == b) == 0);
    if (fails == 0) printf("00a763f0_TEST PASS\n"); else printf("00a763f0_TEST FAIL\n");
    return fails;
}