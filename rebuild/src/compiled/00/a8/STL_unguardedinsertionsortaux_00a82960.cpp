// _STL::__unguarded_insertion_sort_aux(SSwitchableLineList* first, SSwitchableLineList* last, SSwitchableLineList*, CSwitchableLineListLess comp) -- unguarded insertion sort of the 16-byte (unsigned __int64 ID, list of lines) switchable line-list entries: each entry, copied by value (key + a fresh copy of its list), goes through the unguarded insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A82960, 109 bytes, `ret 8` (/Gr: first in ecx, last in edx; the null value-type
//   pointer and the empty comparator's slot on the stack).
// Callees: malloc 0x00BFEA0E (the copy's list sentinel), list<C2DLineF>::_M_insert_dispatch 0x005152C0 (landed),
//   __unguarded_linear_insert 0x00A81E20 (this chunk; it destroys the by-value copy).
// Identity: element = pair<unsigned __int64, list<C2DLineF> > (CNavigationLayer::SwitchableLineLists). Callers: the
//   __final_insertion_sort 0x00A83417 and two more sort drivers. Catalog `_Copy_impl<CNavigationPosition*,...>` is
//   propagated (wrong element and helper).
// Shape: STLport 4.6 verbatim -- `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with
//   the pair's copy ctor inline into the argument area (key words, `_List_base(a)` with the zeroed proxy and the
//   self-linked sentinel, then the out-of-line range insert: `pos` built in its slot through the iterator's user copy
//   ctor, the __false_type temporary homed in the comparator's argument slot -- which needs the range insert's body
//   in the TU: a same-TU noinline stand-in that never touches the tag; declaration-only it gets its own frame slot,
//   110 v 109); the comparator's operator() stays declaration-only (never called here).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }   // placement new (null test on the target)

class C2DVector {
public:
    float X;                                                // +0
    float Y;                                                // +4
};
class C2DLineF {                                            // 16 bytes
public:
    C2DVector Start;                                        // +0x0
    C2DVector End;                                          // +0x8
};

namespace _STL {
class __false_type { public: __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

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
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }   // the copy ctor of `iterator`
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Tp> allocator_type;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _List_base(const allocator_type&) : _M_node(allocator<_Node>(), (_List_node_base*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base();                                          // 0x005127A0 (clear, then free the sentinel)
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef typename _Base::allocator_type allocator_type;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;
    allocator_type get_allocator() const { return allocator_type(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&);   // 0x005152C0
    template <class _InputIter>
    void insert(iterator __pos, _InputIter __first, _InputIter __last) { _M_insert_dispatch(__pos, __first, __last, __false_type()); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
};

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
};

typedef _STL::list<C2DLineF>                                    CLineList;            // 4 bytes (0x18-byte nodes)
typedef _STL::pair<unsigned __int64, CLineList>                 SSwitchableLineList;  // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CLineList>        CSwitchableLineListLess;

#pragma auto_inline(off)
namespace _STL {
// 0x005152C0 (landed): the real range insert as a same-TU noinline body; it never touches the tag.
template <class _Tp, class _Alloc> template <class _InputIter>
__declspec(noinline) void list<_Tp, _Alloc>::_M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
{
    for (; __first._M_node != __last._M_node; __first._M_node = __first._M_node->_M_next) {
        _List_node<_Tp>* __tmp = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        new (&__tmp->_M_data) _Tp(((_List_node<_Tp>*)__first._M_node)->_M_data);
        _List_node_base* __n = __pos._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
}
}
#pragma auto_inline(on)

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A81E20

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux(SSwitchableLineList*, SSwitchableLineList*, SSwitchableLineList*, CSwitchableLineListLess);
}