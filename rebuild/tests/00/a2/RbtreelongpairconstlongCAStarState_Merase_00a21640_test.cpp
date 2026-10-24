// Behaviour test for _Rb_tree<long, pair<const long, CAStarState>>::_M_erase (0x00A21640): every node of a subtree is
// destroyed exactly once (the three state counters drop by the right amounts) and freed.
#include <cstdio>
#include <cstdlib>

class CANavQuadTreeNode;

class CNavigatorAStar {
public:
    class CAStarState {
    public:
        CAStarState*       ParentState;
        CANavQuadTreeNode* Node;
        float              CostToThisState;
        float              TotalCost;
        bool               OnOpenList;
        bool               OnClosedList;
        static long NoOfStates;
        static long NoOfStatesOnOpenList;
        static long NoOfStatesOnClosedList;
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
long CNavigatorAStar::CAStarState::NoOfStates = 0;
long CNavigatorAStar::CAStarState::NoOfStatesOnOpenList = 0;
long CNavigatorAStar::CAStarState::NoOfStatesOnClosedList = 0;

static int g_freed = 0;

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Link_type    _M_header;
    unsigned long _M_node_count;
    static _Link_type _S_left(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_left); }
    static _Link_type _S_right(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_right); }
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Value>
void _Rb_tree<_Key, _Value>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        std::free(__x);
        ++g_freed;
        __x = __y;
    }
}
}

typedef _STL::pair<const long, CNavigatorAStar::CAStarState> SValue;
typedef _STL::_Rb_tree_node<SValue> SNode;

static SNode* MakeNode(bool open, bool closed, SNode* left, SNode* right)
{
    SNode* n = (SNode*)std::malloc(sizeof(SNode));
    n->_M_left = left; n->_M_right = right; n->_M_parent = 0; n->_M_color = 0;
    n->_M_value_field.second.OnOpenList = open;
    n->_M_value_field.second.OnClosedList = closed;
    ++CNavigatorAStar::CAStarState::NoOfStates;
    if (open) ++CNavigatorAStar::CAStarState::NoOfStatesOnOpenList;
    if (closed) ++CNavigatorAStar::CAStarState::NoOfStatesOnClosedList;
    return n;
}

int main()
{
    int ok = 1;
    // 7 nodes: 3 open, 2 closed, 1 both, 1 neither
    SNode* a = MakeNode(true, false, 0, 0);
    SNode* b = MakeNode(false, true, 0, 0);
    SNode* c = MakeNode(true, true, a, b);
    SNode* d = MakeNode(false, false, 0, 0);
    SNode* e = MakeNode(true, false, 0, d);
    SNode* f = MakeNode(false, true, e, 0);
    SNode* root = MakeNode(true, false, c, f);
    _STL::_Rb_tree<long, SValue> tree; tree._M_header = 0; tree._M_node_count = 7;

    if (CNavigatorAStar::CAStarState::NoOfStates != 7) ok = 0;
    tree._M_erase(0);                                         // null subtree: nothing happens
    if (g_freed != 0 || CNavigatorAStar::CAStarState::NoOfStates != 7) ok = 0;
    tree._M_erase(root);
    if (g_freed != 7) ok = 0;
    if (CNavigatorAStar::CAStarState::NoOfStates != 0) ok = 0;
    if (CNavigatorAStar::CAStarState::NoOfStatesOnOpenList != 0) ok = 0;
    if (CNavigatorAStar::CAStarState::NoOfStatesOnClosedList != 0) ok = 0;

    if (!ok) { std::printf("00a21640_TEST FAIL\n"); return 1; }
    std::printf("00a21640_TEST PASS\n");
    return 0;
}