// std::__partial_sort(COptimisedVertex::CGroup* first, COptimisedVertex::CGroup* middle, COptimisedVertex::CGroup* last, CGroup*, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- STLport partial sort of a vertex's bone groups: heapify [first, middle), swap every smaller later group into the heap, then sort the heap
// Flags: /Gr
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD5BC0, 105 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first in
//   ecx, middle in edx, then last, the dead value-type tag and the comparator function pointer on the stack).
// Identity: callers 0x00AD63FC and 0x00AD6FDD (partial_sort / the __introsort_loop depth-limit fallback of the CGroup
//   sort; the landed __introsort_loop 0x00AD6F10 is the same family). The comparator is the static
//   CGroup::CompareWeight pointer, called through ebp with ecx = i, edx = first. The catalog label "std::_Heap_PopSort"
//   is descriptive.
// Callees: __make_heap(first, middle, comp, (CGroup*)0, (int*)0) @ 0x00AD2CE0 (row 0x00AD2CE0, `push 0 ; push 0`),
//   __adjust_heap(first, 0, middle - first, val, comp) @ 0x00AD1DD0, sort_heap(first, middle, comp) @ 0x00AD4960.
// Shape: STLport 4.6 __partial_sort with make_heap and __pop_heap inline: the value `_Tp(*i)` is loaded into ecx/eax
//   before `*i = *first` is stored, then __adjust_heap.
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                   _Compare __comp);   // CGroup: 0x00AD1DD0
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                 _Distance*);   // CGroup: 0x00AD2CE0
template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // CGroup: 0x00AD4960

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CGroup*)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                    _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __partial_sort<CGroup*, CGroup, PFNGroupCompare>(CGroup*, CGroup*, CGroup*, CGroup*, PFNGroupCompare);
}