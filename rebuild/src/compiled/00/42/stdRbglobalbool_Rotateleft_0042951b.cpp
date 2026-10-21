// std::_Rb_global<bool>::_Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root) -- STLport red-black tree left rotation about x
// Leaf: ?_Rotate_left@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z
// Retail: Fable.exe @ 0x0042951B, 64 bytes, cdecl static (`ret`); callers 0x0042975A, 0x004297C3 (the rebalance after insert),
// 0x0042A486, 0x0042A4E4, 0x0042A553. Catalog label CMemoryAllocatorVariableSize::RotateLeft is wrong: this is the STLport
// _Rb_global<bool>::_Rotate_left template (node: colour +0, parent +4, left +8, right +0xc).
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
namespace std {
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class _Dummy> class _Rb_global {
public:
    static void __cdecl _Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);
};
template <class _Dummy>
void __cdecl _Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_right;
    x->_M_right = y->_M_left;
    if (y->_M_left != 0)
        y->_M_left->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
        root = y;
    else if (x == x->_M_parent->_M_left)
        x->_M_parent->_M_left = y;
    else
        x->_M_parent->_M_right = y;
    y->_M_left = x;
    x->_M_parent = y;
}
template class _Rb_global<bool>;
}