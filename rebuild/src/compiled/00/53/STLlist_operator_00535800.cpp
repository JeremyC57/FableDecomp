// _STL::list::operator=(const list& x) -- STLport list<NUISystem::CComponent*> assignment: overwrite the common prefix element by element, then erase this list's surplus or insert the rest of x
// Leaf: ??4?$list@PAVCComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00535800, 117 bytes (+ int3 padding), thiscall `ret 4`, *this in eax.
// Identity: callers 0x00531EA5 (the landed NUISystem::CComponent::SetDeletion 0x00531E90, which names this address as
//   list<CComponent*>::operator= of CDeletion::AssociatedParents) and 0x005358F2. The catalog label "std::list::splice"
//   is wrong (no relinking: element-wise assign).
// Callees: list::erase(iterator first, iterator last) @ 0x00534D30 (iterator result through a hidden pointer into the
//   dead `x` slot; catalogued "std::list_node::splice"), list::_M_insert_dispatch(pos, first, last, const __false_type&)
//   @ 0x0042B4C3 (landed; the tag temporary homed in the dead `x` slot).
// Shape: STLport 4.6 operator= verbatim -- `if (this != &x) { first1 = begin(); last1 = end(); first2 = x.begin(); last2
//   = x.end(); while (first1 != last1 && first2 != last2) *first1++ = *first2++; if (first2 == last2) erase(first1,
//   last1); else insert(last1, first2, last2); } return *this;` with the member-template insert inline; the iterators
//   are passed by value through their user copy constructors (built in their argument slots).
namespace NUISystem {
class CComponent;
}

namespace _STL {
struct __false_type { __false_type() { } };
template <class _Tp> struct _Is_integer { typedef __false_type _Integral; };

template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self operator++(int) { _Self __tmp = *this; this->_M_incr(); return __tmp; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 (the sentinel)

    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last,
                            const __false_type&);       // const_iterator instantiation: 0x0042B4C3
    template <class _InputIterator> void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_insert_dispatch(__pos, __first, __last, _Integral());
    }
    iterator erase(iterator __first, iterator __last);   // 0x00534D30
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);
};

template <class _Tp, class _Alloc>
list<_Tp, _Alloc>& list<_Tp, _Alloc>::operator=(const list<_Tp, _Alloc>& __x)
{
    if (this != &__x) {
        iterator __first1 = begin();
        iterator __last1 = end();
        const_iterator __first2 = __x.begin();
        const_iterator __last2 = __x.end();
        while (__first1 != __last1 && __first2 != __last2)
            *__first1++ = *__first2++;
        if (__first2 == __last2)
            erase(__first1, __last1);
        else
            insert(__last1, __first2, __last2);
    }
    return *this;
}

template list<NUISystem::CComponent*>& list<NUISystem::CComponent*>::operator=(const list<NUISystem::CComponent*>&);
}