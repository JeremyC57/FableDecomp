// CVillageTaskSecuritySweep::IsValid() const -- the sweep task is valid until it has been marked invalid
// Leaf: ?IsValid@CVillageTaskSecuritySweep@@QBE_NXZ
// Retail: Fable.exe @ 0x00860300, 11 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 3 (+0x0C) of CVillageTaskSecuritySweep 0x0125A868 (read from Fable.exe); slots 2 and 4..8
// are the landed GetTaskType / IsTaskAvailable / IsThingInvolvedWithTask / RemoveThingFromTask / IsCreatureAssignedToTask /
// AssignCreatureToTask, and the only remaining no-argument bool virtual in Ego_r.pdb's name set for the class is
// ?IsValid@CVillageTaskSecuritySweep@@UBE_NXZ (its partner ?SetInvalid@CVillageTaskSecuritySweep@@QAEXXZ sets the flag).
// Callees: none.
// Layout: engine/CVillageTaskSecuritySweep.h (HasBecomeInvalid +0x30) through a donor include. That header carries the vptr
// as a data word, so the slot-3 virtual is declared as a plain member (same code, `QBE` instead of retail's `UBE`), as landed
// 0x00661FC0 does.
// Shape: `xor eax,eax ; test dl,dl ; sete al` is `!bool` of a byte member loaded into dl.
#define CVillageTaskSecuritySweep CVillageTaskSecuritySweepDonor
#include "engine/CVillageTaskSecuritySweep.h"
#undef CVillageTaskSecuritySweep

class CVillageTaskSecuritySweep : public CVillageTaskSecuritySweepDonor {
public:
    bool IsValid() const;                                   // virtual slot 3 in retail
};

bool CVillageTaskSecuritySweep::IsValid() const
{
    return !HasBecomeInvalid;
}