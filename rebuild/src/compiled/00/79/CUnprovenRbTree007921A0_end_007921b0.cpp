// CUnprovenRbTree_007921A0::end() -- return an iterator on the red-black tree's header node (the past-the-end position)
// Leaf: ?end@CUnprovenRbTree_007921A0@@QAE?AUiterator@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007921B0, 11 bytes, thiscall `ret 4` (iterator returned through the hidden result pointer):
//   mov ecx,[ecx] ; mov eax,[esp+4] ; mov [eax],ecx.
// Identity (descriptive, unproven): STLport 4.6 _Rb_tree::end() (`iterator(this->_M_header._M_data)`), the member that follows
//   begin() in _tree.h, here directly after the landed CUnprovenRbTree_007921A0::begin 0x007921A0 (header->_M_left) and before
//   the tree erase 0x007921C0 of the CTCDivineWrath TU. No call, jump or absolute reference to 0x007921B0 exists, so no caller
//   fixes the key/value types; the class is the landed begin row's placeholder (same tree, same TU), extended by this member.
struct CUnprovenRbTreeNode_007921A0 {                  // STLport _Rb_tree_node_base layout
    bool _M_color;
    CUnprovenRbTreeNode_007921A0* _M_parent;
    CUnprovenRbTreeNode_007921A0* _M_left;
    CUnprovenRbTreeNode_007921A0* _M_right;
};
class CUnprovenRbTree_007921A0 {
public:
    struct iterator {
        CUnprovenRbTreeNode_007921A0* _M_node;
        iterator(CUnprovenRbTreeNode_007921A0* node) : _M_node(node) { }
    };
    CUnprovenRbTreeNode_007921A0* _M_header;           // _M_header._M_data: the header node
    iterator begin();
    iterator end();
};
CUnprovenRbTree_007921A0::iterator CUnprovenRbTree_007921A0::end()
{
    return iterator(_M_header);
}