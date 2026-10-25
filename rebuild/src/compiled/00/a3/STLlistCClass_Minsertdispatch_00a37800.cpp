// std::list<CClass*>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- insert a copy of each element of [first, last) before pos (STLport range insert, node creation inlined)
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37800, 66 bytes (+ int3 padding), `ret 10h` (pos, first, last, the tag;
//   `this` is never read: the inlined insert only links nodes around pos).
// Identity: called from list<CClass*>::operator= 0x00A37940 (the range-insert tail) and from the CClassTypeInfo copy
//   constructor 0x00A37B50 (`insert(begin(), x.begin(), x.end())`). The speed-TU twin of the landed size-TU body
//   0x0042B4C3 (which calls insert out of line). The catalog's "List_TransferNodes" is descriptive.
// Callee: malloc @ 0x00BFEA0E (12-byte node: next, prev, CClass*).
// Shape: STLport 4.6 `for (; first != last; ++first) insert(pos, *first);` with insert inlined (malloc, null-checked
//   placement copy of the value, prev read into a local before `tmp->next = pos`); the loop is rotated (entry test, then
//   a bottom `jne`), pos lives in edi, first in esi, last in ebx.
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }

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
    _List_node_base* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
    template <class Tr2> _List_iterator(const _List_iterator<T, Tr2>& x) : _M_node(x._M_node) { }
    typename Traits::reference operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> >    const_iterator;
    _List_node_base* _M_node;
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* p = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node_base* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    template <class InputIter>
    void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&)
    {
        for (; first != last; ++first)
            insert(pos, *first);
    }
};
}
template void std::list<CClass*>::_M_insert_dispatch<std::list<CClass*>::const_iterator>(std::list<CClass*>::iterator,
    std::list<CClass*>::const_iterator, std::list<CClass*>::const_iterator, const std::__false_type&);