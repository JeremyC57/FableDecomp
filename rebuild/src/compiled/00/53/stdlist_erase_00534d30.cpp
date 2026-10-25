// std::list::erase(iterator first, iterator last) -- (list<NUISystem::CComponent*>) STLport range erase: unlink and free every node of [first, last), return last
// Leaf: ?erase@?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@_STL@@@_STL@@QAE?AU?$_List_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@_STL@@@2@U32@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00534D30, 54 bytes, thiscall with `this` unused, `ret 0Ch` (hidden
//   iterator result, first, last); returns the result pointer.
// Identity: the manifest's "std::list_node::splice" is propagated. The only caller is list<CComponent*>::operator=
//   0x00535800 (the erase-the-surplus arm), which NUISystem::CComponent::SetDeletion 0x00531E90 (landed) uses for
//   CDeletion::AssociatedParents.
// Callees: free 0x00BFEA14.
// Shape: STLport 4.6 `while (first != last) erase(first++); return last;` with erase(iterator) inline (`prev->next =
//   next; next->prev = prev; _Destroy(&n->_M_data); deallocate(n)`); the element is a pointer, so _Destroy is empty.
//   The iterator has a user copy constructor (the `_Nonconst_traits` iterator), so the result goes through the
//   hidden pointer.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace NUISystem { class CComponent; }

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { }

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

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator((_Node*)__next_node);
    }
    iterator erase(iterator __first, iterator __last);
};

template <class _Tp, class _Alloc>
typename list<_Tp, _Alloc>::iterator list<_Tp, _Alloc>::erase(iterator __first, iterator __last)
{
    while (__first != __last)
        erase(__first++);
    return __last;
}

template list<NUISystem::CComponent*>::iterator list<NUISystem::CComponent*>::erase(iterator, iterator);

} // namespace _STL