// CTCCarryable::OnKill() -- when the carryable is held, tell the carrier's CTCCarrying component to drop this thing
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705150, length 19 bytes. CTCCarryable vtable 0x0123B35C slot 14.
// Callees: CTCCarrying::RemoveThing(CThing*) @ 0x006A19B0 (`ret 4`; landed in this chunk).
// Identity: slot 14 of the CTCBase-derived vtables is OnKill (CTCCarrying/CTCInventory/CTCVillage/...
// carry that label at the same slot); the manifest's "vfunc_14" is the placeholder.
// Layout: engine/CTCBase.h (vptr, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9) and the
// Ego_r.pdb CTCCarryable members (ForceFragility +0xc, PCarryingTC +0x10).
// Shape: `this` is parked in eax so ecx can carry the carrier; the null test guards the member call,
// which is not tail-called (the pushed argument keeps the `call ; ret`).
#include "engine/CTCBase.h"

struct CTCCarrying {
    void RemoveThing(CThing* thing);                // 0x006A19B0
};

struct CTCCarryable : CTCBase {
    bool          ForceFragility;                   // +0x0c
    CTCCarrying*  PCarryingTC;                      // +0x10
    void OnKill();
};

void CTCCarryable::OnKill()
{
    if (PCarryingTC)
        PCarryingTC->RemoveThing(ParentThing);
}