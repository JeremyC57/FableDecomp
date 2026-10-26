
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `flags` -> `GiveHeroStatChangesOnBeingHitOverrideSet[0]`.
#include "engine/CThingGameObject.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall SetAsGivingHeroStatChangesOnBeingHit(CThingGameObject *gameObject, int edx_dummy, bool value)
{
    gameObject->GiveHeroStatChangesOnBeingHitOverrideSet[0] = (unsigned char)((((value & 1) | 2) << 2) | (gameObject->GiveHeroStatChangesOnBeingHitOverrideSet[0] & 0xfb));
}