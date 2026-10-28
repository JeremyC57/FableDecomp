// std::__introsort_loop(pair<unsigned __int64, list<C2DLineF>>* first, pair<...>* last, pair<...>*, int depth_limit, CKeyPairCompareLess<unsigned __int64, list<C2DLineF>> comp) -- STLport introsort recursion over the 16-byte switchable line-list entries (unsigned 64-bit key order): partition around the median of three, recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85050, 309 bytes, /Gr `__fastcall` (first ecx, last edx; the value-type pointer,
//   the depth limit and the empty comparator's slot on the stack), `ret 0Ch`.
// Identity (proven by its callees): __unguarded_partition 0x00A83CC0 and __partial_sort 0x00A84A60 are the landed
//   (unsigned __int64 key, list<C2DLineF>) instances of the CNavQuadTree TU (CNavigationLayer::SwitchableLineLists,
//   CVectorMap<unsigned __int64, list<C2DLineF>, CKeyPairCompareLess<...> > in Ego_r.pdb). The landed sort 0x00A853A0
//   binds this address under an invented global `IntrosortLoop_00a85050(SElement16*, ...)`.
// Callees: list<C2DLineF>::_M_insert_dispatch 0x005152C0 (the pivot's inline list copy), __unguarded_partition 0x00A83CC0
//   (by-value 16-byte pivot + comparator), __partial_sort 0x00A84A60, malloc 0x00BFEA0E (the pivot list's sentinel).
// Shape: STLport verbatim with __median inline (unsigned 64-bit key compares: high words, then low words); the pivot is
//   copied into the partition's argument area by pair's explicit copy (key words, then the inline list copy ctor whose
//   non-const iterator is built in its own argument slot). _M_insert_dispatch is a same-TU noinline stand-in that never
//   reads its tag, so VC homes the tag on the live depth-limit slot as retail does.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector { public: float X, Y; };
class C2DLineF {                                           // 16 bytes (Ego_r.pdb)
public:
    C2DVector Start;                                       // +0x0
    C2DVector End;                                         // +0x8
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
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
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    template <class _InputIter>                            // 0x005152C0 (out of line)
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&);
};
// Same-TU stand-in for 0x005152C0 that never reads its tag (VC then homes the tag on the live atend slot, as retail).
template <class _Tp, class _Alloc> template <class _InputIter>
__declspec(noinline) void list<_Tp, _Alloc>::_M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
{
    if (__first._M_node != __last._M_node)
        __position._M_node->_M_prev = __first._M_node;
}

template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x08
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _Tp, class _Compare>
__forceinline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x00A83CC0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A84A60
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type_of(__first), __comp);
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, std::list<C2DLineF> > TSwitchableLineList;
template void __fastcall std::__introsort_loop(TSwitchableLineList*, TSwitchableLineList*, TSwitchableLineList*, int, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> >);