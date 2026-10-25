// std::list<CReactionMatch*>::operator=(const list& x) -- STLport list assignment: overwrite the common prefix element-wise, then erase this list's surplus or insert x's remaining tail
// Leaf: ??4?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067CF40, 112 bytes, thiscall, `ret 4`, returns this.
// Identity: the manifest's "std::list::merge" is propagated. The only caller is the landed
//   COpinionReactionManagerDef::operator= 0x0067CE50 (its Matches list at +0xA8, Ego_r.pdb list<CReactionMatch*>).
// Callees: list::erase(iterator, iterator) 0x0067CFB0, list::insert(iterator, const_iterator, const_iterator)
//   0x0067CFF0 (landed), both out of line.
// Shape: STLport 4.6 operator= verbatim. The mutable iterator's `_List_iterator(const iterator&)` is its USER copy
//   constructor, so every by-value `iterator` argument is built in its argument slot (`push ecx ; mov ecx,esp ;
//   mov [ecx],edx`), while for the const_iterator the same constructor is only a converting one: its copy is
//   implicit and the two const_iterators are pushed directly (`push ebp ; push ecx`). erase's iterator result is
//   homed in the dead `x` argument slot. The element is a pointer, so `*first1++ = *first2++` is one dword move.
namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

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
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;

    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
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
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    iterator erase(iterator __first, iterator __last);                          // 0x0067CFB0
    void insert(iterator __pos, const_iterator __first, const_iterator __last);  // 0x0067CFF0
    list& operator=(const list& __x);
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

} // namespace std

class CReactionMatch;

template std::list<CReactionMatch*>& std::list<CReactionMatch*>::operator=(const std::list<CReactionMatch*>&);