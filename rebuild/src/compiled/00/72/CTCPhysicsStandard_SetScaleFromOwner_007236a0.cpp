// CTCPhysicsStandard::SetScaleFromOwner() -- re-applies the physics mesh bank index of PPhysicsMeshInfo (virtual SetPhysicsMeshBankIndex, slot 85)
// Leaf: ?SetScaleFromOwner@CTCPhysicsStandard@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007236A0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 86 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts SetScaleFromOwner(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetScaleFromOwner@CTCPhysicsStandard@@UAEXXZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_86 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc86_007236a0.cpp).
// No shared header for CThingPhysicsMeshInfo: minimal local definition with the Ego_r.pdb member the body reads.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
class CThingPhysicsMeshInfo {
public:
    unsigned short BankIndex;                                       // +0x00 (Ego_r CThingPhysicsMeshInfo, 28 bytes)
};
void CTCPhysicsStandard::SetScaleFromOwner()
{
    if (PPhysicsMeshInfo)
        SetPhysicsMeshBankIndex(PPhysicsMeshInfo->BankIndex);
}
