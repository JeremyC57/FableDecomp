// std::_Rb_tree_iterator<CUnprovenValue_0083ADA0, std::_Nonconst_traits<CUnprovenValue_0083ADA0>>::operator*() const -- the value of the node the iterator is on (the node's _M_value_field, +0x10)
// Leaf: ??D?$_Rb_tree_iterator@U?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@std@@U?$_Nonconst_traits@U?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@std@@@2@@std@@QBEAAU?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083ADA0, 6 bytes, thiscall: mov eax,[ecx] ; add eax,10h ; ret.
// Identity (descriptive, unproven instantiation): one of eight unreferenced STLport 4.6 red-black-tree helpers 0x0083ADA0..0x0083AE20
//   (stl/_tree.h text; the node's value field at +0x10 after _M_color / _M_parent / _M_left / _M_right) kept as COMDATs of the
//   TU that also holds the landed _Rb_tree<CUnprovenKey_0083b120,...>::find 0x0083B120 and CRetailTreeIterator::Decrement
//   0x0083AEA0 (retail links without /OPT:REF). No call, jump or absolute reference to any of them exists (xref.py), so no
//   caller fixes the key/mapped types: the run shares the placeholders CUnprovenKey/Data/Value/Tree_0083ADA0 (a map, as the
//   _Select1st member 0x0083AE20 implies), and the member choice among byte-identical candidates is noted per row.
// Shape: explicit instantiation emits the inline member as its own COMDAT.
// Member: operator*() (returns `_M_node->_M_value_field`); operator->() (`&operator*()`) has the same bytes.
struct CUnprovenKey_0083ADA0 { long _M_unproven; };
inline bool operator<(const CUnprovenKey_0083ADA0& __x, const CUnprovenKey_0083ADA0& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenData_0083ADA0 { long _M_unproven; };
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef value_type& reference;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    static reference __cdecl _S_value(_Link_type __x) { return __x->_M_value_field; }
public:
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};
}
typedef std::pair<const CUnprovenKey_0083ADA0, CUnprovenData_0083ADA0> CUnprovenValue_0083ADA0;
typedef std::_Rb_tree<CUnprovenKey_0083ADA0, CUnprovenValue_0083ADA0, std::_Select1st<CUnprovenValue_0083ADA0>,
                      std::less<CUnprovenKey_0083ADA0>, std::allocator<CUnprovenValue_0083ADA0> > CUnprovenTree_0083ADA0;

template struct std::_Rb_tree_iterator<CUnprovenValue_0083ADA0, std::_Nonconst_traits<CUnprovenValue_0083ADA0> >;