// Behaviour test for 00a78800: src/00a78800.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64 uniqueID, bool blocked)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64 uniqueID, bool blocked) -- record whether one switch is open; the node is blocked unless every switch is open. When that state flips, gather the regions of the navigable neighbours: becoming blocked detaches the old region from each of them; becoming open adopts the first neighbour region, connects the node's region to the others and unlocks every switchable neighbour that is blocked with all of its own switches open (no neighbour regions: stay blocked)
// Leaf: SetAsBlocked
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78800, 0x1EE bytes (padded to 496), `ret 0Ch`.
// Callees: CVectorMap<unsigned __int64,bool>::LowerBound 0x00A7DF60, the region vector's _M_insert_overflow 0x00A814B0,
//   CNavRegion::RemoveConnectionToRegion 0x00A78060, CNavRegion::SetAsConnectedToRegion 0x00A786C0,
//   CNavSwitchableLeafNode::IsEveryUniqueIDOpen 0x00A773E0 (landed; manifest label PrepareForLoad is propagated) on the
//   neighbours, Unlock(CNavRegion*) 0x00A77420, free 0x00BFEA14. Virtuals: slot 1 GetNeighbours, slot 6 GetNodeType
//   (3 = switchable), slot 7 GetRegion.
// Layout: offsets as rebuild/include/engine/CNavSwitchableLeafNode.h (Blocked +0xF, PRegion +0x10, UniqueIDsOpen
//   +0x20), but that header is opaque (no virtuals, the map as bytes), so a local model is used here.
// Shape: the region list is a local STLport vector<CNavRegion*> whose finish pointer stays in esi (written back after
//   each push, reloaded after an overflow insert), with the neighbour container spilled to the dead bool-parameter slot
//   and the push_back temporary in the dead key slot. That needs this TU to see the two out-of-line callees that take
//   addresses (the overflow insert and the map's lower bound): both are defined below under auto_inline(off) as
//   stand-ins for retail's link-time knowledge (the landed STLport overflow body, the standard lower-bound walk). The
//   __true_type-style tag has an empty user ctor (no zero store); this node's all-open scan is IsEveryUniqueIDOpen
//   inlined (an early-return helper), while the neighbours' test stays a call; the "no regions" arm (stay blocked) is
//   the trailing else.
#include <stdlib.h>
class CNavRegion { public:
    void RemoveConnectionToRegion(CNavRegion* region);      // 0x00A78060
    void SetAsConnectedToRegion(CNavRegion* region);        // 0x00A786C0
};
class CANavQuadTreeNode;
struct CNodeSmallVector {
    CANavQuadTreeNode** Data;                               // +0x00
    signed char         Count;                              // +0x04
    CANavQuadTreeNode** begin() const { return Data; }
    CANavQuadTreeNode** end() const { return Data + Count; }
};
class CANavQuadTreeNode { public:
    virtual void _v0();
    virtual const CNodeSmallVector* GetNeighbours();        // slot 1
    virtual void _v2(); virtual void _v3(); virtual void _v4(); virtual void _v5();
    virtual long GetNodeType();                             // slot 6
    virtual CNavRegion* GetRegion();                        // slot 7
    float         PositionX, PositionY;                     // +0x04
    unsigned char QuadTreeLevel;                            // +0x0c
    unsigned char NavLayer;                                 // +0x0d
    bool          LeafNode;                                 // +0x0e
    bool          Blocked;                                  // +0x0f
};
struct SScalarTag { SScalarTag() { } };
struct CRegionVector {
    CNavRegion** _M_start;
    CNavRegion** _M_finish;
    CNavRegion** _M_end_of_storage;
    CRegionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRegionVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(CNavRegion** position, CNavRegion* const& x, const SScalarTag& tag, unsigned long count, bool atend);   // 0x00A814B0
    void push_back(CNavRegion* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, SScalarTag(), 1, true);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    CNavRegion*& operator[](unsigned long i) { return _M_start[i]; }
};
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);
#pragma auto_inline(off)
static CNavRegion** CopyTrivial(CNavRegion* const* first, CNavRegion* const* last, CNavRegion** result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (CNavRegion**)((char*)memmove(result, first, n) + n);
}
void CRegionVector::_M_insert_overflow(CNavRegion** position, CNavRegion* const& x, const SScalarTag&, unsigned long n, bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    CNavRegion** newStart = len ? (CNavRegion**)malloc(len * sizeof(CNavRegion*)) : 0;
    CNavRegion** newFinish = CopyTrivial(_M_start, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, _M_finish, newFinish);
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)
struct SSwitchEntry {
    unsigned __int64 first;                                 // +0x00
    bool             second;                                // +0x08 (switch open)
    unsigned char    _pad09[7];
};
struct CSwitchMap {
    SSwitchEntry* _M_start;                                 // +0x00
    SSwitchEntry* _M_finish;                                // +0x04
    SSwitchEntry* _M_end_of_storage;                        // +0x08
    SSwitchEntry* LowerBound(const unsigned __int64& key);  // 0x00A7DF60
    SSwitchEntry* Find(const unsigned __int64& key)
    {
        SSwitchEntry* it = LowerBound(key);
        SSwitchEntry* last = _M_finish;
        return (it == last || key < it->first) ? last : it;
    }
    unsigned long size() const { return _M_finish - _M_start; }
};
#pragma auto_inline(off)
SSwitchEntry* CSwitchMap::LowerBound(const unsigned __int64& key)
{
    SSwitchEntry* first = _M_start;
    int len = _M_finish - _M_start;
    while (len > 0) {
        int half = len / 2;
        SSwitchEntry* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}
#pragma auto_inline(on)
class CNavSwitchableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion*   PRegion;                                  // +0x10
    unsigned char _pad14[0xc];
    CSwitchMap    UniqueIDsOpen;                            // +0x20
    bool AllSwitchesOpen() const                            // IsEveryUniqueIDOpen's body, inlined for this node
    {
        for (unsigned long i = 0; i < UniqueIDsOpen.size(); ++i)
            if (!UniqueIDsOpen._M_start[i].second)
                return false;
        return true;
    }
    bool IsEveryUniqueIDOpen() const;                       // 0x00A773E0
    void Unlock(CNavRegion* region);                        // 0x00A77420
    void SetAsBlocked(unsigned __int64 uniqueID, bool blocked);
};
void CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64 uniqueID, bool blocked)
{
    UniqueIDsOpen.Find(uniqueID)->second = !blocked;
    bool nowBlocked = !AllSwitchesOpen();
    if (nowBlocked == Blocked)
        return;
    Blocked = nowBlocked;
    CRegionVector regions;
    const CNodeSmallVector* neighbours = GetNeighbours();
    for (CANavQuadTreeNode** it = neighbours->begin(); it != neighbours->end(); ++it) {
        CANavQuadTreeNode* neighbour = *it;
        if (neighbour->GetRegion())
            regions.push_back(neighbour->GetRegion());
    }
    if (Blocked) {
        CNavRegion* old = PRegion;
        PRegion = 0;
        for (unsigned long i = 0; i < regions.size(); ++i)
            if (regions[i] != old)
                old->RemoveConnectionToRegion(regions[i]);
    } else if (!regions.empty()) {
        PRegion = regions[0];
        CNavRegion* region = GetRegion();
        for (unsigned long i = 0; i < regions.size(); ++i)
            if (regions[i] != region)
                region->SetAsConnectedToRegion(regions[i]);
        for (CANavQuadTreeNode** it = neighbours->begin(); it != neighbours->end(); ++it) {
            CNavSwitchableLeafNode* neighbour = (CNavSwitchableLeafNode*)*it;
            if (neighbour->GetNodeType() == 3 && neighbour->Blocked && neighbour->IsEveryUniqueIDOpen())
                neighbour->Unlock(regions[0]);
        }
    } else {
        Blocked = true;
    }
}

