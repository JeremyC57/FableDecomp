// std::map::find(const key_type& k) -- (map<unsigned long, long>) STLport: wrap the out-of-line node lookup in an iterator returned through the hidden result pointer
// Leaf: ?find@?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKJ@std@@U?$_Nonconst_traits@U?$pair@$$CBKJ@std@@@2@@2@ABK@Z
// Retail: Fable.exe @ 0x005D8548, 20 bytes, thiscall `ret 8` (hidden iterator pointer, &k); /Os TU (packed address).
// Identity (probable): the map::find of map<unsigned long, long>: it calls the landed
// _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_M_find 0x005D83B3 (proven CTCInventoryStats::CreaturesKilled
// tree) on `this` and stores the node into the result. It sits in that size-optimised CTCInventoryStats TU next to the
// tree's _M_find / _M_upper_bound / _M_lower_bound / erase. Its one caller, 0x00AD9424 (function 0x00AD9410), finds a key
// in the map at this+0xC and reads the node's long (+0x14) as an index into a word vector -- the same COMDAT, so the same
// map<unsigned long, long> type. map::find vs the inner _Rb_tree::find cannot be told apart from bytes (both one wrapper
// over _M_find); the public map::find is the one user code names.
// Callees: _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_M_find(const unsigned long&) const 0x005D83B3.
// Shape: STLport 4.6 `iterator find(const key_type& __x) { return _M_t.find(__x); }` over `iterator(_M_find(__k))`:
// `push [esp+8]` (the key reference straight from its slot, ecx = this unchanged), the call, the node stored through the
// result pointer, the result pointer returned.
// Model: the landed _M_find 0x005D83B3's STLport model with the iterator and map::find added.
#pragma optimize("s", on)

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

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

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
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;        // 0x005D83B3
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const key_type& __x) { return _M_t.find(__x); }
};

template class map<unsigned long, long>;


} // namespace std