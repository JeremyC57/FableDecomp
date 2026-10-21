// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::_M_clone_node(_Link_type x) -- copies a node's value and colour into a fresh childless node
// Leaf: _M_clone_node
// Retail: Fable.exe @ 0x0042BE11, 30 bytes, `ret 4`. Catalog label CSurface::GetHeight is wrong. STLport _M_clone_node:
// _M_create_node(x->_M_value_field) (0x0042BDF0), then color = x's (byte at +0), left = right = 0 (`and dword ptr`).
// Callers: the recursive subtree copy _M_copy 0x0042CB90 (0x0042CB9D, 0x0042CBCD).
// Layout: STLport _Rb_tree_node_base (bool color +0, parent +4, left +8, right +0xc), value at +0x10.
// Opt: size TU (`and dword ptr [eax+8],0`).
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

template std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_M_clone_node(std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type);