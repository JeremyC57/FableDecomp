// std::list<C2DLineF,std::allocator<C2DLineF> >::erase(iterator first, iterator last) -- STLport range erase: unlink and free every node of [first, last), return last
// Leaf: @U32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00512120, 54 bytes, `ret 0Ch` (thiscall: the hidden iterator result, first, last;
//   `this` is never read -- the node allocator is empty).
// Caller: 0x00517785, list<C2DLineF>::operator= (it assigns the overlapping prefix with four dword copies per node, then
//   erases the surplus here or appends the rest through the range insert 0x005152C0); both iterators are built in the
//   argument area by the iterator's user copy constructor, the result lands in a dead caller slot. The 16-byte POD
//   element has no destructor, so nothing but the unlink and the free remains. Catalog "List_Assign_Destroy" is
//   descriptive.
// Callee: free @ 0x00BFEA14 (the node allocator's deallocate).
// Shape: STLport `while (first != last) erase(first++); return last;` with erase(pos) inlined (next/prev read from
//   the node, prev->_M_next = next, next->_M_prev = prev, deallocate(node, 1)); the iterator result is returned through
//   the hidden pointer.
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class C2DVector {
public:
    float X, Y;
};

class C2DLineF {                                    // Ego_r.pdb, 16 bytes
public:
    C2DVector Point1;                               // +0x00
    C2DVector Point2;                               // +0x08
};

namespace std {

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    allocator() { }
    void deallocate(T* p, unsigned int) { free(p); }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                       // +0
    _List_node_base* _M_prev;                       // +4
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                      // +8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};

template <class T> struct _Nonconst_traits { typedef T& reference; };

template <class T, class Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    _List_iterator operator++(int) { _List_iterator tmp = *this; this->_M_incr(); return tmp; }
};

template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;    // +0
};

template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;

    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        _Destroy(&n->_M_data);
        this->_M_node.deallocate(n, 1);
        return iterator(next_node);
    }
    iterator erase(iterator first, iterator last);
};

template <class T, class A>
typename list<T, A>::iterator list<T, A>::erase(iterator first, iterator last)
{
    while (first != last)
        erase(first++);
    return last;
}

template list<C2DLineF, allocator<C2DLineF> >::iterator list<C2DLineF, allocator<C2DLineF> >::erase(
    list<C2DLineF, allocator<C2DLineF> >::iterator first, list<C2DLineF, allocator<C2DLineF> >::iterator last);

}