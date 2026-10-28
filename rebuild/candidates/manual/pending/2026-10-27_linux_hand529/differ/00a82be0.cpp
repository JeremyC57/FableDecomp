// std::__adjust_heap(pair<unsigned __int64, list<C2DLineF> >* first, int holeIndex, int len, pair<unsigned __int64, list<C2DLineF> > val, CKeyPairCompareLess comp) -- STLport heap sift-down of the CNavigationLayer switchable line lists (64-bit key), then __push_heap val back up and destroy the by-value list
// Grade: DIFFER(258v258) -- scheduling only in the val copy for __push_heap: retail loads both key dwords before `push comp` and reloads val.second's node right after malloc (before `add esp,4`); ours loads the high dword after the low store and the node after the sentinel stores (implicit pair copy, STLport user pair copy ctor, simplified allocator all give the same)
// Leaf: ??$__adjust_heap@PAU?$pair@_KV?$list@VC2DLineF@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82BE0, 258 bytes, /Gr `__fastcall` (first in ecx, holeIndex in edx; len,
//   the 16-byte value and the empty comparator on the stack, `ret 18h`).
// Identity: std::__adjust_heap<pair<unsigned __int64, list<C2DLineF> >*, int, pair<...>, CKeyPairCompareLess<unsigned
//   __int64, list<C2DLineF> > > (STLport 4.6 _heap.c). Callers: __pop_heap 0x00A83570 (0x00A835E8) and __make_heap 0x00A83680
//   (0x00A83703), both landed for this element; its callee __push_heap 0x00A820E0 is landed under exactly this
//   instantiation's spelling. Element (16 bytes): the UNSIGNED 64-bit key (high dword compared first: `ja` / `jb`, then the
//   low dword `jae`) and the one-word STLport list<C2DLineF> +8 -- Ego_r.pdb CNavigationLayer::SwitchableLineLists value_type.
//   The catalog's "_Adjust_heap<pair<pair<long,long>,CEngineTextureDecal>...>" label is propagated (wrong element).
// Callees: list<C2DLineF>::operator= 0x00517720, list<C2DLineF>::_M_insert_dispatch<const_iterator> 0x005152C0 (the val
//   copy's list copy constructor), malloc 0x00BFEA0E (its sentinel), __push_heap 0x00A820E0, list<C2DLineF>::~list
//   0x005127A0 (val is destroyed by this callee at the end).
// Shape: STLport 4.6 `__adjust_heap` verbatim; the val copy for __push_heap is built in the argument area (STLport's inline
//   list copy constructor), its empty __false_type tag homed on len's dead slot through the same-TU _M_insert_dispatch body.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                        // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return _MaybeReboundAlloc::allocate(__n); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    typedef _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _AllocProxy;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type((const _Node_allocator_type&)_M_node); }
    _List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _AllocProxy _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    ~list();                                            // 0x005127A0
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    template <class _InputIter>                         // 0x005152C0 (same-TU body)
    __declspec(noinline) void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__position, *__first);
    }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 user copy constructor
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00A820E0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

typedef pair<unsigned __int64, list<C2DLineF> > _CSwitchableLineEntry;
typedef CKeyPairCompareLess<unsigned __int64, list<C2DLineF> > _CSwitchableLineLess;
template void __adjust_heap(_CSwitchableLineEntry*, int, int, _CSwitchableLineEntry, _CSwitchableLineLess);
}
