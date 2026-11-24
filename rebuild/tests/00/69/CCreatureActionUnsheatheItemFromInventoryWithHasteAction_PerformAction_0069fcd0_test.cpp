// Behaviour test for 0x0069FCD0 CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::PerformAction (links the REAL row object, NF270).
// The row forwards to CCreatureAction_UnsheatheItemFromInventory::PerformAction (0x0069F9E0) with `this` unchanged; the test defines that
// callee as a probe that records `this` and counts calls, gives the row raw storage of the retail size (0xBC) and checks one forwarded call
// on the same object.
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h"
#include <stdio.h>
static const void* g_seen_this = 0;
static int g_calls = 0;
void CCreatureAction_UnsheatheItemFromInventory::PerformAction()
{
    g_seen_this = this;
    ++g_calls;
}
int main()
{
    unsigned long storage[0xbc / 4] = { 0 };
    CCreatureAction_UnsheatheItemFromInventoryWithHasteAction* action = (CCreatureAction_UnsheatheItemFromInventoryWithHasteAction*)storage;
    action->CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::PerformAction();
    if (g_calls == 1 && g_seen_this == (const void*)storage) { printf("PASS_0069fcd0\n"); return 0; }
    printf("FAIL_0069fcd0\n");
    return 1;
}
