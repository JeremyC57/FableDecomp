// CUnprovenRbTree_007921A0::begin() -- return an iterator on the leftmost node of a red-black tree (the header node's _M_left)
// Leaf: ?begin@CUnprovenRbTree_007921A0@@QAE?AUiterator@1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007921A0, 14 bytes, thiscall `ret 4` (iterator returned through the hidden result pointer).
// Identity (descriptive, unproven): byte twin of the landed 0x00CD2790 (catalogued `operator--`, landed as a list-iterator accessor).
//   The bytes are `header = this->[0]; result = header->[+8]`: STLport 4.6 _Rb_tree::begin() (`iterator(_M_leftmost())`, the header
//   node's _M_left at +8 after _M_color / _M_parent), returned by value as every member function returns a class in VC7.1.
//   Search: no call, jmp or absolute reference to 0x007921A0 anywhere in Fable.exe (rel32 scan of .text + dword scan of the image),
//   so no caller fixes the tree's key/value types; the neighbours (0x007922A0 _Rb_tree_base with a 0x2C node, 0x00792300
//   _Rb_tree<CThingCreatureBase*, pair<..., CTCDivineWrath::PerCreatureInfo>>::_M_erase) put it in the CTCDivineWrath TU, whose
//   FableWin.pdb copy holds an out-of-line map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>::begin (Dinkumware debug
//   build; not provable for retail). Catalog label `_gapscan sub_007921a0`.
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
};
CUnprovenRbTree_007921A0::iterator CUnprovenRbTree_007921A0::begin()
{
    return iterator(_M_header->_M_left);
}