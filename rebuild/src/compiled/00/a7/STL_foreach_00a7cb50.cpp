// _STL::for_each(CANavQuadTreeNode** first, CANavQuadTreeNode** last, CDeleteNavNode f) -- delete every quad-tree node in a range except null entries and the shared blocked node, return the functor
// Leaf: ??$for_each@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7CB50, 51 bytes (+ int3 padding to 64), `ret 8`. /Gr algorithm TU: the hidden
//   functor-result pointer in ecx (returned in eax), first in edx; last and the empty functor's slot on the stack.
// Callees: the node's virtual deleting destructor (slot 0, `push 1`). Global: the shared CNavBlockedQuadTreeNode
//   0x013D2D2C (the landed 0x00A792E0 CNavQuadTree::ClearNodes skips it the same way; never deleted).
// Label: catalog "ForEach_CallDeleteFunctor" is descriptive; no direct caller (the nav TU's node-array teardown reaches
//   it through an inlined path). The functor's name is descriptive.
// Shape: STLport `for (; first != last; ++first) f(*first); return f;` with the functor's operator() inline
//   (`if (node && node != &blocked) delete node;` -- the delete's own null test folds into the first one); the functor
//   has user constructors, so it is returned through the hidden pointer (empty: nothing copied, eax = ecx).
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();                   // slot 0 (deleting destructor)
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};
extern CNavBlockedQuadTreeNode g_BlockedNavNode;    // 0x013D2D2C

class CDeleteNavNode {
public:
    CDeleteNavNode() { }
    CDeleteNavNode(const CDeleteNavNode&) { }
    void operator()(CANavQuadTreeNode* node) const
    {
        if (node && node != &g_BlockedNavNode)
            delete node;
    }
};

namespace _STL {
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for (; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
template CDeleteNavNode __fastcall for_each(CANavQuadTreeNode**, CANavQuadTreeNode**, CDeleteNavNode);
}