// _global::FUN_0092b250() -- STLport list begin(): an iterator on the sentinel's next node, returned through the hidden result pointer
// Leaf: begin
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B250, 13 bytes, `ret 4` (the hidden result): `mov eax,[ecx] ; mov ecx,[eax] ;
//   mov eax,[esp+4] ; mov [eax],ecx`. Unreferenced out-of-line copy (exact twin of the landed
//   list<NUISystem::CList*>::begin 0x00429A85); the iterator has a user constructor, so it is returned in memory.
struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};

struct SListElement_0092b250;                       // the (unidentified) element type

template <class _Tp>
struct _List_iterator {                          // STLport list iterator: one node pointer
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) { }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};

struct input_iterator_tag { };

template <class _Tp>
struct list {
    _List_node_base* _M_node;                    // +0x00 the sentinel
    _List_iterator<_Tp> begin();
};

template <class _Tp>
_List_iterator<_Tp> list<_Tp>::begin()
{
    return _List_iterator<_Tp>(_M_node->_M_next);
}

template struct list<SListElement_0092b250>;
