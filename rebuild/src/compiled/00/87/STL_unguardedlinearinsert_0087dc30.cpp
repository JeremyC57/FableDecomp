// std::__unguarded_linear_insert(const CAnimationEntry** last, const CAnimationEntry* val, CTurningAnimCompare comp) -- insertion-sort inner step for the turning animations: shift entries up while the comparator says val goes first
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DC30, length 95 bytes (`ret 4`).
// Identity: STLport `__unguarded_linear_insert(last, val, comp)` of the CCreatureAction_InterruptableMidAttackAutoTurn
//   sort (callers 0x0087DE35, 0x0087E065, 0x0087E2BF = __linear_insert 0x0087E250, 0x0087E429), comparator
//   `?A0x37766ae9::CTurningAnimCompare` (Ego_r.pdb). The catalog label "FindClosestRotation_binary_search" is descriptive.
// Callee: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (landed).
// Shape: /Gr TU (`__fastcall`: last in ecx, the 4-byte value in edx, the comparator by value on the stack). The
//   comparator is inlined (both rotations in the 8-byte frame, read from val twice: the retail quirk), `next` lives in
//   esi from `lea esi,[edi-4]`, the loop is the bottom-tested `jmp` back to the comparator.
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
template void __fastcall __unguarded_linear_insert<const CAnimationEntry**, const CAnimationEntry*, CTurningAnimCompare>(
    const CAnimationEntry**, const CAnimationEntry*, CTurningAnimCompare);
}