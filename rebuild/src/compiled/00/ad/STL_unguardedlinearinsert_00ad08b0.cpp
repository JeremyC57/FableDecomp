// std::__unguarded_linear_insert(CGroup* last, CGroup val, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- SGI insertion step without a lower bound: shift the bone-weight groups up while comp(val, *prev), then store val
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD08B0, length 68 bytes (+12 int3), `ret 8`.
// Caller: __linear_insert @ 0x00AD4880 (0x00AD48DB); the comparator is called through ebx.
// Element: QuadricOptimiserInternals::COptimisedVertex::CGroup (Ego_r.pdb, 8 bytes: BoneIndex, BoneWeight), the vector
//   sorted with a `bool (__fastcall*)(const CGroup&, const CGroup&)` predicate (the landed _Push_heap 0x00AD0920 is the
//   same sort's heap step). The manifest's "Insertion_Sort_Shift_Single" is descriptive.
// Shape: /Gr TU `__fastcall` template: last in ecx -> edi, the comparator (the first 4-byte argument after the 8-byte
//   val) in edx -> ebx, val on the stack (`ret 8`); next = last - 1 in esi; the implicit 8-byte assignment moves both
//   words through integer registers.
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

template void __fastcall __unguarded_linear_insert(CGroup*, CGroup, TGroupCompare);
}