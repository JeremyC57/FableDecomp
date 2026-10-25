// std::sort<CThing**, bool (__fastcall*)(CThing*, CThing*)>(first, last, comp) -- STLport sort: for a non-empty range run
//   __introsort_loop(first, last, (CThing**)0, __lg(last - first) * 2, comp), then the inlined __final_insertion_sort
//   (ranges longer than __stl_threshold 16: __insertion_sort over the first 16, __unguarded_insertion_sort_aux over the rest).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009328D0, 112 bytes; /Gr TU (__fastcall: first in ecx, last in edx, comp on the
//   stack), `ret 4`.
// Identity: the only caller is CAIStateGroup_MinionReactToCriminal::Update (0x0093150A: `sort(first, last, &0x00930D90)`,
//   a file-local `bool __fastcall (CThing*, CThing*)` height compare through CThing::GetPosition 0x004C73D0); its callees are
//   the landed TU siblings __introsort_loop<CThing**, CThing*, int, ThingCompare> 0x009327F0 and __insertion_sort<CThing**,
//   ThingCompare> 0x00932660, plus __unguarded_insertion_sort_aux 0x009323D0. The element is CThing* (not a 4-byte
//   struct) and the comparator a plain function pointer (not a functor); the catalog's "Sort_SElement4" / manifest's
//   "Sort_QuickSort_Main_Particles" are generated/propagated. FINDINGS chunk 406.
// Callees: __introsort_loop 0x009327F0, __insertion_sort 0x00932660, __unguarded_insertion_sort_aux 0x009323D0.
// Leaf: ??$sort@PAPAVCThing@@P6I_NPAV1@0@Z@std@@YIXPAPAVCThing@@0P6I_NPAV1@1@Z@Z
class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // the height compare 0x00930D90

namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);   // 0x009327F0
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00932660
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x009323D0

template <class _RandomAccessIter, class _Compare>
inline void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort_aux(__first + 16, __last, (CThing**)0, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CThing**)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CThing**, ThingCompare>(CThing** __first, CThing** __last, ThingCompare __comp);
}
