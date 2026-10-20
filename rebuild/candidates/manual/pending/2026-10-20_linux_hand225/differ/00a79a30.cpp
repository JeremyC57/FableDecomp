// CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, CAFile& file, list<CNavRegion>& regions, bool readPreferability) -- load a switchable leaf node: the navigable leaf part, then the list of 64-bit unique ids of the switches that open it (all read as closed), blocked by default, no region, the id map sorted
// Grade: DIFFER(249v245) -- the pair temporary's `second = false` store is hoisted to the loop top in retail (then forwarded from bl into the copy), ours stores it after the read and re-reads it; count / index homed one argument slot lower (file / regions vs regions / readPreferability); the vtable store and the first zero push are scheduled differently
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
struct C2DVector { float X, Y; };
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
template <class T> struct _STLP_alloc_proxy { T* _M_data; };
struct CUniqueIDVector {                           // STLport vector<SUniqueIDOpen>
    SUniqueIDOpen* _M_start;
    SUniqueIDOpen* _M_finish;
    _STLP_alloc_proxy<SUniqueIDOpen> _M_end_of_storage;
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
struct CUniqueIDMap {                              // CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<...> >
    CUniqueIDVector     Entries;                   // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    CUniqueIDMap() { Entries._M_start = 0; Entries._M_finish = 0; Entries._M_end_of_storage._M_data = 0; Dirty = false; }
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
