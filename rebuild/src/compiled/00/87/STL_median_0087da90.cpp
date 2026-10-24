// _STL::__median(const CAnimationEntry* const& a, const CAnimationEntry* const& b, const CAnimationEntry* const& c, CTurningAnimCompare comp) -- median of three turning-animation entries under the rotation comparator (introsort pivot)
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DA90, length 184 bytes (`ret 8`).
// Identity: called once from the introsort loop of the CCreatureAction_InterruptableMidAttackAutoTurn TU (0x0087E56D);
//   SGI/STLport `__median(a, b, c, comp)` over the vector<const CAnimationEntry*> the turning animations are sorted in,
//   comparator `?A0x37766ae9::CTurningAnimCompare` (Ego_r.pdb). The catalog label "FindClosestRotationDirect" is
//   descriptive.
// Callees: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (landed), the out-of-line
//   CTurningAnimCompare::operator()(a, b) @ 0x0087C660 (this chunk).
// Shape: /Gr TU (`__fastcall`: a in ecx, b in edx, c and the 4-byte comparator by value on the stack). VC7.1 inlines the
//   in-class comparator at its FIRST call only (the two GetRotationInAnimation calls on *a, both rotations in the
//   8-byte frame) and calls the out-of-line copy for the other four, with `this` = the comparator's argument slot.
//   The comparator keeps the retail quirk of reading its left operand twice.
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

namespace _STL {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template const CAnimationEntry* const& __fastcall __median<const CAnimationEntry*, CTurningAnimCompare>(
    const CAnimationEntry* const&, const CAnimationEntry* const&, const CAnimationEntry* const&, CTurningAnimCompare);
}