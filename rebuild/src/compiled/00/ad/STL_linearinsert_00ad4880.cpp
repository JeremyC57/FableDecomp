// std::__linear_insert(COptimisedVertex::CGroup* first, COptimisedVertex::CGroup* last, CGroup val, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- STLport insertion-sort step over a vertex's bone groups: a value that sorts before the first group shifts [first, last) up one slot and goes first, anything else sinks from last
// Flags: /Gr
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD4880, 102 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first in
//   ecx, last in edx, the 8-byte value as two dwords and the comparator function pointer on the stack).
// Identity: single caller 0x00AD5B73, the landed __insertion_sort 0x00AD5B50 of the same family; its callee
//   0x00AD08B0 (landed __unguarded_linear_insert) models QuadricOptimiserInternals::COptimisedVertex::CGroup (Ego_r.pdb,
//   BoneIndex + BoneWeight) sorted by the static CGroup::CompareWeight pointer. The catalog label
//   "std::_InsertionSort_PartitionHelper" is descriptive.
// Callee: __unguarded_linear_insert(last, val, comp) @ 0x00AD08B0 (__fastcall: last in ecx, comp in edx -- the first
//   register-sized argument after the 8-byte val -- and val on the stack, `ret 8`).
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- the comparator is called through ebx with ecx = &val, edx = first;
//   CGroup has no trivial-assignment trait in STLport, so copy_backward is the counted random-access loop, inlined.
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {                           // Ego_r.pdb COptimisedVertex::CGroup, 8 bytes
    public:
        unsigned long BoneIndex;             // +0x0
        float         BoneWeight;            // +0x4
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupCompare)(const CGroup&, const CGroup&);

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // CGroup: 0x00AD08B0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template void __linear_insert<CGroup*, CGroup, PFNGroupCompare>(CGroup*, CGroup*, CGroup, PFNGroupCompare);
}