// ---- stubs and driver ----
// Nodes live in one array; side tables give each node its neighbour list, node type and (for plain nodes) region.
static CNavSwitchableLeafNode g_nodes[8];
static CNodeSmallVector g_neigh[8];
static long g_type[8];
static CNavRegion* g_region[8];
static bool g_useOwnRegion[8];
static int Idx(const void* p) { for (int i = 0; i < 8; ++i) if ((const void*)&g_nodes[i] == p) return i; return -1; }
void CANavQuadTreeNode::_v0() {}
const CNodeSmallVector* CANavQuadTreeNode::GetNeighbours() { return &g_neigh[Idx(this)]; }
void CANavQuadTreeNode::_v2() {} void CANavQuadTreeNode::_v3() {} void CANavQuadTreeNode::_v4() {} void CANavQuadTreeNode::_v5() {}
long CANavQuadTreeNode::GetNodeType() { return g_type[Idx(this)]; }
CNavRegion* CANavQuadTreeNode::GetRegion()
{
    int i = Idx(this);
    return g_useOwnRegion[i] ? ((CNavSwitchableLeafNode*)this)->PRegion : g_region[i];
}
struct SCall { char kind; const void* self; const void* arg; };
static SCall g_log[64]; static int g_n = 0;
static void Log(char k, const void* s, const void* a) { if (g_n < 64) { g_log[g_n].kind = k; g_log[g_n].self = s; g_log[g_n].arg = a; } ++g_n; }
void CNavRegion::RemoveConnectionToRegion(CNavRegion* region) { Log('R', this, region); }
void CNavRegion::SetAsConnectedToRegion(CNavRegion* region) { Log('C', this, region); }
bool CNavSwitchableLeafNode::IsEveryUniqueIDOpen() const
{
    Log('E', this, 0);
    for (unsigned long i = 0; i < UniqueIDsOpen.size(); ++i)
        if (!UniqueIDsOpen._M_start[i].second)
            return false;
    return true;
}
void CNavSwitchableLeafNode::Unlock(CNavRegion* region) { Log('U', this, region); }
static bool Is(int i, char k, const void* s, const void* a) { return g_log[i].kind == k && g_log[i].self == s && g_log[i].arg == a; }
static void SetMap(CNavSwitchableLeafNode& n, SSwitchEntry* e, int count) { n.UniqueIDsOpen._M_start = e; n.UniqueIDsOpen._M_finish = e + count; n.UniqueIDsOpen._M_end_of_storage = e + count; }
int main()
{
    static CNavRegion R[4];
    static CANavQuadTreeNode* list[6];
    // g_nodes: static storage, zero-filled, vtables set by the implicit default ctor
    CNavSwitchableLeafNode& node = g_nodes[0];
    SSwitchEntry entries[3];
    memset(entries, 0, sizeof(entries));
    entries[0].first = 5; entries[0].second = true;
    entries[1].first = 0x100000009ULL; entries[1].second = true;
    entries[2].first = 0x200000000ULL; entries[2].second = true;
    SetMap(node, entries, 3);
    node.PRegion = &R[0]; node.Blocked = false; g_type[0] = 3; g_useOwnRegion[0] = true;
    // neighbours: 1 -> R1, 2 -> none, 3 -> R0 (the node's own region), 4 -> R2, 5 -> R3 (switchable, blocked, all open),
    // 6 -> R1 again (switchable, blocked, one switch shut)
    g_region[1] = &R[1]; g_type[1] = 1;
    g_region[2] = 0;     g_type[2] = 1;
    g_region[3] = &R[0]; g_type[3] = 1;
    g_region[4] = &R[2]; g_type[4] = 3; g_nodes[4].Blocked = false;
    g_region[5] = &R[3]; g_type[5] = 3; g_nodes[5].Blocked = true;
    SSwitchEntry open5[1]; memset(open5, 0, sizeof(open5)); open5[0].first = 1; open5[0].second = true; SetMap(g_nodes[5], open5, 1);
    g_region[6] = &R[1]; g_type[6] = 3; g_nodes[6].Blocked = true;
    SSwitchEntry shut6[2]; memset(shut6, 0, sizeof(shut6)); shut6[0].first = 1; shut6[0].second = true; shut6[1].first = 2; SetMap(g_nodes[6], shut6, 2);
    for (int k = 0; k < 6; ++k) list[k] = &g_nodes[k + 1];
    g_neigh[0].Data = list; g_neigh[0].Count = 6;

    // 1: shut switch 0x100000009 -> node becomes blocked, R0 detaches from every other gathered region (R1, R2, R3, R1)
    g_n = 0;
    node.SetAsBlocked(0x100000009ULL, true);
    CHECK(entries[1].second == false && entries[0].second && entries[2].second);
    CHECK(node.Blocked == true && node.PRegion == 0);
    CHECK(g_n == 4 && Is(0, 'R', &R[0], &R[1]) && Is(1, 'R', &R[0], &R[2]) && Is(2, 'R', &R[0], &R[3]) && Is(3, 'R', &R[0], &R[1]));

    // 2: shutting another switch keeps it blocked -> no state change, nothing else happens
    g_n = 0;
    node.SetAsBlocked(5, true);
    CHECK(entries[0].second == false && node.Blocked == true && node.PRegion == 0 && g_n == 0);

    // 3: open both again -> first call still blocked (no change), second unblocks: adopt R1, connect R1 to R0, R2, R3
    //    (not to itself), then unlock the blocked switchable neighbour whose switches are all open (5) into R1
    g_n = 0;
    node.SetAsBlocked(5, false);
    CHECK(node.Blocked == true && g_n == 0);
    node.SetAsBlocked(0x100000009ULL, false);
    CHECK(node.Blocked == false && node.PRegion == &R[1]);
    CHECK(g_n == 6);
    CHECK(Is(0, 'C', &R[1], &R[0]) && Is(1, 'C', &R[1], &R[2]) && Is(2, 'C', &R[1], &R[3]));
    CHECK(Is(3, 'E', &g_nodes[5], 0) && Is(4, 'U', &g_nodes[5], &R[1]) && Is(5, 'E', &g_nodes[6], 0));

    // 4: open transition with no neighbour regions -> stays blocked, PRegion untouched
    CNavSwitchableLeafNode& lone = g_nodes[7];
    SSwitchEntry one[1]; memset(one, 0, sizeof(one)); one[0].first = 42; one[0].second = false; SetMap(lone, one, 1);
    lone.Blocked = true; lone.PRegion = &R[2]; g_type[7] = 3; g_useOwnRegion[7] = true;
    static CANavQuadTreeNode* lonely[1]; lonely[0] = &g_nodes[2];   // a neighbour without a region
    g_neigh[7].Data = lonely; g_neigh[7].Count = 1;
    g_n = 0;
    lone.SetAsBlocked(42, false);
    CHECK(one[0].second == true && lone.Blocked == true && lone.PRegion == &R[2] && g_n == 0);

    // 5: blocking with a single gathered region equal to the old one -> nothing to detach
    lone.Blocked = false; lone.PRegion = &R[2]; lonely[0] = &g_nodes[4];   // neighbour region R2 == own region
    g_n = 0;
    lone.SetAsBlocked(42, true);
    CHECK(lone.Blocked == true && lone.PRegion == 0 && g_n == 0);

    if (fails == 0) printf("00a78800_TEST PASS\n");
    return fails;
}
