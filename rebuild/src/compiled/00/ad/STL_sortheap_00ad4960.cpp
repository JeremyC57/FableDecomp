// _STL::sort_heap(COptimisedVertex::CGroup* first, COptimisedVertex::CGroup* last, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- STLport sort_heap over a vertex's bone groups: pop the root to the back until one element remains
// Flags: /Gr
// Leaf: ??$sort_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD4960, 0x5C bytes (+ int3 padding), `ret 4`.
// Callee: __adjust_heap @ 0x00AD1DD0 (manifest `_Adjust_heap<CSpeechAnalyser::CMatchProbability*,...>`;
// __fastcall: first in ecx, hole 0 in edx, then len, the 8-byte value by value (two dwords) and the
// comparator, `ret 10h`).  The comparator is a plain function pointer: the callee sifts with
// `lea ecx,[edi+esi*8] ; lea edx,[ecx-8] ; call [esp+24h]` (a /Gr function pointer is __fastcall).
// The 8-byte element (stride 8: `and ecx,-8 ; cmp ecx,8` is `(last - first) > 1`, `sar edx,3` the
// distance) is copied as two plain dwords both ways (trivial POD copy).  Ego_r.pdb names this family:
// `_Sort_heap<COptimisedVertex::CGroup*, bool (__fastcall*)(const CGroup&, const CGroup&)>` with the static
// comparator CGroup::CompareWeight (CGroup { unsigned long BoneIndex; float BoneWeight; }), used by
// COptimisedVertex::LimitGroupCount.  The catalog name `_Sort_heap<long*>` and the callee's
// CMatchProbability label are propagated.
// Shape: STLport sort_heap -> pop_heap -> __pop_heap_aux -> __pop_heap all inline; `_Tp(*(last - 1))`
// is loaded into ecx/edx before `*result = *first` is stored, then the call.  /Gr: every free
// template is __fastcall (first in ecx, last in edx, comp on the stack).
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

namespace _STL {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                       _RandomAccessIterator __result, _Tp __value, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __value, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (CGroup*)0, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template void sort_heap<CGroup*, PFNGroupCompare>(CGroup*, CGroup*, PFNGroupCompare);

} // namespace _STL