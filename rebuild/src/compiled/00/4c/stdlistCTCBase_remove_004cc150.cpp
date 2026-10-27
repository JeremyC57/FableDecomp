// std::list<CTCBase*>::remove(CTCBase* const& value) -- erase every node holding `value`
// Leaf: ?remove@?$list@PAVCTCBase@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CC150, 54 bytes, thiscall, `ret 4`.
// Identity (probable): callers CThing::WorldUpdate 0x004C8050, CThing::AddTC 0x004C9D60 and CTCBase::RemoveFromGlobalTCs 0x00686DF0 (thing-component lists); every stand-in binding (?remove@CTCList@@, ?remove@?$CRetailList@PAVCTCBase@@@@, ?Remove@GContainer@@) names CTCBase*.
// Callee: free 0x00BFEA14 (the node; the 4-byte element has a trivial destructor).
// Shape (family K116, representative 0x004CC150): the K114 / landed list<CCountedPointer<..>>::remove 0x004BA390 with a 4-byte element:
//   `value` re-read through its reference every trip, the unlink, the node freed.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
class CTCBase;
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
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
        return iterator(__next_node);
    }
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first)
                erase(__first);
            __first = __next;
        }
    }
};
}


template void std::list<CTCBase* >::remove(CTCBase* const&);