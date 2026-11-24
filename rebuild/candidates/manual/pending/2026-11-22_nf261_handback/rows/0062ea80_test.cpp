// Behaviour test for 0x0062EA80 CCombatActionBase::GetActionMovementVector (links the REAL row object, NF261; the HEAD test was a standalone
// copy on a local C3DVector). A qualified (non-virtual) call on raw storage: the body ignores this.
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
    C3DVector r = action->CCombatActionBase::GetActionMovementVector();
    if (r.X == 0.0f && r.Y == 1.0f && r.Z == 0.0f) { printf("MOVEVEC_OK\n"); return 0; }
    printf("MOVEVEC_FAIL x=%f y=%f z=%f\n", r.X, r.Y, r.Z);
    return 1;
}
