// CNavSwitchableLeafNode::IsNeighbourOf(const CANavQuadTreeNode* node) const -- a blocked (switched-off) node has no neighbours; otherwise the navigable leaf's neighbour-list test
// Grade: DIFFER(77v72) -- the inlined base's by-value node copy lands in a new frame slot (push ecx; mov [esp+4],eax) where retail homes it, and the find's tag temporary, in the node argument slot (mov [esp+8],eax self-copy; both pushes lea [esp+8]/[esp+0Ch]); tried the early-return and the then-arm-call layouts (the latter fixes the branch)
// Leaf: ?IsNeighbourOf@CNavSwitchableLeafNode@@UBE_NPBVCANavQuadTreeNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76C70, 72 bytes, `ret 4`. Name: Ego_r.pdb
//   ?IsNeighbourOf@CNavSwitchableLeafNode@@UBE_NPBVCANavQuadTreeNode@@@Z; the catalog label agrees.
// Layout: Ego_r.pdb CANavQuadTreeNode Blocked +0xF; CNavNavigableLeafNode Neighbours {Start +0x14, schar Size +0x18};
//   CNavSwitchableLeafNode adds UniqueIDsOpen (CVectorMap) at +0x20.
// Callee: STLport `__find` 0x00A7D0E0 (as 0x00A76C30).
// Shape: `if (Blocked) return false; return CNavNavigableLeafNode::IsNeighbourOf(node);` with the base body inline (its
//   by-value parameter is the self-copy into the argument slot).
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
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const      // 0x00A76C30, inline in the class
    {
        const CANavQuadTreeNode* const* found = std::find(Neighbours.begin(), Neighbours.end(), node);
        return found != Neighbours.end();
    }
protected:
    virtual void RemoveNeighbours();
};

class CNavSwitchableLeafNode : public CNavNavigableLeafNode {
public:
    unsigned char UniqueIDsOpen[0x10];                  // +0x20 CVectorMap<unsigned __int64, bool>
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;
};

bool CNavSwitchableLeafNode::IsNeighbourOf(const CANavQuadTreeNode* node) const
{
    if (!Blocked)
        return CNavNavigableLeafNode::IsNeighbourOf(node);
    return false;
}
