// CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability, const vector<unsigned __int64>& uniqueIDs) -- build a switchable leaf node from the generator: the navigable leaf part (inline), blocked by default, every switch unique id added to the open-id map as closed, no region, the map sorted
// Leaf: ??0CNavSwitchableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79930, 255 bytes (+ int3 padding), `ret 14h`.
// Callees: CBaseClassNonCopyable() 0x0099A2F0 (the root base, out of line), the id vector's _M_insert_overflow(pos, x, tag,
//   fill, atEnd) 0x00A82380, the `__fastcall` sort 0x00A84160 (first, last, comparator by value), malloc 0x00BFEA0E / free
//   0x00BFEA14 (stand-in only).
// Vtables: 0x0129CBBC CANavQuadTreeNode, 0x0129DE04 CNavSwitchableLeafNode (the inline CNavNavigableLeafNode store is dead).
// Layout: as the file ctor 0x00A79A30 (UniqueIDsOpen +0x20: vector, comparator byte +0x2c, Dirty +0x2d); leaf base
//   PRegion +0x10, Neighbours +0x14/+0x18, PreferabilityCost +0x1c.
// Shape: same-TU stand-ins for the growth path and the sort (auto_inline off); the base chain inline (Blocked's false store is dead, overwritten by the body); the loop re-reads the source
//   vector's size; the pair's constant `false` is hoisted out of the loop; PRegion is cleared again in the body.
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
struct CBaseClassNonCopyable {                     // vtable 0x0129A7C4
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};
class CANavQuadTreeNode : public CBaseClassNonCopyable {   // vtable 0x0129CBBC
public:
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    virtual void v00();
    CANavQuadTreeNode(const C2DVector& position, unsigned char level, unsigned char layer, bool leaf, bool blocked)
        : Position(position), QuadTreeLevel(level), NavLayer(layer), LeafNode(leaf), Blocked(blocked) { }
};
template <class T, int N> class CSmallVector { public:
    T*          Data;                              // +0
    signed char Count;                             // +4
    CSmallVector() : Data(0), Count(0) { }
};
class CNavRegion;
class CNavNavigableLeafNode : public CANavQuadTreeNode {   // vtable 0x0129DDA4
public:
    CNavRegion*                               PRegion;             // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;          // +0x14
    unsigned char                             PreferabilityCost;   // +0x1c
    virtual void v00();
    CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability)
        : CANavQuadTreeNode(position, level, layer, true, false), PRegion(0), PreferabilityCost(preferability) { }
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

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
// Same-TU stand-ins (retail compiled these in this TU and VC7.1 used what it knew of them): the vector growth path
// 0x00A82380 and the map sort 0x00A84160 (an introsort in retail; a plain insertion sort here).
#pragma auto_inline(off)
void CUniqueIDVector::_M_insert_overflow(SUniqueIDOpen* position, const SUniqueIDOpen& x, const __true_type&, unsigned long fillLen, bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    SUniqueIDOpen* newStart = (SUniqueIDOpen*)malloc(len * sizeof(SUniqueIDOpen));
    SUniqueIDOpen* newFinish = newStart;
    for (SUniqueIDOpen* p = _M_start; p != position; ++p, ++newFinish)
        new (newFinish) SUniqueIDOpen(*p);
    for (unsigned long i = 0; i < fillLen; ++i, ++newFinish)
        new (newFinish) SUniqueIDOpen(x);
    if (!atEnd)
        for (SUniqueIDOpen* p = position; p != _M_finish; ++p, ++newFinish)
            new (newFinish) SUniqueIDOpen(*p);
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage._M_data = newStart + len;
}
void __fastcall SortUniqueIDs(SUniqueIDOpen* first, SUniqueIDOpen* last, CKeyPairCompareLess)
{
    for (SUniqueIDOpen* i = first; i != last; ++i)
        for (SUniqueIDOpen* j = i; j != first && j->first < (j - 1)->first; --j) {
            SUniqueIDOpen t(*j);
            *j = *(j - 1);
            *(j - 1) = t;
        }
}
#pragma auto_inline(on)

template <class T> struct CRetailVector {         // STLport vector<T>
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long n) const { return *(_M_start + n); }
};

class CNavSwitchableLeafNode : public CNavNavigableLeafNode {   // vtable 0x0129DE04
public:
    CUniqueIDMap UniqueIDsOpen;                    // +0x20
    virtual void v00();
    CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability, const CRetailVector<unsigned __int64>& uniqueIDs);
};

CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability, const CRetailVector<unsigned __int64>& uniqueIDs)
    : CNavNavigableLeafNode(position, level, layer, preferability)
{
    Blocked = true;
    for (unsigned long i = 0; i < uniqueIDs.size(); ++i)
        UniqueIDsOpen.Add(uniqueIDs[i], false);
    PRegion = 0;
    UniqueIDsOpen.Sort();
}