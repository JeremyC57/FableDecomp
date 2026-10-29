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
    private: bool IsEveryUniqueIDOpen() const; public:                       // 0x00A773E0
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