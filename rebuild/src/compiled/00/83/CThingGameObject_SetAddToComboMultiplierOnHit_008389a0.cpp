#include "engine/CThingGameObject.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingGameObject_Methods : CThingGameObject {
    void SetAddToComboMultiplierOnHit(bool v);
};

void CThingGameObject_Methods::SetAddToComboMultiplierOnHit(bool v)
{
    // Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `flags` -> GiveHeroStatChangesOnBeingHitOverrideSet[0] (+0xc8 bitfield byte).
    GiveHeroStatChangesOnBeingHitOverrideSet[0] = (unsigned char)((GiveHeroStatChangesOnBeingHitOverrideSet[0] & 0xfe) | (v & 1) | 2);
}