// Behaviour test for retail 0x00a75f10: the candidate source below is embedded verbatim (from src/00a75f10.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CNavQuadTreeNode::SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf) -- forward the leaf to the two
//   children that border the given side, passing the same side down (recursive virtual call, slot 16)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75F10, 0x52 bytes of code + a 4-entry jump table (164 bytes in the oracle row);
//   slot 16 of vtable 0x0129DCD4 = CNavQuadTreeNode (rtti-vtables.tsv). FableWin.pdb mangles the family as
//   ?SetUpNeighboursWithLeaf@CANavQuadTreeNode@@MAEXW4ENavQuadTreeNeighbours@@PAV1@@Z (global enum, protected virtual).
// Leaf: SetUpNeighboursWithLeaf
// Layout: PDB (Ego_r) names: CANavQuadTreeNode Position +0x04, QuadTreeLevel +0x0c, NavLayer +0x0d, LeafNode +0x0e,
//   Blocked +0x0f; CNavQuadTreeNode TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1c. Vtable slot names come from
//   the CNavQuadTreeNode vtable labels (rtti-vtables.tsv + manifest); unlabelled slots are VFuncNN. The enumerator
//   names are not in the evidence, so the four sides are numbered. Local model: no engine/ header for these classes.
// Jump table 0x00A75FA4 = { 0x00A75F85, 0x00A75F66, 0x00A75F28, 0x00A75F47 }: side 0 -> BL+BR, 1 -> TL+TR, 2 -> TR+BR,
//   3 -> TL+BL, each child told the same side. The arms sit in the bytes as 2, 3, 1, 0, so the cases are written in that
//   order (an earlier version read the cases off the arm order and so sent every side to the wrong children).
enum ENavQuadTreeNeighbours { NAV_NEIGHBOUR_0 = 0, NAV_NEIGHBOUR_1 = 1, NAV_NEIGHBOUR_2 = 2, NAV_NEIGHBOUR_3 = 3 };
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();                       // slot 0
    virtual void VFunc01(); virtual void VFunc02();
    virtual bool HasNeighbours() const;                 // slot 3
    virtual void VFunc04();
    virtual long GetRegionID() const;                   // slot 5
    virtual long GetNodeType() const;                   // slot 6
    virtual void VFunc07(); virtual void VFunc08(); virtual void VFunc09(); virtual void VFunc10();
    virtual CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos);   // slot 11
    virtual bool MakeRegion(const void* region, void* nodes);         // slot 12
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const void* region, void* nodes);   // slot 13
    virtual bool RemoveRegionZero();                    // slot 14
    virtual unsigned long GetMemoryUsage() const;       // slot 15
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf);   // slot 16 (+0x40)
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node);           // slot 17
    C2DVector Position;                                 // +0x04
    unsigned char QuadTreeLevel;                        // +0x0c
    unsigned char NavLayer;                             // +0x0d
    bool LeafNode;                                      // +0x0e
    bool Blocked;                                       // +0x0f
};
class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    CANavQuadTreeNode* TLNode;                          // +0x10
    CANavQuadTreeNode* TRNode;                          // +0x14
    CANavQuadTreeNode* BLNode;                          // +0x18
    CANavQuadTreeNode* BRNode;                          // +0x1c
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf);
};
void CNavQuadTreeNode::SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf)
{
    switch (side) {
    case NAV_NEIGHBOUR_2: TRNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_2, leaf); BRNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_2, leaf); break;
    case NAV_NEIGHBOUR_3: TLNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_3, leaf); BLNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_3, leaf); break;
    case NAV_NEIGHBOUR_1: TLNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_1, leaf); TRNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_1, leaf); break;
    case NAV_NEIGHBOUR_0: BLNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_0, leaf); BRNode->SetUpNeighboursWithLeaf(NAV_NEIGHBOUR_0, leaf); break;
    default: break;
    }
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
CANavQuadTreeNode::~CANavQuadTreeNode() {}
void CANavQuadTreeNode::VFunc01() {} void CANavQuadTreeNode::VFunc02() {} void CANavQuadTreeNode::VFunc04() {}
bool CANavQuadTreeNode::HasNeighbours() const { return false; }
long CANavQuadTreeNode::GetRegionID() const { return 0; }
long CANavQuadTreeNode::GetNodeType() const { return 0; }
void CANavQuadTreeNode::VFunc07() {} void CANavQuadTreeNode::VFunc08() {} void CANavQuadTreeNode::VFunc09() {} void CANavQuadTreeNode::VFunc10() {}
CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) { return 0; }
bool CANavQuadTreeNode::MakeRegion(const void*, void*) { return false; }
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const void*, void*) { return false; }
bool CANavQuadTreeNode::RemoveRegionZero() { return false; }
unsigned long CANavQuadTreeNode::GetMemoryUsage() const { return 0; }
void CANavQuadTreeNode::SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours, CANavQuadTreeNode*) {}
void CANavQuadTreeNode::SetUpNeighbours(ENavQuadTreeNeighbours, CANavQuadTreeNode*) {}
static int g_calls;
struct TChild : CANavQuadTreeNode {
    int side; CANavQuadTreeNode* leaf; int calls;
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours s, CANavQuadTreeNode* l) { side = s; leaf = l; ++calls; ++g_calls; }
};
int main() {
    int ok = offsetof(CNavQuadTreeNode, TLNode) == 0x10 && offsetof(CNavQuadTreeNode, BRNode) == 0x1c && offsetof(CANavQuadTreeNode, Blocked) == 0x0f;
    /* side -> (first child, second child, side passed down); children order TL, TR, BL, BR. From retail jump table
       0x00A75FA4 = { 0x00A75F85 BL+BR push 0, 0x00A75F66 TL+TR push 1, 0x00A75F28 TR+BR push 2, 0x00A75F47 TL+BL push 3 } */
    static const int a[4] = { 2, 0, 1, 0 }, b[4] = { 3, 1, 3, 2 }, passed[4] = { 0, 1, 2, 3 };
    for (int s = 0; ok && s < 5; ++s) {
        TChild c[4]; for (int i = 0; i < 4; ++i) { c[i].side = -1; c[i].leaf = 0; c[i].calls = 0; }
        CNavQuadTreeNode n; n.TLNode = &c[0]; n.TRNode = &c[1]; n.BLNode = &c[2]; n.BRNode = &c[3];
        TChild leaf; g_calls = 0;
        CANavQuadTreeNode* base = &n; base->SetUpNeighboursWithLeaf((ENavQuadTreeNeighbours)s, &leaf);   /* slot 16 dispatch */
        if (s == 4) { ok = g_calls == 0; break; }                                                          /* out of range: nothing */
        ok = g_calls == 2;
        for (int i = 0; ok && i < 4; ++i) {
            int hit = (i == a[s] || i == b[s]);
            ok = hit ? (c[i].calls == 1 && c[i].side == passed[s] && c[i].leaf == &leaf) : c[i].calls == 0;
        }
    }
    printf(ok ? "00a75f10_TEST PASS\n" : "00a75f10_TEST FAIL\n"); return ok ? 0 : 1;
}