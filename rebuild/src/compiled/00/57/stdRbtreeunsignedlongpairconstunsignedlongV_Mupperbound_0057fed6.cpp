// std::_Rb_tree<unsigned long, pair<const unsigned long, V>, ...>::_M_upper_bound(const unsigned long&) const -- first node whose key is greater than k
// Leaf: _M_upper_bound
// Retail: Fable.exe @ 0x0057FED6, 37 bytes, `ret 4`. Catalog label UInt_RBTreeFindLess describes it; the STLport name is
// _M_upper_bound: start from the header (+0x00 of the tree), walk from the root (header->_M_parent +0x04), go left (+0x08)
// remembering the node while k < key (+0x10, unsigned compare), else right (+0x0c). Its only callers are the map's
// upper_bound wrappers (0x005806B4 / 0x00580B37: `call ; mov [ret],eax`), next to the _M_lower_bound twin 0x0057FEB1.
// Layout: STLport _Rb_tree_node (color +0, parent +4, left +8, right +0xc, value +0x10); the mapped type does not reach
// the bytes, so long stands for it.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<V>*   _Link_type;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    C         _M_key_compare;
    _Base_ptr _M_root() const { return _M_header->_M_parent; }
    static const K& _S_key(_Base_ptr x) { return KoV()(((_Link_type)x)->_M_value_field); }
    _Base_ptr _M_upper_bound(const K& k) const
    {
        _Base_ptr y = _M_header;
        _Base_ptr x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
};
}
typedef std::pair<const unsigned long, long> CULongValue;
template class std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >;