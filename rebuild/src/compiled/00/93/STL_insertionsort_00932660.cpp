// _STL::__insertion_sort(CThing** first, CThing** last, ThingCompare comp) -- STLport insertion sort: each element that sorts before *first is memmoved to the front, the rest take the unguarded insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00932660, 110 bytes (112 with padding), `ret 4` (first in ecx, last in edx, comp on the stack).
// Name: STLport `__insertion_sort<CThing**, bool (__fastcall*)(CThing*, CThing*)>`; callers sort 0x0093291A / 0x00932934
//   and __final_insertion_sort 0x0093273B / 0x00932759. Callees: memmove 0x00BFEAE6, __unguarded_linear_insert 0x00932330.
//   The catalog's "std_sort_DownHeap" is an autoname.
// Element: CThing* sorted under a plain `bool (__fastcall*)(CThing*, CThing*)` function pointer -- the sort of
//   CAIStateGroup_MinionReactToCriminal::Update (0x0093150A: `sort(first, last, &0x00930D90)`, a file-local compare whose
//   body is `a->GetPosition().Z < b->GetPosition().Z` through CThing::GetPosition 0x004C73D0). The pointee is only ever
//   handed to GetPosition, so CThing (or a class with CThing at offset 0) is all the bytes show. The catalog's
//   CThingBuilding / CIsHouseLargerThan labels are propagated from the Ego_r.pdb Dinkumware instantiation of another TU.
// /Gr algorithm TU: the first two pointer/int arguments in ecx/edx (the element value and the comparator pointer are
//   register candidates), the rest on the stack; the comparator is called with both elements in ecx/edx.
// Landed siblings in the TU: __median 0x009322D0, __push_heap 0x00932370, sort 0x009328D0.
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`
//   with __linear_insert inlined: `if (comp(val, *first)) { copy_backward(first, i, i + 1); *first = val; } else
//   __unguarded_linear_insert(i, val, comp);` -- copy_backward of a pointer range is STLport's __copy_trivial_backward
//   (`n = (char*)last - (char*)first; n > 0 ? memmove(result - n, first, n) : result`), the byte count strength-reduced
//   to edi (+4 per pass); last parked in the `push ecx` slot.
#include <stddef.h>
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);   // 0x00BFEAE6

class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // e.g. the height compare 0x00930D90

namespace _STL {
inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const ptrdiff_t _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _Tp>
inline _Tp** copy_backward(_Tp** __first, _Tp** __last, _Tp** __result)
{
    return (_Tp**)__copy_trivial_backward(__first, __last, __result);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00932330

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template void __fastcall __insertion_sort(CThing**, CThing**, ThingCompare);
}