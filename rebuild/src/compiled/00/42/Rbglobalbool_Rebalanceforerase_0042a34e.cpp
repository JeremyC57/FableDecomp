// _Rb_global<bool>::_Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root, _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost) -- STLport red-black tree erase: unlink z (or splice in its successor), fix leftmost/rightmost, then recolour/rotate to restore the black height; returns the node to free
// Leaf: _Rebalance_for_erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042A34E, length 572 bytes (`ret`, static __cdecl).
// Callees: _Rb_tree_node_base::_S_minimum @ 0x0041C7E0 / _S_maximum @ 0x0041C800 (cdecl, one argument),
//   _Rb_global<bool>::_Rotate_left @ 0x0042951B / _Rotate_right @ 0x0042955B (landed; cdecl (node, root&)).
// Label correction: the catalog's "CMemoryAllocatorVariableSize::Erase" is propagated; the body is STLport's
//   `_Rb_global<bool>::_Rebalance_for_erase` (node: colour byte +0 with red = false, parent +4, left +8, right +0xC), the
//   erase companion of the landed _Rebalance 0x0042971D.
// Shape: STLport 4.x source verbatim in a size TU (`#pragma optimize("s", on)`: ebp frame, `pop ecx` cleanup); x is homed
//   in the dead z argument slot, x_parent lives in esi.
#pragma optimize("s", on)
struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
    static _Rb_tree_node_base* __cdecl _S_minimum(_Rb_tree_node_base* __x);   // 0x0041C7E0
    static _Rb_tree_node_base* __cdecl _S_maximum(_Rb_tree_node_base* __x);   // 0x0041C800
};
static const bool _S_rb_tree_red = false;
static const bool _S_rb_tree_black = true;
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);    // 0x0042951B
    static void __cdecl _Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042955B
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost);
};

template <class _Dummy> _Rb_tree_node_base* __cdecl
_Rb_global<_Dummy>::_Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                         _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost)
{
    _Rb_tree_node_base* __y = __z;
    _Rb_tree_node_base* __x = 0;
    _Rb_tree_node_base* __x_parent = 0;
    if (__y->_M_left == 0)
        __x = __y->_M_right;
    else
        if (__y->_M_right == 0)
            __x = __y->_M_left;
        else {
            __y = __y->_M_right;
            while (__y->_M_left != 0)
                __y = __y->_M_left;
            __x = __y->_M_right;
        }
    if (__y != __z) {
        __z->_M_left->_M_parent = __y;
        __y->_M_left = __z->_M_left;
        if (__y != __z->_M_right) {
            __x_parent = __y->_M_parent;
            if (__x) __x->_M_parent = __y->_M_parent;
            __y->_M_parent->_M_left = __x;
            __y->_M_right = __z->_M_right;
            __z->_M_right->_M_parent = __y;
        }
        else
            __x_parent = __y;
        if (__root == __z)
            __root = __y;
        else if (__z->_M_parent->_M_left == __z)
            __z->_M_parent->_M_left = __y;
        else
            __z->_M_parent->_M_right = __y;
        __y->_M_parent = __z->_M_parent;
        swap(__y->_M_color, __z->_M_color);
        __y = __z;
    }
    else {
        __x_parent = __y->_M_parent;
        if (__x) __x->_M_parent = __y->_M_parent;
        if (__root == __z)
            __root = __x;
        else
            if (__z->_M_parent->_M_left == __z)
                __z->_M_parent->_M_left = __x;
            else
                __z->_M_parent->_M_right = __x;
        if (__leftmost == __z)
            if (__z->_M_right == 0)
                __leftmost = __z->_M_parent;
            else
                __leftmost = _Rb_tree_node_base::_S_minimum(__x);
        if (__rightmost == __z)
            if (__z->_M_left == 0)
                __rightmost = __z->_M_parent;
            else
                __rightmost = _Rb_tree_node_base::_S_maximum(__x);
    }
    if (__y->_M_color != _S_rb_tree_red) {
        while (__x != __root && (__x == 0 || __x->_M_color == _S_rb_tree_black))
            if (__x == __x_parent->_M_left) {
                _Rb_tree_node_base* __w = __x_parent->_M_right;
                if (__w->_M_color == _S_rb_tree_red) {
                    __w->_M_color = _S_rb_tree_black;
                    __x_parent->_M_color = _S_rb_tree_red;
                    _Rotate_left(__x_parent, __root);
                    __w = __x_parent->_M_right;
                }
                if ((__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black) &&
                    (__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black)) {
                    __w->_M_color = _S_rb_tree_red;
                    __x = __x_parent;
                    __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black) {
                        if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
                        __w->_M_color = _S_rb_tree_red;
                        _Rotate_right(__w, __root);
                        __w = __x_parent->_M_right;
                    }
                    __w->_M_color = __x_parent->_M_color;
                    __x_parent->_M_color = _S_rb_tree_black;
                    if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
                    _Rotate_left(__x_parent, __root);
                    break;
                }
            } else {
                _Rb_tree_node_base* __w = __x_parent->_M_left;
                if (__w->_M_color == _S_rb_tree_red) {
                    __w->_M_color = _S_rb_tree_black;
                    __x_parent->_M_color = _S_rb_tree_red;
                    _Rotate_right(__x_parent, __root);
                    __w = __x_parent->_M_left;
                }
                if ((__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black) &&
                    (__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black)) {
                    __w->_M_color = _S_rb_tree_red;
                    __x = __x_parent;
                    __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black) {
                        if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
                        __w->_M_color = _S_rb_tree_red;
                        _Rotate_left(__w, __root);
                        __w = __x_parent->_M_left;
                    }
                    __w->_M_color = __x_parent->_M_color;
                    __x_parent->_M_color = _S_rb_tree_black;
                    if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
                    _Rotate_right(__x_parent, __root);
                    break;
                }
            }
        if (__x) __x->_M_color = _S_rb_tree_black;
    }
    return __y;
}
template struct _Rb_global<bool>;