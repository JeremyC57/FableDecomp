// Behaviour test for 0x0062EAC0 CCombatActionBase::GetIsToFaceTargetCone (links the REAL row object, NF259; the old test printed PASS
// unconditionally). A qualified (non-virtual) call on raw storage, as the sibling 0x00956730's test: the body ignores this.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include <stdio.h>
int main()
{
    unsigned long storage[0x114 / 4] = { 0 };
    const CCombatActionBase* action = (const CCombatActionBase*)storage;
    float r = action->CCombatActionBase::GetIsToFaceTargetCone();
    if (r == 1.0f / 9.0f && r > 0.1111f && r < 0.1112f) { printf("PASS_0062eac0\n"); return 0; }
    printf("FAIL_0062eac0\n");
    return 1;
}
