// std::_Rb_tree::erase(iterator __position) -- (map<long, CEnginePrimitiveHandle>) unlink the node, destroy its value, free it
// Leaf: ?erase@?$_Rb_tree@JU?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Select1st@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BBAF9, 59 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): destroying the value calls the landed std::pair<const long, CEnginePrimitiveHandle>::~pair 0x004DD728 (bound ??1?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAE@XZ), so this is the map<long, CEnginePrimitiveHandle> node erase; callers in CTCInventoryClothing (0x005B9641) and 0x005BBBF3.
// Callees: std::_Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (cdecl), the value destructor 0x004DD728, free 0x00BFEA14.
// Shape (family M063, representative 0x00475418): STLport `__y = _Rebalance_for_erase(__position._M_node, _M_header._M_data->
//   _M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right); _Destroy(&__y->_M_value_field); _M_header.deallocate(
//   __y, 1); --_M_node_count;` with the allocator's null-tested free inline.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);                      // 0x00BFEA14
class CEnginePrimitiveHandle {                          // vptr + Data (8 bytes)
public:
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    ~pair();                                              // out of line (0x005BBAF9 calls it; landed 0x004DD728)
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
template <class _Tp> __forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> > class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    void erase(iterator __position)
    {
        _Rb_tree_node<_Value>* __y = (_Rb_tree_node<_Value>*)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        _M_header.deallocate(__y, 1);
        --_M_node_count;
    }
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template class _Rb_tree<long, pair<const long, CEnginePrimitiveHandle>, _Select1st<pair<const long, CEnginePrimitiveHandle> >, less<long>, allocator<pair<const long, CEnginePrimitiveHandle> > >;
}