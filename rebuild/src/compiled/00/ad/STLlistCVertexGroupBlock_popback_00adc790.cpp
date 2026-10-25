// std::list<CVertexGroupBlock>::pop_back() -- unlink and destroy the last vertex-group block of the list and free its node
// Leaf: ?pop_back@?$list@VCVertexGroupBlock@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADC790, 35 bytes, `__fastcall`, plain `ret`.
// Identity: the element destructor called on node+8 is CVertexGroupBlock::~CVertexGroupBlock @ 0x00A96DA0 (landed
//   callee, chunks 274/310), and the neighbouring landed list<CVertexGroupBlock> push_back 0x00ADCE10 and
//   _M_insert_dispatch 0x00ADCC80 are the same list; the one caller 0x00ADA1BE is in the block optimiser's AddBlocks
//   (0x00AD9F10). The manifest's "list<CEnginePrimitiveHandle>::pop_back" is propagated.
// Callees: CVertexGroupBlock::~CVertexGroupBlock @ 0x00A96DA0, free @ 0x00BFEA14 (the node).
// Shape: STLport 4.6 `pop_back`: `iterator __tmp = end(); erase(--__tmp);` -- the erase inline (unlink, out-of-line
//   element destructor, node freed without a null test), its returned iterator discarded.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14 (import thunk)

class CVertexGroupBlock {                                // retail 24 bytes: CArray<unsigned long> Groups, map GroupToIndexMap
public:
    unsigned long Fields[6];
    ~CVertexGroupBlock();                                // 0x00A96DA0
};

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }   // malloc-backed node allocator: no null test
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 the header node
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first)) erase(__first);
            __first = __next;
        }
    }
};
}

template void std::list<CVertexGroupBlock>::pop_back();