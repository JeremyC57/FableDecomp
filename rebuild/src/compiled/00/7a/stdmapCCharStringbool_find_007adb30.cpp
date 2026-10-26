// std::map<CCharString, bool>::find(const CCharString& k) -- STLport 4.6 map lookup: iterator(_M_t._M_find(k)), the found node or end()
// Leaf: ?find@?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ADB30, 21 bytes (+ int3 padding), thiscall, hidden iterator result
//   ([esp+4]), key [esp+8], `ret 8`; this (the map, whose _M_t is at +0) passes straight through to _M_find.
// Identity: the only callee is map<CCharString, bool>'s _Rb_tree::_M_find 0x007ACC90 (landed F012 row; its leaf is this
//   tree's _M_find symbol; the same tree CPersistContext::TransferMap<CCharString, bool> 0x007AE3D0 looks keys up in),
//   and the result is the node wrapped in an iterator returned by value -- STLport 4.6 (SGI)
//   `iterator map::find(const key_type& __x) { return _M_t.find(__x); }` with _Rb_tree::find (`iterator(_M_find(__k))`)
//   inlined. The single caller (0x00CC61B8, in the cutscene macro runner beside 0x004BAA30's caller) calls it on a local
//   non-const map and compares the node with that map's header (`it == end()`), so the non-const overload. Same shape and
//   ICF caveat as 0x004BAA30 std::set<CCharString>::find (sweep3): an identical _Rb_tree::find COMDAT would fold here too.
//   Was landed as CRegion::SetMiniMapRegionExitTextOffsetX (a propagated manifest label, the stdcall FableSet_7adb30 stand-in).
// Shape: STLport map<CCharString, bool> (_Rb_tree over _Select1st/less/allocator, 12 bytes); the iterator has a user copy
//   constructor, so it is returned through the hidden pointer.
class CCharStringData;
class CCharString {                                     // 4 bytes: the shared string block pointer
public:
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const;
};
namespace std {

template <class _Tp> class allocator { };

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
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    unsigned int _M_node_count;                         // +0x04
    _Compare _M_key_compare;                            // +0x08 (empty)
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;          // out of line
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {                                             // STLport 4.6 (SGI): 12 bytes
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) const { return _M_t.find(__x); }
    iterator end() const { return _M_t.end(); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                             // STLport 4.6 (SGI): 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
    iterator end() { return _M_t.end(); }
    const_iterator end() const { return _M_t.end(); }
};

} // namespace std

template class std::map<CCharString, bool>;
