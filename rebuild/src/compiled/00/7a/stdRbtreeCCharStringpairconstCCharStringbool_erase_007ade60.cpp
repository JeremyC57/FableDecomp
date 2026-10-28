// std::_Rb_tree<CCharString, pair<const CCharString, bool> >::erase(const CCharString& key) -- STLport erase by key: take the equal_range, count it with distance, erase that range, return the count
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ADE60, 92 bytes, thiscall `ret 4`.
// Identity (proven): reloc_bindings ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@...QAEIABVCCharString@@@Z; its lower bound 0x007ACBB0 is map<CCharString, bool>'s.
// Callees: _M_upper_bound 0x007ACD30 and _M_lower_bound 0x007ACBB0 (equal_range's pair arguments, evaluated right to left),
//   __distance(first, last, const input_iterator_tag&) 0x007ABAB0 (cdecl, out of line), erase(iterator, iterator) 0x007AD180.
// Shape (family K111, representative 0x004BB270): the landed _Rb_tree::erase(const key_type&) 0x0042CDE0 (family G053) in a
//   speed TU with equal_range inline: the distance call pops 8 bytes and its third slot is reused for erase's first iterator.
//   __distance's STLport body sits in this TU as a noinline stand-in, so VC7.1 sees it never writes the pair and keeps both
//   iterators in registers across the call, as retail does.
class CCharStringData;
class CCharString {                                      // 4 bytes: PStringData
public:
    CCharStringData* PStringData;
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
template <class _Iterator> struct iterator_traits {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::difference_type difference_type;
};
template <class _InputIterator>
__declspec(noinline) typename iterator_traits<_InputIterator>::difference_type __cdecl
__distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)   // 0x007ABAB0
{
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it; ++__n;
    }
    return __n;
}
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type __cdecl
distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, typename iterator_traits<_InputIterator>::iterator_category());
}
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef int difference_type;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    private:
    _Base_ptr _M_lower_bound(const key_type& __k) const;        // 0x007ACBB0
    public:
    private:
    _Base_ptr _M_upper_bound(const key_type& __k) const;        // 0x007ACD30
    public:
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x) { return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x)); }
    void erase(iterator __first, iterator __last);              // 0x007AD180
    size_type erase(const key_type& __x)
    {
        pair<iterator, iterator> __p = equal_range(__x);
        size_type __n = distance(__p.first, __p.second);
        erase(__p.first, __p.second);
        return __n;
    }
};
typedef CCharString _TreeKey;
typedef pair<const CCharString, bool > _TreeValue;
template _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::size_type
    _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::erase(const _TreeKey&);
}