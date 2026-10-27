// std::_Rb_tree::_M_create_node(const value_type& __x) -- (map<long, CCharString>) allocate a node and copy-construct the pair into it
// Leaf: ?_M_create_node@?$_Rb_tree@JU?$pair@$$CBJVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBJVCCharString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCCharString@@@std@@@2@@std@@QAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCCharString@@@std@@@2@ABU?$pair@$$CBJVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00466255, 44 bytes, thiscall (this unused), `ret 4` (size-optimised module).
// Identity (proven): its callers are the landed tree copy 0x004661C5 (map<long, CCharString> copy constructor 0x0062EE80 and assignment 0x00466152) and the unique insert 0x00472A05; the value copy is CCharString's copy constructor 0x0099EC30 (node 0x18 = 0x10 + key + CCharString).
// Callees: malloc 0x00BFEA0E (node allocator), CCharString copy constructor 0x0099EC30.
// Shape (family M049, representative 0x00466255): STLport `__tmp = _M_header.allocate(1); _Construct(&__tmp->_M_value_field,
//   __x); return __tmp;` -- allocate/_Construct `__forceinline`; STLport's explicit pair copy constructor copies the key
//   dword, then calls V's copy constructor at +4.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } __forceinline _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    _T1 first;
    _T2 second;
};
}
class CCharString {
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
namespace std {
template <class _Pair> struct _Select1st { };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> > class _Rb_tree {
public:
    _Rb_tree_node<_Value>* _M_create_node(const _Value& __x)
    {
        _Rb_tree_node<_Value>* __tmp = _M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template class _Rb_tree<long, pair<const long, CCharString>, _Select1st<pair<const long, CCharString> >, less<long>, allocator<pair<const long, CCharString> > >;
}