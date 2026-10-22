// _global::FUN_0092b240() -- STLport list iterator pre-increment: step to the next node; returns *this
// Leaf: ??E?$_List_iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B240, 9 bytes, `mov eax,ecx ; mov ecx,[eax] ; mov edx,[ecx] ; mov [eax],edx`.
//   Unreferenced out-of-line copy of the inline `operator++()`; element type unknown.
struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};

struct SListElement_0092b240;                       // the (unidentified) element type

template <class _Tp>
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator& operator++();
};

template <class _Tp>
_List_iterator<_Tp>& _List_iterator<_Tp>::operator++()
{
    _M_node = _M_node->_M_next;
    return *this;
}

template struct _List_iterator<SListElement_0092b240>;