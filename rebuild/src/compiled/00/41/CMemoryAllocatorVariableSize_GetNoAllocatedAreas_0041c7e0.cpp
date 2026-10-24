// Grade: DIFFER(26v23) -- catalog oracle truncated (23 of 26 bytes); this source is a MATCH against the full 26-byte retail body read from Fable.exe (qx.py). Re-bound the catalog row to 26 bytes, then land.
// std::_Rb_tree_node_base::_S_minimum(_Rb_tree_node_base*) -- leftmost node of an STLport red-black subtree
// Leaf: _S_minimum
// Retail: Fable.exe @ 0x0041C7E0, 26 bytes, static (__cdecl, node at [esp+4]). STLport 4.6 node base is
// {_M_color +0, _M_parent +4, _M_left +8, _M_right +0xc}; the loop follows _M_left (+8) until it is null.
// Callers 0x00429366 (tail jmp), 0x0042A424, 0x0042CEBE, 0x0053CBE9 are tree begin/erase helpers. The catalog label
// CMemoryAllocatorVariableSize::GetNoAllocatedAreas is a propagated name for this byte-identical body.
// Catalog row: only the first 23 bytes (stops before `jne` / `ret`); this source equals the full 26-byte retail body.
namespace std {
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;     // +0x00
    _Base_ptr _M_parent;    // +0x04
    _Base_ptr _M_left;      // +0x08
    _Base_ptr _M_right;     // +0x0c
    static _Base_ptr _S_minimum(_Base_ptr x);
};
_Rb_tree_node_base::_Base_ptr _Rb_tree_node_base::_S_minimum(_Base_ptr x)
{
    while (x->_M_left != 0)
        x = x->_M_left;
    return x;
}
}
