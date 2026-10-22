// _global::FUN_0092b290(const _List_iterator&, const _List_iterator&, const input_iterator_tag&) -- STLport __distance for list iterators: count the steps from first to last
// Leaf: __distance
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B290, 26 bytes, cdecl `ret`: both iterators by reference
//   (`mov ecx,[esp+4] ; mov edx,[esp+8] ; mov ecx,[ecx] ; mov edx,[edx]`), then the counting walk. Unreferenced
//   out-of-line instantiation; its exact twin 0x0092B270 is another element type's instantiation.
struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};

struct SListElement_0092b290;                       // the (unidentified) element type

template <class _Tp>
struct _List_iterator {                          // STLport list iterator: one node pointer
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) { }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};

struct input_iterator_tag { };

template <class _InputIterator>
long __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    long __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}

template long __distance<_List_iterator<SListElement_0092b290> >(const _List_iterator<SListElement_0092b290>&, const _List_iterator<SListElement_0092b290>&, const input_iterator_tag&);