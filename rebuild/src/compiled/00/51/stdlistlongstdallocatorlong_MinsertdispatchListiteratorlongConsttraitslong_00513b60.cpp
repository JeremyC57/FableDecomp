// std::list<long,std::allocator<long> >::_M_insert_dispatch<_List_iterator<long,_Const_traits<long> > >(iterator position, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert: insert a copy of every element of [first, last) before position
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513B60, 66 bytes, `ret 10h` (thiscall; `this` is never read -- the node
//   allocator is empty).
// Callers: 0x00514E18 (list::insert(pos, first, last): the tag temporary homed in its dead `last` slot, `pos` built in
//   the argument area by the iterator's user copy constructor, the const iterators pushed as plain words), 0x00515E83
//   (the list copy constructor `insert(begin(), x.begin(), x.end())`), 0x005175CC, 0x00517EDF, 0x0051C00D. One COMDAT
//   for every list of 4-byte elements (ICF): modelled as list<long>. Catalog label "std_list_link_nodes" is descriptive.
// Callee: malloc @ 0x00BFEA0E (node allocate(1) = malloc(0x0C)).
// Shape: STLport 4.5 `for (; first != last; ++first) insert(position, *first);` with insert(pos, x) and
//   _M_create_node inlined: the placement null test on &node->_M_data, then `prev = pos->_M_prev` read before
//   `node->_M_next = pos` (GOTCHAS 24b), ++first follows _M_next.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
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
template <class T> struct _Const_traits { typedef const T& reference; };

template <class T, class Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_node<T> _Node;
    _List_iterator(_Node* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    typename Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
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
    typedef _List_iterator<T, _Const_traits<T> > const_iterator;

    protected:
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    public:
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    template <class InputIter>
    void _M_insert_dispatch(iterator position, InputIter first, InputIter last, const __false_type&)
    {
        for (; first != last; ++first)
            insert(position, *first);
    }
};

template void list<long, allocator<long> >::_M_insert_dispatch<list<long, allocator<long> >::const_iterator>(
    list<long, allocator<long> >::iterator position, list<long, allocator<long> >::const_iterator first,
    list<long, allocator<long> >::const_iterator last, const __false_type&);

}