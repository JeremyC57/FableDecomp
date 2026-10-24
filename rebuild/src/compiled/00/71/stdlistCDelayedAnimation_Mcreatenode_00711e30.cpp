// std::list<CDelayedAnimation>::_M_create_node(const CAnimateThing::CDelayedAnimation& x) -- allocate a list node and copy-construct a delayed animation (its own list of combination anim sets) into it
// Leaf: ?_M_create_node@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711E30, length 82 bytes. __fastcall (`this` unused), `ret 4`, node in eax.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E (the 12-byte node, then the copy's 0x5C-byte sentinel),
//          list<CAnimSet>::_M_insert_dispatch<const_iterator> @ 0x007119C0.
// Identity: the manifest label "AnimSet_CopyArray_ctor" is descriptive. list<CDelayedAnimation> nodes are 12 bytes
//   (links + the 4-byte CDelayedAnimation = list<CAnimSet>), the queue CAnimateThing::DelayedAnimQueue (+0x60); called
//   from the out-of-line push_back sites of that queue (0x00711ED5, 0x00711F28).
// Shape: STLport `_M_create_node`: allocate(1), `_Construct(&p->_M_data, x)` (placement new with its null test) = the
//   implicit CDelayedAnimation copy constructor = the STLport list copy constructor: `_List_base(get_allocator())`
//   (zero store, sentinel built in a local) then `insert(begin(), x.begin(), x.end())` -> `_M_insert_dispatch(pos,
//   first, last, __false_type())`. The iterator's user copy constructor builds pos in the outgoing slot (`push ecx ;
//   mov ecx,esp ; mov [ecx],eax`), the const_iterator copies are plain pushes, and the empty tag lives in the dead `x`
//   argument slot (`lea ebx,[esp+10h]`); the tag types need a user constructor (`__false_type() {}`), else VC7.1 zeroes
//   the tag byte.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

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
    _List_base(const allocator_type&) : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    allocator_type get_allocator() const { return allocator_type(); }
};
template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef allocator<_Tp> allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp>(__a) {}
    list(const list<_Tp>& __x) : _List_base<_Tp>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    iterator begin() { return iterator((_Node*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_Node*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
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
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&);
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
};

class CAnimateThing {
public:
    class CDelayedAnimation {
    public:
        list<CAnimSet> CombinationAnims;                // +0x00
    };
};

template list<CAnimateThing::CDelayedAnimation>::_Node* list<CAnimateThing::CDelayedAnimation>::_M_create_node(const CAnimateThing::CDelayedAnimation&);