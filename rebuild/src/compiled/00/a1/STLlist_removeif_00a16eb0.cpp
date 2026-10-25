// _STL::list::remove_if(CIsNavigator pred) -- STLport list<pair<long, const CNavigatorBase*> >: erase every entry whose navigator is the predicate's
// Leaf: ??$remove_if@VCIsNavigator@@@?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A16EB0, 52 bytes (+ int3 padding), __fastcall `ret 4` (the 4-byte predicate by value).
// Identity: STLport 4.6 member-template list::remove_if with the inline single-node erase (trivial payload: unlink + free).
//   Node payload at +8: pair<long, const CNavigatorBase*> (first +8, second +0xC). Ego_r.pdb CIsNavigator
//   { CNavigatorBase* Navigator; }.
// Callee: free @ 0x00BFEA14.
// Shape: the predicate is loaded once into ebx (edi = end, eax = current, esi = next); `entry.second == Navigator`.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CNavigatorBase;

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
};
}

class CIsNavigator {
public:
    const CNavigatorBase* Navigator;                    // +0x00
    bool operator()(const _STL::pair<long, const CNavigatorBase*>& entry) const { return entry.second == Navigator; }
};

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
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
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
    iterator end() { return iterator(this->_M_node._M_data); }

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

    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};

template void list<pair<long, const CNavigatorBase*> >::remove_if<CIsNavigator>(CIsNavigator);

} // namespace _STL