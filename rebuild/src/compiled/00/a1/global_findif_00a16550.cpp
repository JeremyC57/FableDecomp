// __find_if(first, last, IsSameNavQuadTreeNode pred, random_access_iterator_tag) -- first node pointer in the range whose node equals the predicate's node
// Leaf: ??$__find_if@PAPBVCANavQuadTreeNode@@VIsSameNavQuadTreeNode@@@_STL@@YIPAPBVCANavQuadTreeNode@@PAPBV1@0VIsSameNavQuadTreeNode@@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A16550, 187 bytes, `ret 8`. Callers 0x00A15C2A / 0x00A1691A (CNavigatorManager
//   node searches). The catalog label "Vector_Find_NAVNode" is descriptive.
// Identity: Ego_r.pdb `find_if<vector<const CANavQuadTreeNode*>::iterator, IsSameNavQuadTreeNode>` with
//   `IsSameNavQuadTreeNode(const CANavQuadTreeNode*)` and `bool operator()(const CANavQuadTreeNode*) const`; the
//   comparison is `CANavQuadTreeNode::operator==(const CANavQuadTreeNode&) const` 0x00A763F0 (PDB ??8). The member
//   name `Node` is descriptive.
// Calling convention: STLport free template under /Gr = __fastcall: first in ecx, last in edx, the predicate (one
//   pointer, kept in ebx as the operator== `this`) and the `const random_access_iterator_tag&` on the stack.
// Shape: STLport's 4-way unrolled random-access __find_if (trip count = bytes >> 4) and the fall-through switch on
//   the 0..3 remaining elements.
class CANavQuadTreeNode {
public:
    bool operator==(const CANavQuadTreeNode& other) const;  // 0x00A763F0
};

class IsSameNavQuadTreeNode {
public:
    const CANavQuadTreeNode* Node;                          // descriptive member name
    IsSameNavQuadTreeNode(const CANavQuadTreeNode* node) : Node(node) {}
    bool operator()(const CANavQuadTreeNode* node) const { return *Node == *node; }
};

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}
}

template const CANavQuadTreeNode** __fastcall _STL::__find_if(const CANavQuadTreeNode**, const CANavQuadTreeNode**, IsSameNavQuadTreeNode, const _STL::random_access_iterator_tag&);