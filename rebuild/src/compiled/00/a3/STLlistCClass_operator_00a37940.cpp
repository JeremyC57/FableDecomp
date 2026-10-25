// std::list<CClass*>::operator=(const list& x) -- STLport list assignment: overwrite the common prefix element by element, then erase this list's surplus or append the rest of x
// Leaf: ??4?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37940, 117 bytes (+ int3 padding), `ret 4`.
// Identity: the 4-byte-element list of the CClassFactory module (12-byte nodes, value at +8); callees are this list's
//   out-of-line erase(first, last) 0x00A37720 (unlink + free, iterator result through a hidden pointer) and
//   _M_insert_dispatch(pos, first, last, const __false_type&) 0x00A37800. The catalog's "List_CopyOrMoveNodes" is
//   descriptive. Used by the CClassTypeInfo assignment (the TypeList member of the vector<CClassTypeInfo> element).
// Shape: STLport 4.6 `if (this != &x) { first1 = begin(); last1 = end(); first2 = x.begin(); last2 = x.end();
//   while (first1 != last1 && first2 != last2) *first1++ = *first2++; if (first2 == last2) erase(first1, last1);
//   else insert(last1, first2, last2); } return *this;` -- STLport's `_List_iterator(const iterator&)` is the non-const
//   iterator's user copy constructor (the const iterator's copy stays implicit), so the by-value `iterator` arguments are
//   built in their argument slots (`push ecx ; mov ecx,esp ; mov [ecx],..`) while the const iterators are pushed; erase's hidden
//   result and the __false_type tag (user-constructed, so not zero-filled) are homed in the dead `x` argument slot.
class CClass;

namespace std {
struct __false_type { __false_type() { } };
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                           // +0x08
};
template <class T> struct _Nonconst_traits { typedef T& reference; };
template <class T> struct _Const_traits { typedef const T& reference; };
template <class T, class Traits> struct _List_iterator {
    typedef _List_iterator<T, Traits> _Self;
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_node_base* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const iterator& x) : _M_node(x._M_node) { }      // the non-const iterator's user copy ctor
    typename Traits::reference operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _Self& operator++() { _M_node = _M_node->_M_next; return *this; }
    _Self operator++(int) { _Self tmp = *this; _M_node = _M_node->_M_next; return tmp; }
    bool operator==(const _List_iterator& x) const { return _M_node == x._M_node; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> >    const_iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
    iterator erase(iterator first, iterator last);                         // 0x00A37720
    template <class InputIter>
    void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&);   // 0x00A37800
    template <class InputIter>
    void insert(iterator pos, InputIter first, InputIter last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list& operator=(const list& x);
};

template <class T>
list<T>& list<T>::operator=(const list<T>& x)
{
    if (this != &x) {
        iterator first1 = begin();
        iterator last1 = end();
        const_iterator first2 = x.begin();
        const_iterator last2 = x.end();
        while (first1 != last1 && first2 != last2)
            *first1++ = *first2++;
        if (first2 == last2)
            erase(first1, last1);
        else
            insert(last1, first2, last2);
    }
    return *this;
}
}
template class std::list<CClass*>;