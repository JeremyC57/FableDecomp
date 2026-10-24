// _STL::__unguarded_insertion_sort_aux(CGroup* first, CGroup* last, CGroup*, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- unguarded linear insert of every bone-weight group of [first, last) under a function-pointer predicate
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD1CA0, length 114 bytes (+14 int3), `ret 8`.
// Callers: __unguarded_insertion_sort @ 0x00AD2B70 (`push comp ; push 0 ; call`) and two more 8-byte sorts folded onto
//   this body (0x00AD628C, 0x00AD75A9).
// Element: QuadricOptimiserInternals::COptimisedVertex::CGroup (Ego_r.pdb, 8 bytes), as the landed _Push_heap 0x00AD0920
//   and the __unguarded_linear_insert 0x00AD08B0 of the same sort. The manifest's "Insertion_Sort_Elements" is
//   descriptive.
// Shape: /Gr TU `__fastcall` (first in ecx, last in edx, the unused value-type pointer and the comparator on the stack);
//   STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp)` with the insert inlined: the
//   value copy lives in an 8-byte frame slot under an `and esp,-8` aligned frame, the comparator re-read from [ebp+0Ch].
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

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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

template void __fastcall __unguarded_insertion_sort_aux(CGroup*, CGroup*, CGroup*, TGroupCompare);
}