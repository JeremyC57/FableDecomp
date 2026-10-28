// CNavNavigableLeafNode::IsNeighbourOf(const CANavQuadTreeNode* node) const -- whether the node is in this leaf's neighbour list
// Grade: DIFFER(55v53) -- the find's random_access_iterator_tag temporary gets its own frame slot (push ecx; lea eax,[esp+7]) where retail homes it over the live node argument slot (both stack arguments are lea [esp+8]/[esp+0Ch] = &node), plus a mov edx,ecx copy; tried the inline compare, a named result (fixes the end() re-read), POD tags, a key copy (moves the TAG onto the argument slot and the key to the frame, 63v53)
// Leaf: ?IsNeighbourOf@CNavNavigableLeafNode@@UBE_NPBVCANavQuadTreeNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76C30, 53 bytes, `ret 4`. Name: Ego_r.pdb
//   ?IsNeighbourOf@CNavNavigableLeafNode@@UBE_NPBVCANavQuadTreeNode@@@Z; vtable 0x0129DDA4 slot 4; the catalog label agrees.
// Layout: Ego_r.pdb CNavNavigableLeafNode Neighbours CSmallVector {Start +0x14, schar Size +0x18}.
// Callee: STLport `__find(first, last, const T&, const random_access_iterator_tag&)` 0x00A7D0E0 (__fastcall: first in ecx,
//   last in edx, the value and the tag references on the stack; landed as a descriptive global).
// Shape: `return std::find(Neighbours.begin(), Neighbours.end(), node) != Neighbours.end();` -- end() recomputed after the
//   call (movsx of the signed size).
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

bool CNavNavigableLeafNode::IsNeighbourOf(const CANavQuadTreeNode* node) const
{
    const CANavQuadTreeNode* const* found = std::find(Neighbours.begin(), Neighbours.end(), node);
    return found != Neighbours.end();
}
