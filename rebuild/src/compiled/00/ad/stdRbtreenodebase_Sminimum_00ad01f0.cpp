// std::_Rb_tree_node_base::_S_minimum(_Rb_tree_node_base* __x) -- STLport 4.6: walk _M_left links down to the leftmost node of a red-black subtree
// Leaf: ?_S_minimum@_Rb_tree_node_base@std@@SAPAU12@PAU12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD01F0, 26 bytes, static __cdecl (_STLP_CALL), no callers (an out-of-line COMDAT
//   copy the linker kept; every other use is inlined).
// Identity: the body is STLport 4.6 stl/_tree.h `_S_minimum` verbatim -- one stack argument, follows +0x08 (_M_left of
//   _Rb_tree_node_base: _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC) until null, returns the node. Name is
//   probable: the Ego_r/FableWin PDBs were built against Dinkumware, so no PDB symbol names STLport internals, and the
//   same bytes would come from any leftmost-walk over that layout. Model copied from the landed 0x0083B170 _Rb_tree text.
namespace std {

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
    static _Base_ptr __cdecl _S_minimum(_Base_ptr __x);
    static _Base_ptr __cdecl _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};

_Rb_tree_node_base* __cdecl _Rb_tree_node_base::_S_minimum(_Base_ptr __x)
{
    while (__x->_M_left != 0) __x = __x->_M_left;
    return __x;
}

}   // namespace std