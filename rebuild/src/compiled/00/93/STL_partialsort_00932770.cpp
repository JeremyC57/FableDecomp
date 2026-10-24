// _STL::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, ThingCompare comp) -- STLport partial sort: heap the front, swap every later element that sorts before the root into the heap, then heap-sort the front
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00932770, 95 bytes, `ret 0Ch` (first in ecx, middle in edx; last, the value-type pointer and comp on the stack).
// Name: STLport `__partial_sort<CThing**, CThing*, bool (__fastcall*)(CThing*, CThing*)>`; caller __introsort_loop
//   0x009328BD (and 0x009327DC). Callees: __make_heap 0x00932540, __adjust_heap 0x00932420, sort_heap 0x009326D0. The
//   catalog's "Sort_IntroSort_Buildings" is descriptive.
// Element: CThing* sorted under a plain `bool (__fastcall*)(CThing*, CThing*)` function pointer -- the sort of
//   CAIStateGroup_MinionReactToCriminal::Update (0x0093150A: `sort(first, last, &0x00930D90)`, a file-local compare whose
//   body is `a->GetPosition().Z < b->GetPosition().Z` through CThing::GetPosition 0x004C73D0). The pointee is only ever
//   handed to GetPosition, so CThing (or a class with CThing at offset 0) is all the bytes show. The catalog's
//   CThingBuilding / CIsHouseLargerThan labels are propagated from the Ego_r.pdb Dinkumware instantiation of another TU.
// /Gr algorithm TU: the first two pointer/int arguments in ecx/edx (the element value and the comparator pointer are
//   register candidates), the rest on the stack; the comparator is called with both elements in ecx/edx.
// Landed siblings in the TU: __median 0x009322D0, __push_heap 0x00932370, sort 0x009328D0.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, _Tp(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- make_heap and __pop_heap inlined (the
//   `*i = *first` store scheduled between the argument pushes), comp in ebp.
#include <stddef.h>
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);   // 0x00BFEAE6

class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // e.g. the height compare 0x00930D90

namespace _STL {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x00932540

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x00932420

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x009326D0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CThing**)0, (int*)0);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(CThing**, CThing**, CThing**, CThing**, ThingCompare);
}