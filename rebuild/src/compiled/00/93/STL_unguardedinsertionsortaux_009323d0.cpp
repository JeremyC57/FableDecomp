// _STL::__unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing**, bool (__fastcall* comp)(CThing*, CThing*)) -- STLport final unguarded insertion pass of the minion guard's criminal sort: insert each thing pointer into the sorted run before it
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009323D0, 80 bytes, /Gr `__fastcall` (first in ecx -> ebp, last in edx spilled to
//   the `push ecx` frame word; the value-type tag pointer and the comparator function pointer on the stack, `ret 8`).
// Identity: STLport 4.6 `__unguarded_insertion_sort_aux<CThing**, CThing*, bool (__fastcall*)(CThing*, CThing*)>`: the
//   landed sort 0x009328D0 of CAIStateGroup_MinionReactToCriminal::Update calls it from its inlined
//   __final_insertion_sort (`(first + 16, last, (CThing**)0, comp)`); the TU's landed __unguarded_linear_insert 0x00932330
//   is the same insert out of line. Callers: 0x00932507, 0x00932747, 0x00932926. The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::_Destroy" is propagated.
// Shape: `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` with the
//   STLport insert expanded inline: val in ebx, the trailing `last` in edi, `next` in esi, the comparator called through
//   its stack slot (`call [esp+1Ch]`, both elements in ecx/edx).
class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // e.g. the height compare 0x00930D90

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<CThing**, CThing*, ThingCompare>(CThing**, CThing**, CThing**, ThingCompare);
}