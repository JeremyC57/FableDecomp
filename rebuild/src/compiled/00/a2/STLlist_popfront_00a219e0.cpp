// _STL::list::pop_front() -- STLport list<long>: unlink the first node and free it
// Leaf: ?pop_front@?$list@J
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A219E0, 22 bytes, __fastcall, plain `ret`.
// Identity: STLport 4.6 `void pop_front() { erase(begin()); }` with the inline single-node erase (unlink, trivial
//   _Destroy, deallocate = free). Only caller 0x00A1FA4F. The catalog's "list<long>::pop_front" spelling is kept; the
//   element type cannot be told from the bytes (any trivially destructible 4-byte payload compiles the same).
// Callee: free @ 0x00BFEA14 (STLport's malloc allocator; the dropped result is cleaned with `pop ecx`).
// Shape: the node's next/prev are loaded together, then the two link stores bracket the `push node`.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
inline void _Destroy(long*) {}

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator(__next_node);
    }
    void pop_front() { erase(begin()); }
};

template void list<long>::pop_front();

} // namespace _STL