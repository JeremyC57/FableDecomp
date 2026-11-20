// Behaviour test for 0x006620F0 (links the REAL row object): CThingCreatureBase::GetDefHoverMediumSpeed() const returns the creature
// definition's MaxHoverSpeedMedium (the def pointer at +0xE0, the float at +0xA8 of the def).
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETDEFHOVERSPEEDS
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include <stdio.h>
int main()
{
    unsigned long creature[0x200 / 4];
    unsigned long def[0x200 / 4];
    for (int i = 0; i < 0x200 / 4; ++i) { creature[i] = 0; def[i] = 0; }
    creature[0xE0 / 4] = (unsigned long)def;
    *(float*)((char*)def + 0xA8) = 3.5f;
    *(float*)((char*)def + 0xA8 - 4) = 1.0f;
    *(float*)((char*)def + 0xA8 + 4) = 2.0f;
    const CThingCreatureBase* c = (const CThingCreatureBase*)creature;
    float r = c->GetDefHoverMediumSpeed();
    *(float*)((char*)def + 0xA8) = -7.25f;
    float r2 = c->GetDefHoverMediumSpeed();
    if (r != 3.5f || r2 != -7.25f) { printf("FAIL_006620f0\n"); return 1; }
    printf("CTCPhysicsBase_006620f0_TEST PASS\n");
    return 0;
}
