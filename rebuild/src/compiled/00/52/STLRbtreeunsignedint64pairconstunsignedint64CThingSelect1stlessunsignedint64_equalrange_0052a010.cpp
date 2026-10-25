// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, _Select1st, less<unsigned __int64> >::equal_range(const unsigned __int64& k) -- STLport equal_range of CThingManager::UIDToThingMap: the pair (lower_bound(k), upper_bound(k)) returned through the hidden pointer
// Leaf: equal_range
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052A010, 70 bytes, `ret 8` (thiscall: the hidden pair result, &k).
// Callees: _M_lower_bound(const key&) const 0x00528650 (out of line; the iterator front end 0x00529010 wraps the same
//   call; manifest "BinaryTree_LowerBound_Uint64_Dup2" is propagated). _M_upper_bound is inlined.
// Callers: the map's erase(const key&) 0x0052AFB0 (used by the landed CThingManager::OnDeleteClass 0x005215F0 on
//   UIDToThingMap at CThingManager +0x58).
// Label correction: catalogued as "BinaryTree_InsertLowerBound_Uint64"; it is STLport `_Rb_tree::equal_range`.
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; the header's
//   parent is the root; nodes { colour +0, parent +4, left +8, right +0xc, unsigned 64-bit key +0x10 }.
// Shape: STLport 4.x `return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x));` -- the arguments evaluate
//   right to left, so the inlined `_M_upper_bound` walk (`key < node.key` goes left: high word `ja/jb`, low word
//   `jae`) runs first with y in esi, then `_M_lower_bound` is called; first = eax, second = esi.
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

struct SUIDThingPair {                                      // pair<const unsigned __int64, CThing*>
    unsigned __int64 first;                                 // +0x00
    CThing* second;                                         // +0x08
};

struct SUIDThingNode : _Rb_tree_node_base {
    SUIDThingPair _M_value_field;                           // +0x10
};

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

class CUIDToThingTree {                                     // _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, ...>
public:
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base*     _M_header;                      // +0x00
    unsigned long           _M_node_count;                  // +0x04
    less<unsigned __int64>  _M_key_compare;                 // +0x08

    static const unsigned __int64& _S_key(_Rb_tree_node_base* x) { return ((SUIDThingNode*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_root() const { return _M_header->_M_parent; }

    _Rb_tree_node_base* _M_lower_bound(const unsigned __int64& k) const
    {
        _Rb_tree_node_base* y = _M_header;                  // last node which is not less than k
        _Rb_tree_node_base* x = _M_root();                  // current node
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    _Rb_tree_node_base* _M_upper_bound(const unsigned __int64& k) const
    {
        _Rb_tree_node_base* y = _M_header;                  // last node which is greater than k
        _Rb_tree_node_base* x = _M_root();                  // current node
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const unsigned __int64& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const unsigned __int64& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const unsigned __int64& k);
};

pair<CUIDToThingTree::iterator, CUIDToThingTree::iterator> CUIDToThingTree::equal_range(const unsigned __int64& k)
{
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}