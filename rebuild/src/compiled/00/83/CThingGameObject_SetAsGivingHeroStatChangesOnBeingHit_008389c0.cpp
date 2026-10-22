
#include "engine/CThingGameObject.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall SetAsGivingHeroStatChangesOnBeingHit(CThingGameObject *gameObject, int edx_dummy, bool value)
{
    gameObject->flags = (unsigned char)((((value & 1) | 2) << 2) | (gameObject->flags & 0xfb));
}