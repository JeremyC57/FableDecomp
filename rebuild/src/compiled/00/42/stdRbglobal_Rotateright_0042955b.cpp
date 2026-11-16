// std::_Rb_global<bool>::_Rotate_right(_Rb_tree_node_base* x, _Rb_tree_node_base*& root) -- STLport red-black tree right rotation about x
// Leaf: ?_Rotate_right@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z
// Retail: Fable.exe @ 0x0042955B, 64 bytes, cdecl static (`ret`); called by _Rb_global<bool>::_Rebalance 0x0042971D (landed, binds
// this symbol) and the erase rebalance 0x0042A34E. Twin of _Rotate_left 0x0042951B (landed).
// Identity (NF42, round 5): STLport 4.6 _tree.c `_Rb_global<_Dummy>::_Rotate_right` (y = x->_M_left; node: colour +0, parent +4,
// left +8, right +0xc); descriptive-by-library (retail is STLport, both PDBs are Dinkumware builds, so no PDB holds the symbol).
// Was spelled `RotateLeft(CMemoryArea*, CMemoryArea**)` on a local node model; now on stlport/fable_map.h (opt-in
// FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE declares the two rotations).
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE
#include "stlport/fable_map.h"
namespace std {
template <class _Dummy>
void __cdecl _Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_left;
    x->_M_left = y->_M_right;
    if (y->_M_right != 0)
        y->_M_right->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
        root = y;
    else if (x == x->_M_parent->_M_right)
        x->_M_parent->_M_right = y;
    else
        x->_M_parent->_M_left = y;
    y->_M_right = x;
    x->_M_parent = y;
}
template void __cdecl _Rb_global<bool>::_Rotate_right(_Rb_tree_node_base*, _Rb_tree_node_base*&);
}
