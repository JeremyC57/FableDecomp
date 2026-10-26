// std::_Rb_tree<long, pair<const long, pair<long, long> >, _Select1st, less<long> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert for map<long, pair<long,long> >: link a new node as y's left child (y is the header, or x is set, or v.first < key(y), unless a hint w is given) else as its right child, fix the root/leftmost/rightmost links, rebalance, count it, return the iterator
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00616F90, 203 bytes (+5 int3), thiscall `ret 14h` (this -> edi; the hidden
//   iterator result, x, y, &v and the hint w on the stack).
// Callees: malloc 0x00BFEA0E (`allocate(1)` folds to malloc(0x1C): 16-byte node base + the 12-byte value),
//   _Rb_global<bool>::_Rebalance(x, root&) 0x0042971D (static, __cdecl).
// Callers: nine sites in the same tree's insert_unique(v) 0x00617060 and insert_unique(hint, v) 0x00617D50 (the latter
//   is what map<long, pair<long,long> >::operator[] 0x00618DE0 calls).
// Label correction: the catalog `RBTree_Insert` is behavioural; the 0x1C-byte node with a signed key at +0x10 and an
//   8-byte mapped value is the map<long, pair<long,long> > that FableWin.pdb names for operator[] (CTCInventoryMap::
//   CreateMapMarkers' per-region counters); the body is STLport 4.x `_M_insert` verbatim.
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; nodes =
//   _Rb_tree_node_base { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC } + the value (key +0x10, pair +0x14).
// Shape: speed TU (no pragma), the landed set<CCharString> `_M_insert` 0x004ABC70 with this tree's types: VC7.1 lays
//   the right arm out first, each arm's inline _M_create_node null-tests &node->_M_value_field and copies the three
//   value words through edx (STLport's explicit pair copy constructors).
#include <new>

extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

namespace std {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> struct allocator { };

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x0
    _Rb_tree_node_base* _M_parent;                  // +0x4
    _Rb_tree_node_base* _M_left;                    // +0x8
    _Rb_tree_node_base* _M_right;                   // +0xC
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                          // +0x10
};

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _Link_type   _M_header;                         // +0x0 (_STLP_alloc_proxy<_Link_type>::_M_data)
    unsigned int _M_node_count;                     // +0x4
    _Compare     _M_key_compare;                    // +0x8

    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }

protected:
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<_Value>));
        new (&__tmp->_M_value_field) _Value(__x);
        return __tmp;
    }
public:

private:
    iterator _M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v,
                       _Rb_tree_node_base* __w_ = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_,
                                                                 const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == _M_header ||
        (__w == 0 &&                                // If w != 0, the remainder fails to false
         (__x != 0 ||                               // If x != 0, the remainder succeeds to true
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;                         // also makes _M_leftmost() = __z when __y == _M_header
        if (__y == _M_header) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;                    // maintain _M_leftmost() pointing to min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;                   // maintain _M_rightmost() pointing to max node
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}

typedef pair<const long, pair<long, long> > TCounterValue;
template _Rb_tree<long, TCounterValue, _Select1st<TCounterValue>, less<long>, allocator<TCounterValue> >::iterator
_Rb_tree<long, TCounterValue, _Select1st<TCounterValue>, less<long>, allocator<TCounterValue> >::_M_insert(
    _Rb_tree_node_base*, _Rb_tree_node_base*, const TCounterValue&, _Rb_tree_node_base*);

}