// Behaviour test for 0x00A78A90 CNavConnectorNode::CNavConnectorNode (links the REAL row object; the prologue below is the row's own).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CSMALLVECTOR_RESIZE
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#include "stlport/fable_containers.h"
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

CNavConnectorNode::~CNavConnectorNode() { }               // 0x00A780B0 is another row; nothing to undo in this test
int main()
{
    CNavRegion r1(1, 1), r2(1, 2);
    TestLeaf a(&r1), b(&r2), c(&r2);
    b.Add(&c.node);                                       // b already has one neighbour
    ResetCounters();
    {
        CNavConnectorNode connector(&a.node, &b.node);
        CHECK(connector.Neighbour1 == &a.node); CHECK(connector.Neighbour2 == &b.node);
    }
    CHECK(a.node.Neighbours.Size == 1); CHECK(a.storage[0] == &b.node);
    CHECK(b.node.Neighbours.Size == 2); CHECK(b.storage[0] == &c.node); CHECK(b.storage[1] == &a.node);
    CHECK(a.storage[1] == (const CANavQuadTreeNode*)0x1111); CHECK(b.storage[2] == (const CANavQuadTreeNode*)0x1111);
    CHECK(g_resizes == 2);
    CHECK(g_resizeThis[0] == &a.node.Neighbours && g_resizeSizes[0] == 1 && g_resizeFills[0] == 0);     // push_back = resize(size + 1) with a null fill
    CHECK(g_resizeThis[1] == &b.node.Neighbours && g_resizeSizes[1] == 2 && g_resizeFills[1] == 0);
    CHECK(g_getRegionCalls == 2); CHECK(g_getRegionOrder[0] == &a.node); CHECK(g_getRegionOrder[1] == &b.node);
    CHECK(g_connects == 1); CHECK(g_removes == 0); CHECK(g_connectThis == &r1); CHECK(g_connectArg == &r2);
    CHECK(c.node.Neighbours.Size == 0);
    // same region on both sides: still one connect call, region to itself
    TestLeaf d(&r2), e(&r2);
    ResetCounters();
    { CNavConnectorNode connector(&e.node, &d.node); CHECK(connector.Neighbour1 == &e.node); CHECK(connector.Neighbour2 == &d.node); }
    CHECK(e.storage[0] == &d.node && d.storage[0] == &e.node && e.node.Neighbours.Size == 1 && d.node.Neighbours.Size == 1);
    CHECK(g_connects == 1 && g_connectThis == &r2 && g_connectArg == &r2);
    if (g_fail) { printf("00a78a90_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a78a90_TEST PASS"); return 0;
}
