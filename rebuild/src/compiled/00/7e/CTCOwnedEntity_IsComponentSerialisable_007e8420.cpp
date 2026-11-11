// CTCOwnedEntity::IsComponentSerialisable() const -- serialisable only while the owner is alive
// Leaf: ?IsComponentSerialisable@CTCOwnedEntity@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E8420, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 23 of CTCOwnedEntity's vtable 0x0123EB4C (the slot holds this body in 1 vtable(s), owner CTCOwnedEntity); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCOwnedEntity@@UBE_NXZ; `ret` matches the signature.
// Was: CTCOwnedEntity::vfunc_23 (rebuild/src/compiled/00/7e/CTCOwnedEntity_vfunc23_007e8420.cpp).
#include "engine/ETCInterfaceType.h"
#include "engine/CTCOwnedEntity.h"
bool CTCOwnedEntity::IsComponentSerialisable() const
{
    const CThing* owner = POwner.Get();
    if (owner != 0 && !owner->IsDead())
        return true;
    return false;
}
