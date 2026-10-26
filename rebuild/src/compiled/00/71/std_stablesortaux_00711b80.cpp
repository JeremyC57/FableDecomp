// std::__stable_sort_aux(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* last, list<CAnimSet>::const_iterator*, int*, CAnimSetItPriorityLessThan comp) -- stable sort through a temporary buffer, in place when none can be allocated
// Leaf: ??$__stable_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711B80, length 133 bytes (+ int3 padding). __fastcall (first ecx, last
//         edx), `ret 0Ch`.
// Callees: _Temporary_buffer::_M_allocate_buffer @ 0x0070F410, std::__inplace_stable_sort<...> @ 0x00711640,
//          std::__stable_sort_adaptive<...> @ 0x00711920, free (CRT import thunk) @ 0x00BFEA14.
// Identity: the manifest label "CAnimSet_SortAndOptimize" is descriptive. The out-of-line body of the inline
//   stable_sort(first, last, CAnimSetItPriorityLessThan()); callers 0x0070DFAE, 0x0070E173, 0x00711C99 sort a vector of
//   list<CAnimSet>::const_iterator by priority.
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `__stable_sort_aux` with the _Temporary_buffer constructor (distance, _M_allocate_buffer, then
//   `uninitialized_fill_n(_M_buffer, _M_len, *__first)` -- the __false_type placement loop with its null test, because
//   the iterator is not POD) and destructor (trivial _Destroy, free) inlined; VC7.1 duplicates the free tail into both
//   branches. The three callees are same-TU stand-in bodies under auto_inline(off) (the STLport text of 0x0070F410,
//   0x00711640, 0x00711920): with declarations only VC7.1 re-reads _M_len / _M_buffer from the frame after every call
//   (127 v 133 bytes).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CAnimSet {                                        // retail layout (Ego_r.pdb minus 8: 12-byte vectors)
public:
    unsigned char _before[0x4C];                        // UpdateMode, Anims, InternalAlphas, ... AnimDelay
    long Priority;                                      // +0x4C
    unsigned char _after[0x08];                         // Type, the flag bits
};

namespace std {
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
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};
}
typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetConstIterator;   // list<CAnimSet>::const_iterator

class CAnimSetItPriorityLessThan { public:                     // empty functor (a 4-byte argument slot)
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) {}   // trivial destructor: nothing to run

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _ForwardIterator, class _Tp>
class _Temporary_buffer {
private:
    int _M_original_len;
    int _M_len;
    _Tp* _M_buffer;
    void _M_allocate_buffer();                          // 0x0070F410
    void _M_initialize_buffer(const _Tp&, const __true_type&) {}
    void _M_initialize_buffer(const _Tp& __val, const __false_type&) { uninitialized_fill_n(_M_buffer, _M_len, __val); }
public:
    int size() const { return _M_len; }
    _Tp* begin() { return _M_buffer; }
    _Tp* end() { return _M_buffer + _M_len; }
    _Temporary_buffer(_ForwardIterator __first, _ForwardIterator __last)
    {
        _M_len = __last - __first;
        _M_allocate_buffer();
        if (_M_len > 0)
            _M_initialize_buffer(*__first, __false_type());
    }
    ~_Temporary_buffer()
    {
        _Destroy(_M_buffer, _M_buffer + _M_len);
        free(_M_buffer);
    }
};

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);        // 0x00711190
template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp);
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __merge_sort_with_buffer(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                         _Distance*, _Compare __comp);                                         // 0x007116B0
template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2, _Pointer __buffer, _Distance __buffer_size,
                                 _Compare __comp);                                                             // 0x00711340

// Same-TU stand-in bodies (the STLport text of 0x0070F410 / 0x00711640 / 0x00711920, all later in this TU): VC7.1 uses
// what it learns from them.
#pragma auto_inline(off)
template <class _ForwardIterator, class _Tp>
__declspec(noinline) void _Temporary_buffer<_ForwardIterator, _Tp>::_M_allocate_buffer()
{
    _M_original_len = _M_len;
    _M_buffer = 0;
    if (_M_len > (int)(0x7FFFFFFF / sizeof(_Tp)))
        _M_len = 0x7FFFFFFF / sizeof(_Tp);
    while (_M_len > 0) {
        _M_buffer = (_Tp*)malloc(_M_len * sizeof(_Tp));
        if (_M_buffer)
            break;
        _M_len /= 2;
    }
}
template <class _RandomAccessIter, class _Compare>
__declspec(noinline) void __fastcall __inplace_stable_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first < 15) {
        __insertion_sort(__first, __last, __comp);
        return;
    }
    _RandomAccessIter __middle = __first + (__last - __first) / 2;
    __inplace_stable_sort(__first, __middle, __comp);
    __inplace_stable_sort(__middle, __last, __comp);
    __merge_without_buffer(__first, __middle, __last, __middle - __first, __last - __middle, __comp);
}
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
__declspec(noinline) void __fastcall __stable_sort_adaptive(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                                            _Distance __buffer_size, _Compare __comp)
{
    _Distance __len = (__last - __first + 1) / 2;
    _RandomAccessIter __middle = __first + __len;
    if (__len > __buffer_size) {
        __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, __comp);
        __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, __comp);
    }
    else {
        __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0, __comp);
        __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0, __comp);
    }
    __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), _Distance(__last - __middle), __buffer,
                     __buffer_size, __comp);
}
#pragma auto_inline(on)

template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
void __fastcall __stable_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Distance*, _Compare __comp)
{
    _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
    if (buf.begin() == 0)
        __inplace_stable_sort(__first, __last, __comp);
    else
        __stable_sort_adaptive(__first, __last, buf.begin(), _Distance(buf.size()), __comp);
}

template void __fastcall __stable_sort_aux<CAnimSetConstIterator*, CAnimSetConstIterator, int, CAnimSetItPriorityLessThan>(
    CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, int*, CAnimSetItPriorityLessThan);
}