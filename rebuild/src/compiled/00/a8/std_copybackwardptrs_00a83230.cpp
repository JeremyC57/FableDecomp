// std::__copy_backward_ptrs(std::list<C2DLineF>* first, std::list<C2DLineF>* last, std::list<C2DLineF>* result, const __false_type&) -- STLport backward copy over a 4-byte element with a user assignment operator (list<C2DLineF>): n = last - first elements assigned back to front so the copy ends at result, returning its new start
// Leaf: ??$__copy_backward_ptrs@PAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@PAV12@@std@@YIPAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@0@PAV10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83230, 49 bytes, __fastcall (first ecx, last edx), `ret 8`; the element's
//   operator= 0x00517720 called per element (thiscall on --result with --last pushed); returns result - n in eax.
// Identity (proven): std::__copy_backward_ptrs<list<C2DLineF>*, list<C2DLineF>*>; evidence: its only caller 0x00A84690 is the vector<list<C2DLineF> >::_M_fill_insert shape (`__copy_backward_ptrs(position, old_finish - n, old_finish, tag)` at 0x00A84708 after the 4-byte `sar ecx,2` capacity test), and its per-element call is the landed list<C2DLineF>::operator= 0x00517720 -- the element is list<C2DLineF>.
// Label: the manifest's "Vector_DestroyBackwardRange" is a descriptive / propagated name (family H041: six 49-byte backward copies that differ
//   only in the element's assignment callee and the stack-argument count).
// Callees: list<C2DLineF>::operator= 0x00517720 (landed 0x00517720, ??4?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAEAAV01@ABV01@@Z).
// Shape: STLport 4.6 _algobase.h `__copy_backward_ptrs(first, last, result, const __false_type&)` -- the non-trivial-assignment overload vector::_M_fill_insert / insert / _M_range_insert call with _TrivialAss() -- with `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined; __copy_backward is __forceinline (as plain `inline` VC7.1 keeps it out of line as a
//   cdecl call, 25v49), the loop counts n down in ebx and reads the source through the constant distance last - result.
class C2DLineF;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport: one node pointer
public:
    void* _M_node;
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
};
}
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                 _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template std::list<C2DLineF>* __fastcall __copy_backward_ptrs<std::list<C2DLineF>*, std::list<C2DLineF>*>(std::list<C2DLineF>*, std::list<C2DLineF>*, std::list<C2DLineF>*, const __false_type&);
}