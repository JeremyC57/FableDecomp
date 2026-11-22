// Behaviour test for 0x0062EA40 CCombatActionBase::IsToUseRotationInAnimation (links the REAL row object, NF261; the HEAD test was a standalone copy).
// A qualified (non-virtual) call on raw storage, as the landed 0x0062EAC0 test: the body ignores this.
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
    bool r = action->CCombatActionBase::IsToUseRotationInAnimation();
    if (r == true) { printf("AUTO_TINY_0062ea40_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_0062ea40_TEST FAIL\n");
    return 1;
}
