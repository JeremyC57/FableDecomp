// CNavNavigableLeafNode::~CNavNavigableLeafNode() -- unlink this node from every neighbour's neighbour list (find it, swap it with the last entry, shrink by one), then free the neighbour storage and run the base destructors
// Leaf: ??1CNavNavigableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76A80, 260 bytes (+ int3 padding), plain `ret`.
// Callees: the neighbour's virtual slot 1 (+4, its neighbour list), the `__fastcall` STLport find(first, last, const T&,
//   const random_access_iterator_tag&) 0x00A7D050, CSmallVector<const CANavQuadTreeNode*, 8>::resize(unsigned long,
//   const T&) 0x00A7DB10, operator delete[] 0x00BFEB1C, ~CBaseClassNonCopyable 0x0099A300.
// Vtables: 0x0129DDA4 CNavNavigableLeafNode, 0x0129CBBC CANavQuadTreeNode (inline base dtor).
// Layout: engine/CNavNavigableLeafNode.h (Ego_r.pdb): Neighbours +0x14 (data pointer, signed byte count +0x18).
// Shape: same-TU stand-ins for find / resize let VC7.1 hoist the loop-invariant temporaries as retail's build did; the find key (`this` as a const node pointer) and resize's zero fill are loop-invariant temporaries stored once
//   before the loop; every list access goes through the virtual getter; the CSmallVector dtor is inline.
void __cdecl operator delete[](void* p);           // 0x00BFEB1C
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CBaseClassNonCopyable {                     // vtable 0x0129A7C4
    virtual void v00();
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
class CANavQuadTreeNode;
template <class T, int N> struct CSmallVector {
    T*          Data;                              // +0
    signed char Count;                             // +4
    T* begin() const { return Data; }
    T* end() const { return Data + Count; }
    long size() const { return Count; }
    T& operator[](long n) const { return Data[n]; }
    void resize(long size, const T& value = T());  // 0x00A7DB10 (same-TU stand-in below)
    ~CSmallVector()
    {
        delete[] Data;
        Data = 0;
        Count = 0;
    }
};
typedef CSmallVector<const CANavQuadTreeNode*, 8> CNodeNeighbours;
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22
// Same-TU stand-ins (retail compiled these templates in this TU; VC7.1 uses what it knows of them): the STLport
// 4-way unrolled find 0x00A7D050 (as landed) and CSmallVector::resize 0x00A7DB10.
#pragma auto_inline(off)
const CANavQuadTreeNode** __fastcall find(const CANavQuadTreeNode** first, const CANavQuadTreeNode** last, const CANavQuadTreeNode* const& value, const random_access_iterator_tag&)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}
template <class T, int N> void CSmallVector<T, N>::resize(long size, const T& value)
{
    if (size <= 0)
        return;
    T* data = (T*)operator new[](size * sizeof(T));
    long i;
    for (i = 0; i < (size < Count ? size : (long)Count); ++i)
        data[i] = Data[i];
    for (long j = 0; j < size - Count; ++j)
        data[Count + j] = value;
    delete[] Data;
    Data = data;
    Count = (signed char)size;
}
template struct CSmallVector<const CANavQuadTreeNode*, 8>;
#pragma auto_inline(on)
template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

class CANavQuadTreeNode : public CBaseClassNonCopyable {   // vtable 0x0129CBBC
public:
    float         PositionX, PositionY;            // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    virtual CNodeNeighbours* GetNeighbours();      // slot 1 (+4)
    ~CANavQuadTreeNode() { }
};
struct CNavRegion;
class CNavNavigableLeafNode : public CANavQuadTreeNode {   // vtable 0x0129DDA4
public:
    CNavRegion*     PRegion;                       // +0x10
    CNodeNeighbours Neighbours;                    // +0x14
    unsigned char   PreferabilityCost;             // +0x1c
    virtual CNodeNeighbours* GetNeighbours();
    ~CNavNavigableLeafNode();
};

CNavNavigableLeafNode::~CNavNavigableLeafNode()
{
    for (const CANavQuadTreeNode** it = Neighbours.begin(); it != Neighbours.end(); ++it) {
        CANavQuadTreeNode* neighbour = (CANavQuadTreeNode*)*it;
        const CANavQuadTreeNode** self = find(neighbour->GetNeighbours()->begin(), neighbour->GetNeighbours()->end(), this, random_access_iterator_tag());
        if (self - neighbour->GetNeighbours()->begin() < neighbour->GetNeighbours()->size() - 1)
            swap(*self, (*neighbour->GetNeighbours())[neighbour->GetNeighbours()->size() - 1]);
        CNodeNeighbours* list = neighbour->GetNeighbours();
        list->resize(list->size() - 1);
    }
}