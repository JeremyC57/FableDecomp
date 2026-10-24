// std::list<CAnimSet>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- insert a copy of every animation set in [first, last) before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007119C0, length 74 bytes (+ int3 padding). __fastcall (`this` unused),
//         `ret 10h`: pos, first, last by value, then the tag reference.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E (the 0x5C-byte node), CAnimSet::CAnimSet(const CAnimSet&) @ 0x00710B80.
// Identity: the manifest label "AnimSet_CopyIntoList" is descriptive. The STLport 4.x range-insert worker of list<CAnimSet>
//   for const_iterator ranges: called from the inlined list<CAnimSet> copy constructor in ClearAllAnims 0x0070EC30, in
//   list<CDelayedAnimation>::_M_create_node 0x00711E30 and five more CAnimateThing-TU sites (Ego_r.pdb names the
//   Dinkumware twin `list<CAnimSet>::_Insert<_List_const_iterator>`).
// Shape: `for (; first != last; ++first) insert(pos, *first);` with STLport insert inlined: _M_create_node's placement
//   copy (null test on &node->_M_data), then the link through locals n / prev (next, prev, prev->next, n->prev) -- with
//   the members re-read through the iterator VC7.1 reloads pos->prev after the first store.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type {};
struct __false_type {};

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

class CAnimSet {
public:
    unsigned char _body[0x54];                          // retail CAnimSet (Ego_r.pdb 0x5C minus 8)
    CAnimSet(const CAnimSet& other);                    // 0x00710B80
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { _M_incr(); return *this; }
};

template <class _Tp> class _List_base {
public:
    typedef allocator<_Tp> allocator_type;
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
};
template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__position, *__first);
    }
};

template void list<CAnimSet>::_M_insert_dispatch<list<CAnimSet>::const_iterator>(list<CAnimSet>::iterator,
    list<CAnimSet>::const_iterator, list<CAnimSet>::const_iterator, const __false_type&);