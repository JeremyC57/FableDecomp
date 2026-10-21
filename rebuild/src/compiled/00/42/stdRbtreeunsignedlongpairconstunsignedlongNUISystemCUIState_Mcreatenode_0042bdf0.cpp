// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::_M_create_node(const value_type&) -- mallocs a 0x38-byte node and copy-constructs the pair into it
// Leaf: _M_create_node
// Retail: Fable.exe @ 0x0042BDF0, 33 bytes, `ret 4`. Catalog label NScript::CDanyalTestScriptsData::Alloc is wrong. STLport
// _M_create_node: allocate(1) through the malloc import 0x00BFEA0E (`push 38h`), then _Construct = placement new (null test)
// with pair<const unsigned long, CUIState>'s copy ctor 0x0042A944 on node +0x10. Callers: _M_clone_node 0x0042BE11 and the
// map's _M_insert 0x0042CB08 / 0x0042CB30. The map is NUISystem's map<unsigned long, CUIState> (Ego_r.pdb _Tmap_traits@KVCUIState).
// Layout: STLport _Rb_tree_node_base (color, parent, left, right) then the value at +0x10; CUIState 0x24 bytes.
// Opt: size TU (`pop ecx` after the cdecl malloc, `push dword ptr [esp+8]`).
#pragma optimize("s", on)
#include <stdlib.h>
#include <new>
namespace NUISystem {
class CUIState {                                         // 0x24 bytes (Ego_r.pdb 36)
public:
    unsigned long StateChangeFlag;
    long          StateChangeType;
    float         Position[2];
    float         Zoom[2];
    unsigned long Colour;
    float         UpdateTime;
    bool          LinearChange;
    CUIState(const CUIState& s);                         // 0x00429E29 (out of line)
};
}
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

template std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_M_create_node(const std::pair<const unsigned long, NUISystem::CUIState>&);