// Behaviour test for 0x00A780B0 CNavConnectorNode::~CNavConnectorNode (links the REAL row object; the prologue below is the row's own).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CSMALLVECTOR_RESIZE
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C2DVector.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavNavigableLeafNode.h"
#include "engine/CNavRegion.h"
#include "engine/CNavConnectorNode.h"

// ---- test harness: links the REAL row object ----
#include <stdio.h>
#include <string.h>
static int g_fail;
#define CHECK(c) do { if (!(c)) { ++g_fail; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
const float g_QuadTreeLevelSizes[] = { 32.0f, 16.0f, 8.0f, 4.0f, 2.0f, 1.0f, 0.5f };
// The node's virtual table (the shared header declares every slot): only slot 7 GetRegion is reached; the others fail the test.
CANavQuadTreeNode::~CANavQuadTreeNode() { }
const CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() const { ++g_fail; return 0; }
CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() { ++g_fail; return 0; }
bool CANavQuadTreeNode::HasNeighbours() const { ++g_fail; return false; }
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const { ++g_fail; return false; }
long CANavQuadTreeNode::GetRegionID() const { ++g_fail; return 0; }
ENavQuadTreeNodeType CANavQuadTreeNode::GetNodeType() const { ++g_fail; return (ENavQuadTreeNodeType)0; }
static int g_getRegionCalls; static const CANavQuadTreeNode* g_getRegionOrder[4];
const CNavRegion* CANavQuadTreeNode::GetRegion() const
{
    if (g_getRegionCalls < 4) g_getRegionOrder[g_getRegionCalls] = this;
    ++g_getRegionCalls;
    return static_cast<const CNavNavigableLeafNode*>(this)->PRegion;
}
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const { ++g_fail; return 0; }
bool CANavQuadTreeNode::IsSwitchable() const { ++g_fail; return false; }
bool CANavQuadTreeNode::IsSwitchableOn(unsigned __int64) const { ++g_fail; return false; }
const CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const { ++g_fail; return 0; }
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&) { ++g_fail; return false; }
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&) { ++g_fail; return false; }
bool CANavQuadTreeNode::RemoveRegionZero() { ++g_fail; return false; }
unsigned long CANavQuadTreeNode::GetMemoryUsage(unsigned long&, unsigned long&, unsigned long&, unsigned long&) const { ++g_fail; return 0; }
// The file-loading constructor is the only one the header declares by default; the test uses it as a plain builder.
CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector&, unsigned char, unsigned char, CAFile&, const std::list<CNavRegion>&, bool)
{
    PRegion = 0; Neighbours.Start = 0; Neighbours.Size = 0; PreferabilityCost = 0;
}
CNavRegion::CNavRegion(long mapIndex, long internalIndex) : RegionID(mapIndex * 65536 + internalIndex), MostRecentSearchID(0) { }
// CSmallVector::resize 0x00A7DB10 (out of line in retail): fixed test storage of 8 slots per vector, new slots take the fill value.
static int g_resizes; static long g_resizeSizes[8]; static const CANavQuadTreeNode* g_resizeFills[8]; static void* g_resizeThis[8];
template<> void CSmallVector<const CANavQuadTreeNode*, 8>::resize(long size, const CANavQuadTreeNode* const& fill)
{
    if (g_resizes < 8) { g_resizeSizes[g_resizes] = size; g_resizeFills[g_resizes] = fill; g_resizeThis[g_resizes] = this; }
    ++g_resizes;
    if (size < 0 || size > 8) { ++g_fail; return; }
    for (long i = Size; i < size; ++i) Start[i] = fill;
    Size = (signed char)size;
}
static CNavRegion* g_connectThis; static CNavRegion* g_connectArg; static int g_connects, g_removes;
void CNavRegion::SetAsConnectedToRegion(CNavRegion& region) { g_connectThis = this; g_connectArg = &region; ++g_connects; }
void CNavRegion::RemoveConnectionToRegion(CNavRegion& region) { g_connectThis = this; g_connectArg = &region; ++g_removes; }
struct TestLeaf {
    const CANavQuadTreeNode* storage[8];
    CNavNavigableLeafNode node;
    TestLeaf(CNavRegion* region) : node(C2DVector(), 0, 0, *(CAFile*)0, *(const std::list<CNavRegion>*)0, false)
    {
        for (int i = 0; i < 8; ++i) storage[i] = (const CANavQuadTreeNode*)0x1111;
        node.PRegion = region; node.Neighbours.Start = storage; node.Neighbours.Size = 0;
    }
    void Add(const CANavQuadTreeNode* n) { storage[node.Neighbours.Size] = n; ++node.Neighbours.Size; }
};
static void ResetCounters() { g_getRegionCalls = g_resizes = g_connects = g_removes = 0; g_connectThis = g_connectArg = 0; }

