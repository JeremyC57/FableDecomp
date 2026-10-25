// _Rb_tree<long, pair<const long, CAStarState>>::_M_insert(x, y, v, w) -- link a new A* state node under parent y (left or right by the hint and key order), then rebalance
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@U?$_Select1st@U?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCAStarState@CNavigatorAStar@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21D90, 180 bytes, `ret 14h`. Callers 0x00A22203 / 0x00A2223B (the tree's
//   insert_unique paths). The catalog label "RBTree_InsertNodeAStar" is descriptive.
// Identity: the tree under CNavigatorAStar's state map (map<long, CAStarState>, node 0x28; see _M_erase 0x00A21640).
//   The value copy is the out-of-line pair<const long, CAStarState> copy constructor 0x00A20670 (key, the five
//   CAStarState fields, then the three live-state counters 0x013D27B8/BC/C0); an explicit-specialisation declaration
//   keeps it out of line. Signed key compare (`jl`).
// Shape: STLport 4.6 `_Rb_tree::_M_insert` verbatim (as 0x00A2C4E0), the new node held in esi across the copy call.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

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
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
// The node value's copy runs out of line (0x00A20670: key, CAStarState's copy constructor and its live-state counters).
template <> pair<const long, CNavigatorAStar::CAStarState>::pair(const pair<const long, CNavigatorAStar::CAStarState>& __p);
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    typedef _Value value_type;
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;

    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const value_type& __v, _Rb_tree_node_base* __w = 0);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}

template class std::_Rb_tree<long, std::pair<const long, CNavigatorAStar::CAStarState>,
    std::_Select1st<std::pair<const long, CNavigatorAStar::CAStarState> >, std::less<long> >;