// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::_M_copy(_Link_type x, _Link_type p) -- structural copy of a red-black subtree under parent p (right subtrees recursively, the left spine iteratively)
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042CB90, length 115 bytes, `ret 8` after `leave`. The catalog's
// "CSurface_Clone_Recursive" is descriptive. Callers: itself (0x0042CBB9, 0x0042CBE6), the tree's operator=
// (0x0042CEB1, row 0x0042CE7A) and 0x0053CBDC.
// Callees: _M_clone_node @ 0x0042BE11 (landed: _M_create_node + colour copy + null children), itself.
// Shape: STLport 4.x `_Rb_tree::_M_copy` with exceptions off (`_STLP_TRY` / `_STLP_UNWIND` compile to nothing):
//   top = _M_clone_node(x); top->_M_parent = p; if (x->_M_right) top->_M_right = _M_copy(_S_right(x), top);
//   p = top; x = _S_left(x); while (x) { y = _M_clone_node(x); p->_M_left = y; y->_M_parent = p;
//   if (x->_M_right) y->_M_right = _M_copy(_S_right(x), y); p = y; x = _S_left(x); } return top;
// Frame-pointer size TU (`optimize("s", on)` + `("y", off)`): `this` homed at [ebp-4], `top` parked in the dead `x`
// argument slot across the loop. Node layout as the landed _M_clone_node (color +0, parent +4, left +8, right +0xc,
// value +0x10); the value is pair<const unsigned long, NUISystem::CUIState> (0x28 bytes, node 0x38).
#pragma optimize("s", on)
#pragma optimize("y", off)
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
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    static _Link_type& _S_left(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_right); }
    static _Link_type& _S_parent(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_parent); }
    _Link_type _M_clone_node(_Link_type x);              // 0x0042BE11
    _Link_type _M_copy(_Link_type x, _Link_type p);
};
template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::_Link_type _Rb_tree<K, V, KoV, C, A>::_M_copy(_Link_type x, _Link_type p)
{
    _Link_type top = _M_clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = _M_copy(_S_right(x), top);
    p = top;
    x = _S_left(x);
    while (x != 0)
    {
        _Link_type y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = _M_copy(_S_right(x), y);
        p = y;
        x = _S_left(x);
    }
    return top;
}
}

template std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_M_copy(std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type, std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type);