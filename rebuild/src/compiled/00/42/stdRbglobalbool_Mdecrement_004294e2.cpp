// std::_Rb_global<bool>::_M_decrement(_Rb_tree_node_base* node) -- STLport red-black tree predecessor step: the header (red, its parent's parent is itself) steps to the rightmost node, else the rightmost node of the left subtree, else the first ancestor reached from a right child
// Leaf: ?_M_decrement@?$_Rb_global@_N@std@@SAPAU_Rb_tree_node_base@2@PAU32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004294E2, 57 bytes (ends at _Rotate_left 0x0042951B), cdecl static, plain `ret`.
// Opt: size TU (unaligned entry, as the landed _Rb_global<bool>::_Rotate_left 0x0042951B / _Rebalance_for_erase 0x0042A34E).
// The catalog label "RBTree::GetMaxNode" is wrong: this is STLport 4.6 _Rb_global<bool>::_M_decrement
//   (node: colour +0 with red == false, parent +4, left +8, right +0xc).
// Shape: the header test returns `node->_M_right` directly; the left-subtree walk and the ancestor climb share the
//   final `mov eax,ecx ; ret`.
#pragma optimize("s", on)
namespace std {
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (red == false)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
static const bool _S_rb_tree_red = false;
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node);
};
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == _S_rb_tree_red &&
        _M_node->_M_parent->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        _Rb_tree_node_base* __y = _M_node->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        _M_node = __y;
    }
    else {
        _Rb_tree_node_base* __y = _M_node->_M_parent;
        while (_M_node == __y->_M_left) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        _M_node = __y;
    }
    return _M_node;
}
template class _Rb_global<bool>;
}