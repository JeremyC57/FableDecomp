// Behaviour test for 0x00A78800 CNavSwitchableLeafNode::SetAsBlocked (links the REAL row object: the row function, its same-TU
// IsEveryUniqueIDOpen and ForceRegionTo copies and the shared headers' _M_insert_overflow / LowerBound bodies come from it).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION
#define FABLE_RETAIL_CSMALLVECTOR_RESIZE
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include <stdlib.h>
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavNavigableLeafNode.h"
#include "engine/CNavRegion.h"
#include "engine/CNavSwitchableLeafNode.h"
// The fixtures: the real class has only the two file / generator constructors, so the nodes are a default-constructible layout twin
// (same base, PRegion +0x10, the map's three vector words +0x20) whose storage the row is called on.
struct SSwitchEntry {
    unsigned __int64 first;                                 // +0x00
    bool             second;                                // +0x08 (switch open)
    unsigned char    _pad09[7];
};
struct CSwitchMap {
    SSwitchEntry* _M_start;                                 // +0x00
    SSwitchEntry* _M_finish;                                // +0x04
    SSwitchEntry* _M_end_of_storage;                        // +0x08
};
class CTestSwitchableNode : public CANavQuadTreeNode {
public:
    CNavRegion*   PRegion;                                  // +0x10
    unsigned char _pad14[0xc];
    CSwitchMap    UniqueIDsOpen;                            // +0x20
    unsigned char _pad2c[4];
    void SetAsBlocked(unsigned __int64 unique_id, bool blocked) { ((CNavSwitchableLeafNode*)(void*)this)->SetAsBlocked(unique_id, blocked); }
};
// ---- test harness: links the REAL row object ----
#include <stdio.h>
#include <string.h>
static int g_fail;
#define CHECK(c) do { if (!(c)) { ++g_fail; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
const float g_QuadTreeLevelSizes[] = { 32.0f, 16.0f, 8.0f, 4.0f, 2.0f, 1.0f, 0.5f };
CNavRegion::CNavRegion(long mapIndex, long internalIndex) : RegionID(mapIndex * 65536 + internalIndex), MostRecentSearchID(0) { }
// Nodes live in one array; side tables give each node its neighbour list, node type and (for plain nodes) region.
static CTestSwitchableNode g_nodes[8];
static CSmallVector<const CANavQuadTreeNode*, 8> g_neigh[8];
static long g_type[8];
static CNavRegion* g_region[8];
static bool g_useOwnRegion[8];
static int Idx(const void* p) { for (int i = 0; i < 8; ++i) if ((const void*)&g_nodes[i] == p) return i; return -1; }
struct SCall { char kind; const void* self; const void* arg; };
static SCall g_log[64]; static int g_n = 0;
static void Log(char k, const void* s, const void* a) { if (g_n < 64) { g_log[g_n].kind = k; g_log[g_n].self = s; g_log[g_n].arg = a; } ++g_n; }
// The node's virtual table (the shared header declares every slot): the row reaches slot 1 (non-const GetNeighbours), slot 6 and slot 7.
CANavQuadTreeNode::~CANavQuadTreeNode() { }
const CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() const { ++g_fail; return 0; }
CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() { return &g_neigh[Idx(this)]; }
bool CANavQuadTreeNode::HasNeighbours() const { ++g_fail; return false; }
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const { ++g_fail; return false; }
long CANavQuadTreeNode::GetRegionID() const { ++g_fail; return 0; }
ENavQuadTreeNodeType CANavQuadTreeNode::GetNodeType() const { return (ENavQuadTreeNodeType)g_type[Idx(this)]; }
const CNavRegion* CANavQuadTreeNode::GetRegion() const
{
    int i = Idx(this);
    return g_useOwnRegion[i] ? ((const CTestSwitchableNode*)this)->PRegion : g_region[i];
}
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const { ++g_fail; return 0; }
bool CANavQuadTreeNode::IsSwitchable() const { ++g_fail; return false; }
bool CANavQuadTreeNode::IsSwitchableOn(unsigned __int64) const { ++g_fail; return false; }
const CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const { ++g_fail; return 0; }
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&) { ++g_fail; return false; }
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&) { ++g_fail; return false; }
bool CANavQuadTreeNode::RemoveRegionZero() { ++g_fail; return false; }
unsigned long CANavQuadTreeNode::GetMemoryUsage(unsigned long&, unsigned long&, unsigned long&, unsigned long&) const { ++g_fail; return 0; }
void CNavRegion::RemoveConnectionToRegion(CNavRegion& region) { Log('R', this, &region); }
void CNavRegion::SetAsConnectedToRegion(CNavRegion& region) { Log('C', this, &region); }
// IsEveryUniqueIDOpen and ForceRegionTo are the row object's own strong definitions (IsEveryUniqueIDOpen expanded for the node
// itself, called for the neighbours; ForceRegionTo expanded).
void CNavSwitchableLeafNode::Unlock(const CNavRegion* pregion) { Log('U', this, pregion); }
static bool Is(int i, char k, const void* s, const void* a) { return g_log[i].kind == k && g_log[i].self == s && g_log[i].arg == a; }
static void SetMap(CTestSwitchableNode& n, SSwitchEntry* e, int count) { n.UniqueIDsOpen._M_start = e; n.UniqueIDsOpen._M_finish = e + count; n.UniqueIDsOpen._M_end_of_storage = e + count; }
int main()
{
    static CNavRegion R[4] = { CNavRegion(3, 0), CNavRegion(3, 1), CNavRegion(3, 2), CNavRegion(3, 3) };
    static const CANavQuadTreeNode* list[6];
    // g_nodes: static storage, zero-filled, vtables set by the implicit default ctor
    CTestSwitchableNode& node = g_nodes[0];
    SSwitchEntry entries[3];
    memset(entries, 0, sizeof(entries));
    entries[0].first = 5; entries[0].second = true;
    entries[1].first = 0x100000009ULL; entries[1].second = true;
    entries[2].first = 0x200000000ULL; entries[2].second = true;
    SetMap(node, entries, 3);
    node.PRegion = &R[0]; node.Blocked = false; g_type[0] = 3; g_useOwnRegion[0] = true;
    // neighbours: 1 -> R1, 2 -> none, 3 -> R0 (the node's own region), 4 -> R2, 5 -> R3 (switchable, blocked, all open),
    // 6 -> R2 again (switchable, blocked, one switch shut): the first and the last gathered region differ
    g_region[1] = &R[1]; g_type[1] = 1;
    g_region[2] = 0;     g_type[2] = 1;
    g_region[3] = &R[0]; g_type[3] = 1;
    g_region[4] = &R[2]; g_type[4] = 3; g_nodes[4].Blocked = false;
    g_region[5] = &R[3]; g_type[5] = 3; g_nodes[5].Blocked = true;
    SSwitchEntry open5[1]; memset(open5, 0, sizeof(open5)); open5[0].first = 1; open5[0].second = true; SetMap(g_nodes[5], open5, 1);
    g_region[6] = &R[2]; g_type[6] = 3; g_nodes[6].Blocked = true;
    SSwitchEntry shut6[2]; memset(shut6, 0, sizeof(shut6)); shut6[0].first = 1; shut6[0].second = true; shut6[1].first = 2; SetMap(g_nodes[6], shut6, 2);
    for (int k = 0; k < 6; ++k) list[k] = &g_nodes[k + 1];
    g_neigh[0].Start = list; g_neigh[0].Size = 6;

    // 1: shut switch 0x100000009 -> node becomes blocked, R0 detaches from every other gathered region (R1, R2, R3, R2)
    g_n = 0;
    node.SetAsBlocked(0x100000009ULL, true);
    CHECK(entries[1].second == false && entries[0].second && entries[2].second);
    CHECK(node.Blocked == true && node.PRegion == 0);
    CHECK(g_n == 4 && Is(0, 'R', &R[0], &R[1]) && Is(1, 'R', &R[0], &R[2]) && Is(2, 'R', &R[0], &R[3]) && Is(3, 'R', &R[0], &R[2]));

    // 2: shutting another switch keeps it blocked -> no state change, nothing else happens
    g_n = 0;
    node.SetAsBlocked(5, true);
    CHECK(entries[0].second == false && node.Blocked == true && node.PRegion == 0 && g_n == 0);

    // 3: open both again -> first call still blocked (no change), second unblocks: adopt R1, connect R1 to R0, R2, R3, R2
    //    (not to itself), then unlock the blocked switchable neighbour whose switches are all open (5) into R1
    g_n = 0;
    node.SetAsBlocked(5, false);
    CHECK(node.Blocked == true && g_n == 0);
    node.SetAsBlocked(0x100000009ULL, false);
    CHECK(node.Blocked == false && node.PRegion == &R[1]);
    CHECK(g_n == 5);
    CHECK(Is(0, 'C', &R[1], &R[0]) && Is(1, 'C', &R[1], &R[2]) && Is(2, 'C', &R[1], &R[3]) && Is(3, 'C', &R[1], &R[2]));
    CHECK(Is(4, 'U', &g_nodes[5], &R[1]));   // node 5 (all open) unlocked; node 6 (one switch shut) and node 4 (not blocked) are not

    // 4: open transition with no neighbour regions -> stays blocked, PRegion untouched
    CTestSwitchableNode& lone = g_nodes[7];
    SSwitchEntry one[1]; memset(one, 0, sizeof(one)); one[0].first = 42; one[0].second = false; SetMap(lone, one, 1);
    lone.Blocked = true; lone.PRegion = &R[2]; g_type[7] = 3; g_useOwnRegion[7] = true;
    static const CANavQuadTreeNode* lonely[1]; lonely[0] = &g_nodes[2];   // a neighbour without a region
    g_neigh[7].Start = lonely; g_neigh[7].Size = 1;
    g_n = 0;
    lone.SetAsBlocked(42, false);
    CHECK(one[0].second == true && lone.Blocked == true && lone.PRegion == &R[2] && g_n == 0);

    // 5: blocking with a single gathered region equal to the old one -> nothing to detach
    lone.Blocked = false; lone.PRegion = &R[2]; lonely[0] = &g_nodes[4];   // neighbour region R2 == own region
    g_n = 0;
    lone.SetAsBlocked(42, true);
    CHECK(lone.Blocked == true && lone.PRegion == 0 && g_n == 0);

    if (g_fail == 0) printf("00a78800_TEST PASS\n"); else printf("00a78800_TEST FAIL (%d)\n", g_fail);
    return g_fail;
}
