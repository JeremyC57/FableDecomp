// _STL::__partial_sort(const CAnimationEntry** first, const CAnimationEntry** middle, const CAnimationEntry** last, const CAnimationEntry**, CTurningAnimCompare comp) -- introsort depth-limit fallback for the turning animations: heap the prefix, pop every closer entry from the rest into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E450, 152 bytes, `ret 0Ch` (/Gr algorithm TU: first in ecx, middle
//   in edx; last, the value-type tag pointer and the 4-byte comparator on the stack).
// Identity: STLport 4.x `__partial_sort` of the CCreatureAction_InterruptableMidAttackAutoTurn sort, called by
//   __introsort_loop 0x0087E530 as `partial_sort(first, last, last, comp)` (`push comp ; push 0 ; push last`);
//   comparator `?A0x37766ae9::CTurningAnimCompare` (Ego_r.pdb). The catalog label
//   "introsort_3way_partial_sort_CAnimationEntry" is descriptive.
// Callees: __make_heap(first, middle, comp, _Tp*, _Distance*) @ 0x0087E0D0 (`push 0` x2), __adjust_heap @ 0x0087DE50
//   (landed), sort_heap @ 0x0087E3B0, CAnimationEntry::GetRotationInAnimation @ 0x005DBFD0 (landed).
// Shape: make_heap inline over __make_heap; the comparator is inlined in the loop (the left operand `*i` copied to a
//   frame word because every callee-saved register is taken, read twice: the TU's retail quirk); __pop_heap inline
//   (`*i = *first; __adjust_heap(first, 0, middle - first, val, comp)`); `last` re-read from its slot every pass.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const; // 0x005DBFD0
};

namespace {
class CTurningAnimCompare {                             // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const    // out of line @ 0x0087C660
    {
        float rotation_a;
        float rotation_b;
        a->GetRotationInAnimation(&rotation_a);
        a->GetRotationInAnimation(&rotation_b);
        float delta_a = (float)fabs(RotationAngle - rotation_a);
        float delta_b = (float)fabs(RotationAngle - rotation_b);
        if (delta_a < delta_b)
            return true;
        return false;
    }
};
}
typedef const CAnimationEntry* TEntry;

namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0087DE50
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x0087E0D0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0087E3B0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (TEntry*)0, (int*)0);
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
template void __fastcall __partial_sort<TEntry*, TEntry, CTurningAnimCompare>(TEntry*, TEntry*, TEntry*, TEntry*,
                                                                              CTurningAnimCompare);
}