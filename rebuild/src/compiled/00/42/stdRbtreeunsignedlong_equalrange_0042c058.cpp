// std::_Rb_tree<unsigned long, ...>::equal_range(const unsigned long&) -- the [lower_bound, upper_bound) pair for a key
// Leaf: equal_range
// Retail: Fable.exe @ 0x0042C058, 42 bytes, `ret 8` (hidden pair return slot + key). Catalog label
// CBankFile::CompareDataOffset::operator() is wrong: the body calls _M_upper_bound (0x0042AEB5) then _M_lower_bound
// (0x0042AE90) -- right-to-left argument evaluation of `pair(lower_bound(k), upper_bound(k))` -- and stores them as
// {first, second}. Both helpers compare the key as unsigned long at node +0x10. The only caller (0x0042CDF0, CBankFile)
// feeds the pair to distance() (0x0042A8BD), i.e. a multimap/multiset count by unsigned long key; the mapped type does
// not reach the bytes, so long stands for it.
// Opt: size TU (unaligned entry, `push dword ptr [esp+10h]`). Shape: STLport's _Rb_tree_iterator has a user copy
// constructor (its converting ctor from iterator); without it VC7.1 moves `pop edi` ahead of both pair stores.
#pragma optimize("s", on)
namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<V> iterator;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    _Base_ptr _M_lower_bound(const K& k) const;          // 0x0042AE90
    _Base_ptr _M_upper_bound(const K& k) const;          // 0x0042AEB5
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const K& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const K& k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
};
}
typedef std::pair<const unsigned long, long> CULongValue;
template std::pair<std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >::iterator, std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >::iterator> std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >::equal_range(const unsigned long&);