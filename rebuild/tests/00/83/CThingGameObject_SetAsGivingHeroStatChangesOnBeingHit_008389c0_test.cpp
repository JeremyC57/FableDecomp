// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `flags` -> `GiveHeroStatChangesOnBeingHitOverrideSet[0]`.
#include "engine/CThingGameObject.h"
#include <stdio.h>


void model(CThingGameObject *gameObject, bool value)
{
    gameObject->GiveHeroStatChangesOnBeingHitOverrideSet[0] = (unsigned char)((((value & 1) | 2) << 2) | (gameObject->GiveHeroStatChangesOnBeingHitOverrideSet[0] & 0xfb));
}

int main()
{
    CThingGameObject o;
    o.GiveHeroStatChangesOnBeingHitOverrideSet[0] = 0xFF;
    model(&o, true);
    if ((o.GiveHeroStatChangesOnBeingHitOverrideSet[0] & 0x04) == 0) { printf("FAIL1\n"); return 1; }

    o.GiveHeroStatChangesOnBeingHitOverrideSet[0] = 0x00;
    model(&o, true);
    if (o.GiveHeroStatChangesOnBeingHitOverrideSet[0] != 0x0C) { printf("FAIL2 %02x\n", o.GiveHeroStatChangesOnBeingHitOverrideSet[0]); return 1; }

    o.GiveHeroStatChangesOnBeingHitOverrideSet[0] = 0x00;
    model(&o, false);
    if (o.GiveHeroStatChangesOnBeingHitOverrideSet[0] != 0x08) { printf("FAIL3 %02x\n", o.GiveHeroStatChangesOnBeingHitOverrideSet[0]); return 1; }

    o.GiveHeroStatChangesOnBeingHitOverrideSet[0] = 0xF3;
    model(&o, true);
    if (o.GiveHeroStatChangesOnBeingHitOverrideSet[0] != 0xFF) { printf("FAIL4 %02x\n", o.GiveHeroStatChangesOnBeingHitOverrideSet[0]); return 1; }

    o.GiveHeroStatChangesOnBeingHitOverrideSet[0] = 0xFF;
    model(&o, false);
    if (o.GiveHeroStatChangesOnBeingHitOverrideSet[0] != 0xFB) { printf("FAIL5 %02x\n", o.GiveHeroStatChangesOnBeingHitOverrideSet[0]); return 1; }

    printf("BEHAVIOUR_OK\n");
    return 0;
}