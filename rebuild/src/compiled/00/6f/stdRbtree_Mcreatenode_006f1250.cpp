// std::_Rb_tree::_M_create_node(const pair<const unsigned __int64, CThing*>&) -- (map<unsigned __int64, CThing*>) STLport tree node allocation: malloc a 0x20-byte node and copy-construct the value into it
// Leaf: ?_M_create_node@?$_Rb_tree@_KU?$pair@$$CB_KPAVCThing@@@std@@U?$_Select1st@U?$pair@$$CB_KPAVCThing@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KPAVCThing@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CB_KPAVCThing@@@std@@@2@ABU?$pair@$$CB_KPAVCThing@@@2@@Z
// Retail: Fable.exe @ 0x006F1250, 42 bytes, thiscall `ret 4`: `push 20h ; call malloc ; lea ecx,[eax+10h] ; add esp,4 ;
// test ecx,ecx ; je ; three dword copies from the argument ; ret 4` (the node stays in eax).
// Identity (probable): STLport 4.6 `_M_create_node` (`__tmp = _M_header.allocate(1); _Construct(&__tmp->_M_value_field, __x);
// return __tmp;`) for a 0x20-byte node whose 16-byte, 8-aligned value copies three words: the
// pair<const unsigned __int64, CThing*> of the adjacent landed tree insert 0x006F1280 (labelled CThingCompareMap::Insert,
// which inlines the same allocation and copy). The row is unreferenced (the scan's ".text 0x0050D768" xref is a false hit
// inside a call instruction).
// Callee: malloc 0x00BFEA0E (the MSVCR71 import thunk, called directly; STLport's allocator under _STLP_USE_MALLOC).
// Shape: placement new keeps its null test on the value address; pair's memberwise copy ctor gives the three esi moves.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* __p) { return __p; }
class CThing;
namespace std {
template <class _Tp> class allocator { public:
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
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
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Value                 value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
};
template class _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, _Select1st<pair<const unsigned __int64, CThing*> >,
                        less<unsigned __int64>, allocator<pair<const unsigned __int64, CThing*> > >;
}