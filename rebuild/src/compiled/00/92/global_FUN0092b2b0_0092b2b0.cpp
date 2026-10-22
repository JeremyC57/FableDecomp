// _global::FUN_0092b2b0() -- STLport list size(): count the nodes from begin() to end()
// Leaf: size
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B2B0, 24 bytes, `ret`: the sentinel into edx, its next node into ecx, then
//   the counting walk (`mov ecx,[ecx] ; inc eax ; cmp ecx,edx ; jne`, loop head aligned with `lea ebx,[ebx]`).
//   Unreferenced out-of-line copy of `size() const { return distance(begin(), end()); }`.
struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};

struct SListElement_0092b2b0;                       // the (unidentified) element type

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
inline long __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    long __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}

template <class _Tp>
struct list {
    _List_node_base* _M_node;                    // +0x00 the sentinel
    _List_iterator<_Tp> begin() const { return _List_iterator<_Tp>(_M_node->_M_next); }
    _List_iterator<_Tp> end() const { return _List_iterator<_Tp>(_M_node); }
    long size() const;
};

template <class _Tp>
long list<_Tp>::size() const
{
    return __distance(begin(), end(), input_iterator_tag());
}

template struct list<SListElement_0092b2b0>;