// std::_Rb_tree::equal_range(const unsigned __int64& k) -- STLport equal_range of the UID -> CCombatantInfo tree: the upper bound walk inlined, the lower bound called
// Leaf: ?equal_range@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F27B0, 70 bytes, `ret 8`.
// Label correction: catalog "BinaryTree_LowerBoundPair_Helper" is propagated; this is the equal_range of the
//   UID -> CCombatantInfo map (the combat manager's combatant map; node 0x88 bytes).
// Callees: this tree's out-of-line _M_lower_bound 0x006F0CF0.
// Shape: identical to 0x006F2760 (same template over a different value type); the value is opaque here.
struct CCombatantInfo {                                    // 0x70 bytes (node 0x88: links +0, key +0x10, value +0x18)
    unsigned char _body[0x70];
};

namespace std {
template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};

template<class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template<class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

template<class T> class allocator { public: allocator() {} };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};

template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                                 // +0x10
};

template<class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node<Value>* x) : _M_node(x) {}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = allocator<Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Link_type _M_header;                                 // +0x00
    unsigned int _M_node_count;                           // +0x04
    Compare _M_key_compare;                               // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header->_M_parent; }
    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
    static const Key& _S_key(_Link_type x) { return KeyOfValue()(x->_M_value_field); }

    _Link_type _M_lower_bound(const Key& k) const;        // out of line (this tree: 0x006F0CF0)
    _Link_type _M_upper_bound(const Key& k) const
    {
        _Link_type y = _M_header;
        _Link_type x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = _S_left(x);
            else
                x = _S_right(x);
        return y;
    }
    iterator lower_bound(const Key& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const Key& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const Key& k) { return pair<iterator, iterator>(lower_bound(k), upper_bound(k)); }
};
}

typedef std::pair<const unsigned __int64, CCombatantInfo> SCombatantByUID;
template class std::_Rb_tree<unsigned __int64, SCombatantByUID, std::_Select1st<SCombatantByUID>, std::less<unsigned __int64> >;