// CTurningAnimCompare::operator()(const CAnimationEntry* a, const CAnimationEntry* b) const -- the turning-animation sort order: is a's rotation closer to the wanted angle than b's (retail reads a twice)
// Leaf: ??RCTurningAnimCompare
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C660, length 76 bytes (`ret 8`).
// Identity: Ego_r.pdb names the functor `?A0x37766ae9::CTurningAnimCompare` { float RotationAngle } of the
//   CCreatureAction_InterruptableMidAttackAutoTurn TU; this binary overload is the comparator of the STLport sort/heap
//   instances around it (__median 0x0087DA90 calls it out of line, the landed _Adjust_heap 0x0087DE50 and __push_heap
//   0x0087DCA0 inline it). The catalog label "CAnimationEntry_FindClosestRotation" is descriptive.
// Callee: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (landed).
// Retail quirk (as in the landed heap instances): both GetRotationInAnimation calls are made on the LEFT entry (`mov
//   ecx,edi` twice), so b is never read and the result is |R - x| < |R - x|.
// Shape: the first rotation is homed in a's dead argument slot, the second in the `push ecx` frame word; each distance
//   is rounded into a float local (`(float)fabs(...)`), which leaves the `fxch st(1) ; fxch st(1)` pair before fcompp;
//   the out-of-line copy returns through `if (...) return true; return false;` (byte `mov al,1` / `xor al,al` exits --
//   `return delta_a < delta_b;` widens both to eax).
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
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const;
};

bool CTurningAnimCompare::operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
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
}