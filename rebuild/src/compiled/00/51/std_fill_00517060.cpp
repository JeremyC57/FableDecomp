// std::fill(std::set<unsigned long>* first, std::set<unsigned long>* last, const std::set<unsigned long>& value) -- (std::set<unsigned long>) STLport fill: assign value to every element of [first, last) through the element's assignment (set::operator= inline, its _Rb_tree::operator= 0x00512CE0)
// Leaf: ??$fill@PAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@V12@@std@@YIXPAV?$set@KU?$less@K@std@@V?$allocator@K@2@@0@0ABV10@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00517060, 37 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx, the value
//   reference on the stack), `ret 4`.
// Identity (probable): std::fill<std::set<unsigned long>*, std::set<unsigned long> >; evidence: the only caller 0x0051C920 is the _M_fill_insert of a vector of 12-byte elements (stride 0xC) whose assignment 0x00512CE0 is an STLport _Rb_tree operator= (clear through _M_erase 0x004A9CB0, copy through _M_copy 0x005114F0); _M_copy allocates 0x14-byte nodes and copies one dword value, _M_erase frees nodes without a value destructor -- a set of a 4-byte scalar; the only Ego_r vector-of-set type is vector<std::set<unsigned long> > (the key's signedness is not visible in the copy) (stride 0xC asserted)
// Label: the manifest's "fill<CArray<unsigned_short>*,CArray<unsigned_short>_>" is propagated from 0x00F3B850 (five byte-similar fill bodies; retail has no identical-code folding,
//   so each address is its own element type).
// Callee: std::_Rb_tree<unsigned long, unsigned long, _Identity<unsigned long>, less<unsigned long>, allocator<unsigned long> >::operator= (STLport spelling; set::operator= is inline `_M_t = __x._M_t`) 0x00512CE0.
// Body: STLport 4.6 `for ( ; __first != __last; ++__first) *__first = __val;` (explicit instantiation of the inline template).
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
}   // namespace std
namespace std {
template <class _ForwardIter, class _Tp>
inline void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}   // namespace std
template void __fastcall std::fill<std::set<unsigned long>*, std::set<unsigned long> >(std::set<unsigned long>*, std::set<unsigned long>*, const std::set<unsigned long>&);