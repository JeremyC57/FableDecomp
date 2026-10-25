// std::__unguarded_partition(CGroup* first, CGroup* last, CGroup pivot, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- SGI quicksort partition of the bone-weight groups around a by-value pivot under a function-pointer predicate
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD2AF0, length 116 bytes (+12 int3), `ret 0Ch`.
// Caller: __introsort_loop @ 0x00AD6FA5 region; the comparator is called through ebx.
// Element: QuadricOptimiserInternals::COptimisedVertex::CGroup (Ego_r.pdb, 8 bytes), as the landed _Push_heap 0x00AD0920.
//   The manifest's "Partition_TwoWaySwap" is descriptive.
// Shape: /Gr TU `__fastcall` template (first in ecx -> esi, last in edx -> edi, the 8-byte pivot and the comparator on the
//   stack); STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` with iter_swap's temporary in eax/ecx.
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {                                  // 8 bytes (Ego_r.pdb)
    public:
        unsigned long BoneIndex;                    // +0x0
        float         BoneWeight;                   // +0x4
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* TGroupCompare)(const CGroup& a, const CGroup& b);

namespace std {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, (CGroup*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template CGroup* __fastcall __unguarded_partition(CGroup*, CGroup*, CGroup, TGroupCompare);
}