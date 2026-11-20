// Behaviour test for 0x00956720 CCreatureAction_ThrowShotObject::IsToFaceTarget (links the REAL row object; NF175: the old test re-implemented the free
// stand-in). A qualified (non-virtual) call on raw storage: the body ignores this.
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
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_FACETARGET
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include <stdio.h>
int main()
{
    unsigned long storage[0x114 / 4] = { 0 };
    const CCreatureAction_ThrowShotObject* action = (const CCreatureAction_ThrowShotObject*)storage;
    bool r = action->CCreatureAction_ThrowShotObject::IsToFaceTarget();
    if (r == true) { printf("AUTO_TINY_00956720_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00956720_TEST FAIL\n");
    return 1;
}