// 0x00A78A90 is another row: a plain builder here (the lists are set up by the test).
CNavConnectorNode::CNavConnectorNode(const CANavQuadTreeNode* neighbour1, const CANavQuadTreeNode* neighbour2)
    : Neighbour1((CNavNavigableLeafNode*)neighbour1), Neighbour2((CNavNavigableLeafNode*)neighbour2) { }
static void Case(int n1, int pos2in1, int n2, int pos1in2)
{
    // node x has n1 neighbours with y at index pos2in1; node y has n2 neighbours with x at index pos1in2
    CNavRegion r1(2, 1), r2(2, 2);
    TestLeaf x(&r1), y(&r2), o1(&r1), o2(&r1), o3(&r2), o4(&r2);
    const CANavQuadTreeNode* others[4] = { &o1.node, &o2.node, &o3.node, &o4.node };
    const CANavQuadTreeNode* ex[8]; const CANavQuadTreeNode* ey[8]; int k;
    k = 0; for (int i = 0; i < n1; ++i) { const CANavQuadTreeNode* v = (i == pos2in1) ? (const CANavQuadTreeNode*)&y.node : others[k++]; x.Add(v); ex[i] = v; }
    k = 0; for (int i = 0; i < n2; ++i) { const CANavQuadTreeNode* v = (i == pos1in2) ? (const CANavQuadTreeNode*)&x.node : others[3 - k++]; y.Add(v); ey[i] = v; }
    // expected: the hit swapped with the last entry (unless it is the last), then one entry dropped
    if (pos2in1 < n1 - 1) { ex[pos2in1] = ex[n1 - 1]; }
    if (pos1in2 < n2 - 1) { ey[pos1in2] = ey[n2 - 1]; }
    ResetCounters();
    { CNavConnectorNode connector(&x.node, &y.node); }
    CHECK(x.node.Neighbours.Size == n1 - 1); CHECK(y.node.Neighbours.Size == n2 - 1);
    for (int i = 0; i < n1 - 1; ++i) CHECK(x.storage[i] == ex[i]);
    for (int i = 0; i < n2 - 1; ++i) CHECK(y.storage[i] == ey[i]);
    CHECK(g_resizes == 2);
    CHECK(g_resizeThis[0] == &x.node.Neighbours && g_resizeSizes[0] == n1 - 1 && g_resizeFills[0] == 0);
    CHECK(g_resizeThis[1] == &y.node.Neighbours && g_resizeSizes[1] == n2 - 1 && g_resizeFills[1] == 0);
    CHECK(g_getRegionCalls == 2); CHECK(g_getRegionOrder[0] == &x.node); CHECK(g_getRegionOrder[1] == &y.node);
    CHECK(g_removes == 1); CHECK(g_connects == 0); CHECK(g_connectThis == &r1); CHECK(g_connectArg == &r2);
    CHECK(o1.node.Neighbours.Size == 0 && o4.node.Neighbours.Size == 0);
}
int main()
{
    Case(1, 0, 1, 0);          // each the other's only neighbour: no swap
    Case(3, 0, 2, 1);          // first of three (swapped to the back); last of two (no swap)
    Case(3, 1, 3, 0);          // middle of three; first of three
    Case(3, 2, 3, 2);          // last on both sides: no swap
    Case(5, 4, 5, 3);          // five entries: the 4x unrolled find takes one full round plus the tail
    Case(5, 2, 5, 0);
    Case(4, 3, 2, 0);
    Case(2, 1, 4, 1);
    if (g_fail) { printf("00a780b0_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a780b0_TEST PASS"); return 0;
}
