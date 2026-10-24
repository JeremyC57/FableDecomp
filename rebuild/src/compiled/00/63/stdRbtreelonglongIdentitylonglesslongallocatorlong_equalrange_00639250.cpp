// std::_Rb_tree<long,long,_Identity<long>,less<long>,allocator<long> >::equal_range(const long& k) -- STLport equal_range for the thing-map manager's active game-map set: the lower and upper bound of k found by two root-to-leaf walks
// Leaf: ?equal_range@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00639250, 81 bytes (+ int3 padding), `ret 8` (hidden pair<iterator,
// iterator> result, &k).
// Identity: the only caller is the tree's erase(const key_type&) 0x00639590 (equal_range, distance, erase(first, last)),
// which CThingMapManager calls at 0x00638308 as `ActiveGameMaps.erase(key)` on +0x20 = PDB CThingMapManager::ActiveGameMaps,
// a set<long> (the two 12-byte retail CArrays put it at +0x20, PDB +0x28); CTCVillage::RemoveCrime and
// CGameTimeManager::SetTimeAsStopped reach the same folded erase. The catalog label "RBTree_find_bounds" is descriptive.
// Callees: none.
// Shape: STLport 4.x `equal_range(k) { return pair<iterator, iterator>(lower_bound(k), upper_bound(k)); }` with both bound
// walks inlined: VC evaluates the pair arguments right to left, so the upper-bound walk (`k < key(x)`, result in edi) runs
// before the lower-bound walk (`!(key(x) < k)`, result in ecx); the header and the root are loaded once and shared. The
// iterator pair is stored straight into the hidden result.
namespace std {

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class T> struct _Identity { const T& operator()(const T& x) const { return x; } };
template <class T> class allocator { };

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                           // +0x00
    _Base_ptr   _M_parent;                          // +0x04
    _Base_ptr   _M_left;                            // +0x08
    _Base_ptr   _M_right;                           // +0x0C
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Node* _Link_type;
    typedef Key key_type;
    typedef _Rb_tree_iterator<Value> iterator;

    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;     // +0x00
    unsigned int _M_node_count;                                 // +0x04
    Compare _M_key_compare;                                     // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Base_ptr x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Base_ptr x) { return (_Link_type&)(x->_M_right); }
    static const Key& _S_key(_Base_ptr x) { return KeyOfValue()(((_Link_type)x)->_M_value_field); }

    _Link_type _M_lower_bound(const key_type& k) const
    {
        _Link_type y = this->_M_header._M_data;
        _Link_type x = _M_root();
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = _S_left(x);
            else
                x = _S_right(x);
        return y;
    }
    _Link_type _M_upper_bound(const key_type& k) const
    {
        _Link_type y = this->_M_header._M_data;
        _Link_type x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = _S_left(x);
            else
                x = _S_right(x);
        return y;
    }
    iterator lower_bound(const key_type& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const key_type& k) { return iterator(_M_upper_bound(k)); }

    pair<iterator, iterator> equal_range(const key_type& k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
};

}

template std::pair<std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> >::iterator,
                   std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> >::iterator>
std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> >::equal_range(const long& k);