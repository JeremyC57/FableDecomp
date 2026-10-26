// Behaviour test for 00a79a30: src/00a79a30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, list<CNavRegion>& regions, bool readPreferability)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, list<CNavRegion>& regions, bool readPreferability) -- load a switchable leaf node: the navigable leaf part, then the list of 64-bit unique ids of the switches that open it (all read as closed), blocked by default, no region, the id map sorted
// Leaf: ??0CNavSwitchableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79A30, 245 bytes (+ int3 padding), `ret 18h`.
// Callees: CNavNavigableLeafNode(file ctor) 0x00A772D0 (out of line), CAFile slot 3 Read(void*, size, flags) (+0x0c), the id
//   vector's _M_insert_overflow(pos, x, tag, fill, atEnd) 0x00A82380, the `__fastcall` sort 0x00A84160 (first, last,
//   comparator by value).
// Vtable: 0x0129DE04 CNavSwitchableLeafNode.
// Layout: Ego_r.pdb CNavSwitchableLeafNode: UniqueIDsOpen +0x20 (CVectorMap<unsigned __int64, bool>: STLport vector of
//   16-byte pairs, comparator byte +0x2c, Dirty +0x2d); the leaf base's Blocked +0x0f and PRegion +0x10.
// Shape: the count and the loop index are homed in the dead `regions` / `readPreferability` slots; each id is added
//   through the vector map's inline Add (push_back, then Dirty), the map is sorted once through the inline Sort().
class C2DVector { public: float X, Y; };
class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(void* data, unsigned long size, long flags);   // slot 3 (+0x0c)
};
struct CRegionList;
class CNavNavigableLeafNode {                      // vtable 0x0129DDA4
public:
    virtual void v00();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    void*         PRegion;                         // +0x10
    void*         NeighbourData;                   // +0x14
    signed char   NeighbourCount;                  // +0x18
    unsigned char _pad19[3];
    unsigned char PreferabilityCost;               // +0x1c
    CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, CRegionList& regions, bool readPreferability);   // 0x00A772D0
};

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };
struct SUniqueIDOpen {                             // pair<unsigned __int64, bool> (16 bytes)
    unsigned __int64 first;
    bool             second;
    SUniqueIDOpen(const unsigned __int64& a, const bool& b) : first(a), second(b) { }
    SUniqueIDOpen(const SUniqueIDOpen& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess { CKeyPairCompareLess() { } };
void __fastcall SortUniqueIDs(SUniqueIDOpen* first, SUniqueIDOpen* last, CKeyPairCompareLess compare);   // 0x00A84160
template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(T* data) : _M_data(data) { }
};
struct CUniqueIDVector {                           // STLport vector<SUniqueIDOpen>
    SUniqueIDOpen* _M_start;
    SUniqueIDOpen* _M_finish;
    _STLP_alloc_proxy<SUniqueIDOpen> _M_end_of_storage;
    CUniqueIDVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void _M_insert_overflow(SUniqueIDOpen* position, const SUniqueIDOpen& x, const __true_type&, unsigned long fillLen, bool atEnd);   // 0x00A82380
    __forceinline void push_back(const SUniqueIDOpen& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            if (_M_finish)
                new (_M_finish) SUniqueIDOpen(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);   // stand-in: the reallocation (returns the new end slot)
#pragma auto_inline(off)
// Same-TU body of the vector growth instantiation 0x00A82380: placement copy of the value read through its const reference
// into the new storage; the empty tag is never touched.
__declspec(noinline) void CUniqueIDVector::_M_insert_overflow(SUniqueIDOpen* position, const SUniqueIDOpen& x, const __true_type& p2, unsigned long fillLen, bool atEnd)
{
    SUniqueIDOpen* p = (SUniqueIDOpen*)GrowVectorStorage_s7(this, fillLen);
    if (p)
        new (p) SUniqueIDOpen(x);
}
#pragma auto_inline(on)
struct CUniqueIDMap {                              // CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<...> >
    CUniqueIDVector     Entries;                   // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    CUniqueIDMap() : Dirty(false) { }
    __forceinline void Add(const unsigned __int64& key, const bool& value)
    {
        Entries.push_back(SUniqueIDOpen(key, value));
        Dirty = true;
    }
    void Sort()
    {
        if (Dirty) {
            SortUniqueIDs(Entries._M_start, Entries._M_finish, Compare);
            Dirty = false;
        }
    }
};

class CNavSwitchableLeafNode : public CNavNavigableLeafNode {   // vtable 0x0129DE04
public:
    CUniqueIDMap UniqueIDsOpen;                    // +0x20
    virtual void v00();
    CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, CRegionList& regions, bool readPreferability);
};

CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, CRegionList& regions, bool readPreferability)
    : CNavNavigableLeafNode(position, level, layer, file, regions, readPreferability)
{
    Blocked = true;
    unsigned long count;
    file.Read(&count, 4, 0);
    for (unsigned long i = 0; i < count; ++i) {
        unsigned __int64 id;
        file.Read(&id, 8, 0);
        UniqueIDsOpen.Add(id, false);
    }
    PRegion = 0;
    UniqueIDsOpen.Sort();
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_base = 0, g_sort = 0, g_grow = 0, g_reads = 0; static unsigned long g_count = 0; static unsigned __int64 g_ids[4] = { 9, 3, 7, 1 };
static long g_flags = -1;
void CAFile::v00() {} void CAFile::v01() {} void CAFile::v02() {}
void CAFile::Read(void* data, unsigned long size, long flags)
{ g_flags = flags; if (g_reads == 0) { CHECK(size == 4); *(unsigned long*)data = g_count; } else { CHECK(size == 8); *(unsigned __int64*)data = g_ids[(g_reads - 1) & 3]; } ++g_reads; }
void CNavNavigableLeafNode::v00() {}
void CNavSwitchableLeafNode::v00() {}
CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile&, CRegionList&, bool)
{ ++g_base; Position = position; QuadTreeLevel = level; NavLayer = layer; Blocked = false; PRegion = (void*)1; }
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{ ++g_grow; CUniqueIDVector* v = (CUniqueIDVector*)storage; long n = v->_M_finish - v->_M_start;
  SUniqueIDOpen* s = (SUniqueIDOpen*)malloc((n + count) * 2 * sizeof(SUniqueIDOpen)); if (n) memcpy(s, v->_M_start, n * sizeof(SUniqueIDOpen));
  v->_M_start = s; v->_M_finish = s + n + count; v->_M_end_of_storage._M_data = s + (n + count) * 2; return s + n; }
void __fastcall SortUniqueIDs(SUniqueIDOpen* first, SUniqueIDOpen* last, CKeyPairCompareLess)
{ ++g_sort; for (SUniqueIDOpen* i = first; i < last; ++i) for (SUniqueIDOpen* j = i + 1; j < last; ++j) if (j->first < i->first) { SUniqueIDOpen t = *i; memcpy(i, j, sizeof(t)); memcpy(j, &t, sizeof(t)); } }
int main()
{
    static CAFile file; C2DVector pos = { 1, 2 };
    g_count = 4;
    CNavSwitchableLeafNode* n = (CNavSwitchableLeafNode*)malloc(sizeof(CNavSwitchableLeafNode));
    new (n) CNavSwitchableLeafNode(pos, 3, 1, file, *(CRegionList*)0, true);
    CHECK(g_base == 1 && n->Blocked && n->PRegion == 0 && g_reads == 5 && g_flags == 0);
    CHECK(n->UniqueIDsOpen.Entries._M_finish - n->UniqueIDsOpen.Entries._M_start == 4 && g_grow >= 1 && g_sort == 1 && !n->UniqueIDsOpen.Dirty);
    SUniqueIDOpen* e = n->UniqueIDsOpen.Entries._M_start; CHECK(e[0].first == 1 && e[1].first == 3 && e[2].first == 7 && e[3].first == 9);
    CHECK(!e[0].second && !e[3].second);
    g_count = 0; g_reads = 0; CNavSwitchableLeafNode* m = (CNavSwitchableLeafNode*)malloc(sizeof(CNavSwitchableLeafNode));
    new (m) CNavSwitchableLeafNode(pos, 3, 1, file, *(CRegionList*)0, false);
    CHECK(m->UniqueIDsOpen.Entries._M_start == 0 && g_sort == 1 && g_reads == 1);   // nothing added: not dirty, no sort
    if (fails == 0) printf("00a79a30_TEST PASS\n"); else printf("00a79a30_TEST FAIL\n");
    return fails;
}