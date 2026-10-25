// _STL::__insertion_sort(const CAnimationEntry** first, const CAnimationEntry** last, CTurningAnimCompare comp) -- insertion sort of the turning animations: linear-insert every entry after the first into the sorted prefix
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E370, 50 bytes (+ int3 padding), `ret 4` (/Gr algorithm TU: first in
//   ecx, last in edx, the 4-byte comparator by value on the stack).
// Identity: STLport 4.x `__insertion_sort(first, last, comp)` of the CCreatureAction_InterruptableMidAttackAutoTurn
//   sort (comparator `?A0x37766ae9::CTurningAnimCompare`, Ego_r.pdb); callers __final_insertion_sort 0x0087E400 (both
//   arms). The catalog label "for_each_range" is descriptive.
// Callee: __linear_insert(first, i, *i, comp) @ 0x0087E250 (landed, same instantiation, out of line).
// Shape: empty-range early out, then the `i = first + 1` loop; the comparator stays in ebp and is re-pushed by value.
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0087E250

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<TEntry*, CTurningAnimCompare>(TEntry*, TEntry*, CTurningAnimCompare);
}