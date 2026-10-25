// std::__linear_insert(const CAnimationEntry** first, const CAnimationEntry** last, const CAnimationEntry* val, CTurningAnimCompare comp) -- insertion-sort step for the turning animations: a new minimum shifts the whole prefix up with memmove, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E250, length 123 bytes (`ret 8`).
// Identity: STLport `__linear_insert(first, last, val, comp)` of the CCreatureAction_InterruptableMidAttackAutoTurn
//   sort (caller 0x0087E38F, the __insertion_sort), comparator `?A0x37766ae9::CTurningAnimCompare` (Ego_r.pdb). The
//   catalog label "AnimationEntry_FilterAndInsert" is descriptive.
// Callees: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (landed), memmove @ 0x00BFEAE6 (CRT),
//   __unguarded_linear_insert @ 0x0087DC30 (this chunk).
// Shape: /Gr TU (`__fastcall`: first in ecx, last in edx, the value and the comparator on the stack). The comparator
//   is inlined (first rotation homed in val's argument slot, second in the `push ecx` word; val read twice: the retail
//   quirk); STLport's pointer copy_backward reduces to `__copy_trivial_backward` = one `memmove` guarded by `n > 0`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
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
inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}
template <class _Tp>
inline _Tp** copy_backward(_Tp** __first, _Tp** __last, _Tp** __result)
{
    return (_Tp**)__copy_trivial_backward(__first, __last, __result);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0087DC30

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template void __fastcall __linear_insert<const CAnimationEntry**, const CAnimationEntry*, CTurningAnimCompare>(
    const CAnimationEntry**, const CAnimationEntry**, const CAnimationEntry*, CTurningAnimCompare);
}