// CNavNavigableLeafNode::RemoveNeighbours() -- drop the neighbour list (free its block, empty it) and forget the region
// Leaf: ?RemoveNeighbours@CNavNavigableLeafNode@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76C10, 28 bytes. Name: Ego_r.pdb ?RemoveNeighbours@CNavNavigableLeafNode@@MAEXXZ;
//   CNavNavigableLeafNode vtable 0x0129DDA4 slot 19 (rtti-vtables.tsv); the catalog label agrees.
// Layout: Ego_r.pdb CNavNavigableLeafNode PRegion +0x10, Neighbours CSmallVector {Start +0x14, schar Size +0x18}.
// Callee: operator delete[] 0x00BFEB1C.
// Shape: the inline CSmallVector::clear (delete[] Start, Start = 0, Size = 0), then PRegion = 0.
void __cdecl operator delete[](void* block);            // 0x00BFEB1C (MSVCR71 ??_V import thunk)
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode;
class CNavRegion;
namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&);   // 0x00A7D0E0 (STLport, unrolled by four)
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
}
template <class T, int N> class CSmallVector {          // Ego_r.pdb CSmallVector<const CANavQuadTreeNode*, 8>, 8 bytes
public:
    T*          Start;                                  // +0x00
    signed char Size;                                   // +0x04
    T* begin() { return Start; }
    const T* begin() const { return Start; }
    T* end() { return Start + Size; }
    const T* end() const { return Start + Size; }
    void clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
};
class CANavQuadTreeNode {                               // vtable 0x0129CBBC, 0x10 bytes
public:
    virtual ~CANavQuadTreeNode();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;   // slot 4
    C2DVector     Position;                             // +0x04
    unsigned char QuadTreeLevel;                        // +0x0C
    unsigned char NavLayer;                             // +0x0D
    bool          LeafNode;                             // +0x0E
    bool          Blocked;                              // +0x0F
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {   // vtable 0x0129DDA4, 0x20 bytes
public:
    CNavRegion*                                PRegion;             // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8>  Neighbours;          // +0x14
    unsigned char                              PreferabilityCost;   // +0x1C
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;
protected:
    virtual void RemoveNeighbours();
};

void CNavNavigableLeafNode::RemoveNeighbours()
{
    Neighbours.clear();
    PRegion = 0;
}