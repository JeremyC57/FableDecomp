// std::_Rb_tree::equal_range(const key_type& k) -- STLport tree equal_range over the 0x30-node tree of the 0x00776AA0 family: the lower and upper bound of a signed 4-byte key, returned as an iterator pair
// Leaf: ?equal_range@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007776A0, 83 bytes, `ret 8` (thiscall: hidden pair<iterator,iterator> result + key
//   reference).
// Identity: STLport 4.6 `pair<iterator,iterator> equal_range(const key_type& __x) { return pair<iterator, iterator>(
//   lower_bound(__x), upper_bound(__x)); }` with _M_lower_bound / _M_upper_bound inlined (arguments evaluate right to
//   left, so the upper-bound walk comes first, into edi). Its only caller is 0x00777B43 in the tree's `erase(const key&)`
//   0x00777B30 (equal_range, then __distance 0x00776CF0, then the landed range erase 0x007775B0, return the count), the
//   same instantiation as the landed 0x00777520 constructor / 0x00777560 / 0x00777A40 rows: the key/value types are the
//   per-instantiation placeholders CUnprovenKey/Value/KeyOfValue_00776aa0 used there (signed key at node +0x10, node
//   0x30). Catalog row was the placeholder `_gapscan::sub_007776a0`.
// Shape: the header pointer is loaded once (`mov ecx,[ecx]`) and doubles as both walks' "not found" node; signed
//   compares (`jge` for `k < key(x)`, `jl` for `key(x) < k`). The iterator needs STLport's user copy constructor (from the
//   non-const iterator): with the implicit one VC stores pair.second first and sinks pair.first below the pops.
struct CUnprovenKey_00776aa0 { long _M_unproven; };
inline bool operator<(const CUnprovenKey_00776aa0& __x, const CUnprovenKey_00776aa0& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenValue_00776aa0 { CUnprovenKey_00776aa0 _M_key; unsigned char _M_unproven[28]; };
struct CUnprovenKeyOfValue_00776aa0 { const CUnprovenKey_00776aa0& operator()(const CUnprovenValue_00776aa0& __v) const { return __v._M_key; } };

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
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
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                         // +0x00
    _Base_ptr _M_parent;                        // +0x04
    _Base_ptr _M_left;                          // +0x08
    _Base_ptr _M_right;                         // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                      // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
protected:
    unsigned int _M_node_count;                 // +0x04
    _Compare     _M_key_compare;                // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
public:
    private:
    _Link_type _M_lower_bound(const key_type& __k) const;
    public:
    private:
    _Link_type _M_upper_bound(const key_type& __k) const;
    public:
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x) {
        return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x));
    }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;   // last node which is not less than __k
    _Link_type __x = _M_root();                 // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_upper_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;   // last node which is greater than __k
    _Link_type __x = _M_root();                 // current node
    while (__x != 0)
        if (_M_key_compare(__k, _S_key(__x)))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}

typedef _Rb_tree<CUnprovenKey_00776aa0, CUnprovenValue_00776aa0, CUnprovenKeyOfValue_00776aa0,
                 less<CUnprovenKey_00776aa0>, allocator<CUnprovenValue_00776aa0> > _Tree_00776aa0;
template pair<_Tree_00776aa0::iterator, _Tree_00776aa0::iterator> _Tree_00776aa0::equal_range(const CUnprovenKey_00776aa0&);
}