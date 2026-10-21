// std::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_create_node(const value_type&) -- mallocs a 0x18-byte node and copy-constructs the pair into it
// Leaf: _M_create_node
// Retail: Fable.exe @ 0x0042B587, 33 bytes, `ret 4`. Catalog label NScript::CDanyalTestScriptsData::Alloc is wrong. STLport
// _M_create_node: malloc import 0x00BFEA0E (`push 18h`), placement new (null test) with the pair copy ctor
// 0x004297DB (CCharString copy 0x0099EC30, then a 4-byte value copy) on node +0x10. Callers: the map's _M_insert 0x0042BCB5,
// shared (identical-code folded) by several map<CCharString, 4-byte value> instantiations (e.g. the _M_upper_bound user
// 0x009D34E0), so the value type is not unique; long stands for it here.
// Opt: size TU (`pop ecx` after the cdecl malloc, `push dword ptr [esp+8]`).
#pragma optimize("s", on)
#include <stdlib.h>
#include <new>
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    size_t              _M_node_count;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
};
}

template std::_Rb_tree<CCharString, std::pair<const CCharString, long>, std::_Select1st<std::pair<const CCharString, long> >, std::less<CCharString>, std::allocator<std::pair<const CCharString, long> > >::_Link_type std::_Rb_tree<CCharString, std::pair<const CCharString, long>, std::_Select1st<std::pair<const CCharString, long> >, std::less<CCharString>, std::allocator<std::pair<const CCharString, long> > >::_M_create_node(const std::pair<const CCharString, long>&);