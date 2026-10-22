// _global::FUN_0092b230(_List_node_base*) -- STLport list iterator constructor from a node pointer; returns this
// Leaf: ??0?$_List_iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B230, 11 bytes, `mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; ret 4`.
//   Unreferenced out-of-line copy of the inline `_List_iterator(_Node* x) : _M_node(x) {}` (a run of list
//   iterator/size helpers 0x0092B230..0x0092B2B0 follows CAIStateGroup_ApprenticeHero::Update); element type unknown.
struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};

struct SListElement_0092b230;                       // the (unidentified) element type

template <class _Tp>
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* node);
};

template <class _Tp>
_List_iterator<_Tp>::_List_iterator(_List_node_base* node)
    : _M_node(node)
{
}

template struct _List_iterator<SListElement_0092b230>;