// std::set<unsigned long>::operator=(const std::set<unsigned long>& x) -- STLport set assignment: assigns the underlying red-black tree and returns *this
// Leaf: ??4?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514210, 19 bytes, thiscall `ret 4`, returns `this` (esi kept across the call).
//   An out-of-line COMDAT with no direct caller (the xref.py hit 0x0047FA8C decodes to no instruction).
// Identity (probable): the only callee 0x00512CE0 is STLport _Rb_tree<unsigned long, unsigned long, _Identity, less,
//   allocator>::operator= (bound by the landed std::fill<set<unsigned long>*> 0x00517060: clear through _M_erase
//   0x004A9CB0, copy through _M_copy 0x005114F0 of 0x14-byte one-dword nodes); the set keeps _M_t at +0, so
//   set::operator= is `_M_t = x._M_t; return *this;`. The key's signedness is not visible (as in 0x00517060); the only
//   Ego_r set-of-scalar in that family is set<unsigned long>.
// Label: the catalog's sub_00514210 (gap scan); 0x00512CE0's "LTextTreeWalkThrough_Assign" is a forge autoname.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
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
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                                                                                      // +0x04
    _Compare     _M_key_compare;                                                                                     // +0x08
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);   // 0x00512CE0
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                     // 12 bytes (retail STLport)
public:
    set<_Key, _Compare, _Alloc>& operator=(const set<_Key, _Compare, _Alloc>& __x) { _M_t = __x._M_t; return *this; }
};
template set<unsigned long>& set<unsigned long>::operator=(const set<unsigned long>&);
}   // namespace std