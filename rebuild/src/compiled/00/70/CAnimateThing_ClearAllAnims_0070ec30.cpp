// CAnimateThing::ClearAllAnims() -- clear the main anim set, the transition and combination anim lists and the delayed-animation queue, refill the queue with three empty delayed animations and clear the current delayed animation
// Leaf: ?ClearAllAnims@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070EC30, length 224 bytes. __fastcall, plain `ret`. Ego_r.pdb:
//         `?ClearAllAnims@CAnimateThing@@QAEXXZ` (Ego_r 0x00578160).
// Callees: CAnimSet::Clear @ 0x00711520 (landed as "CNavigatorOwner_00711520::ResetBuffers"), list<CAnimSet>::clear @
//          0x0070B8A0, list<CDelayedAnimation>::clear @ 0x0070BA00, malloc / free (CRT import thunks) @ 0x00BFEA0E /
//          0x00BFEA14, list<CAnimSet>::_M_insert_dispatch<const_iterator> @ 0x007119C0.
// Identity: the manifest label "Navigator_UpdateBuildingTreeUI" is propagated and wrong. Retail CAnimateThing: CurrentAnim
//   (CCompleteAnimation 0x60: MainAnimSet 0x54, TransitionAnimSets +0x54, CombinationAnims +0x58), DelayedAnimQueue +0x60
//   (list<CDelayedAnimation>), DelayedAnim +0x64 -- Ego_r.pdb offsets minus 8 per 16-byte container.
// Shape: STLport containers inlined: size() is the distance loop (head read once), push_back(CDelayedAnimation()) is
//   `insert(end(), x)` -- end() read before the node malloc, the CDelayedAnimation copy = list copy constructor (the
//   _M_create_node 0x00711E30 body) with the tag byte at [esp+13h], the link through n / prev locals, then the temporary's
//   ~_List_base: clear() and the sentinel free, unchecked because the sentinel was dereferenced. The two clear bodies are
//   same-TU stand-ins under auto_inline(off): with declarations only VC7.1 keeps the temporary in memory (its zero store,
//   a reload after clear() and the null test before free).
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
    ~CAnimSet();
    void Clear();                                       // 0x00711520
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
    void clear();                                       // list<CAnimSet> 0x0070B8A0, list<CDelayedAnimation> 0x0070BA00
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
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
    protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    public:
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
    unsigned int size() const
    {
        unsigned int __n = 0;
        const_iterator __last = end();
        for (const_iterator __it = begin(); __it != __last; ++__it)
            ++__n;
        return __n;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&);
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
};

// Same-TU stand-in body for the two out-of-line clears (0x0070B8A0 / 0x0070BA00 sit in this TU): VC7.1 uses what it
// learns from it.
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void _List_base<_Tp>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node._M_data->_M_next;
    while (__cur != _M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        __tmp->_M_data.~_Tp();
        _M_node.deallocate(__tmp, 1);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
}
#pragma auto_inline(on)

class CAnimateThing {
public:
    class CDelayedAnimation {
    public:
        list<CAnimSet> CombinationAnims;                // +0x00
    };
    class CCompleteAnimation {
    public:
        CAnimSet MainAnimSet;                           // +0x00
        list<CAnimSet> TransitionAnimSets;              // +0x54
        list<CAnimSet> CombinationAnims;                // +0x58
        bool MainAnimCanTransitionOut;                  // +0x5C
    };
    CCompleteAnimation CurrentAnim;                     // +0x00
    list<CDelayedAnimation> DelayedAnimQueue;           // +0x60
    CDelayedAnimation DelayedAnim;                      // +0x64
    void ClearAllAnims();
};

void CAnimateThing::ClearAllAnims()
{
    CurrentAnim.MainAnimSet.Clear();
    CurrentAnim.TransitionAnimSets.clear();
    CurrentAnim.CombinationAnims.clear();
    DelayedAnimQueue.clear();
    while (DelayedAnimQueue.size() != 3)
        DelayedAnimQueue.push_back(CDelayedAnimation());
    DelayedAnim.CombinationAnims.clear();
}