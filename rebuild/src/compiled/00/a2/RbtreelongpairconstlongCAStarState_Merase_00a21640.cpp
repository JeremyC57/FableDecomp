// _Rb_tree<long, pair<const long, CAStarState>>::_M_erase(node* x) -- free a subtree of the A* state map without rebalancing, counting each state out
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21640, 90 bytes, `ret 4`. Nine callers: itself (0x00A21656), the
//   CNavigatorAStar constructor/destructor/clear paths (0x00A1CBAD, 0x00A1CE51, 0x00A1CEE7, 0x00A1E3A6) and the tree's
//   own clear/erase helpers (0x00A21B32, 0x00A22072, 0x00A22142, 0x00A223B2). Catalog label "TreeNode_RecursiveDelete"
//   is descriptive.
// Node: CNavigatorAStar::States (map<long, CAStarState>, node 0x28): key +0x10, CAStarState +0x14 (Ego_r.pdb:
//   ParentState, Node, CostToThisState, TotalCost, OnOpenList +0x10, OnClosedList +0x11 -> node +0x24 / +0x25).
// Shape: STLport `_M_erase` (recurse right, keep left, _Destroy the value, deallocate = free 0x00BFEA14). The inline
//   ~CAStarState counts the state out of three statics 0x013D27B8 / 0x013D27BC / 0x013D27C0 (the landed scalar
//   deleting destructor 0x00A205C0 decrements the same three for the pair<const long, CAStarState>); their names are
//   not in the PDB name sets, so NoOfStates / NoOfStatesOnOpenList / NoOfStatesOnClosedList are descriptive names.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

class CANavQuadTreeNode;

class CNavigatorAStar {
public:
    class CAStarState {                                 // 0x14 bytes
    public:
        CAStarState*       ParentState;                 // +0x00
        CANavQuadTreeNode* Node;                        // +0x04
        float              CostToThisState;             // +0x08
        float              TotalCost;                   // +0x0C
        bool               OnOpenList;                  // +0x10
        bool               OnClosedList;                // +0x11
        static long NoOfStates;                         // 0x013D27B8 (descriptive)
        static long NoOfStatesOnOpenList;               // 0x013D27BC (descriptive)
        static long NoOfStatesOnClosedList;             // 0x013D27C0 (descriptive)
        ~CAStarState()
        {
            --NoOfStates;
            if (OnOpenList)
                --NoOfStatesOnOpenList;
            if (OnClosedList)
                --NoOfStatesOnClosedList;
        }
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _Rb_tree_node_base {
    char                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Link_type    _M_header;
    unsigned long _M_node_count;
    static _Link_type _S_left(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_left); }
    static _Link_type _S_right(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_right); }
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Value>
void _Rb_tree<_Key, _Value>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        free(__x);
        __x = __y;
    }
}
}

template void std::_Rb_tree<long, std::pair<const long, CNavigatorAStar::CAStarState> >::_M_erase(std::_Rb_tree_node<std::pair<const long, CNavigatorAStar::CAStarState> >*);