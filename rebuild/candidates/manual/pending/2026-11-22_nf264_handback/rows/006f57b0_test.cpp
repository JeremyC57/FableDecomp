// Behaviour test for 0x006F57B0 CCombatAction_StrikeResponseScorpionKing::AllowsCombinationAnims (links the REAL row object, NF264; the HEAD test
// was a standalone copy of the free stand-in). A qualified (non-virtual) call on raw storage: the body ignores this.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_DEFAULTCTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_PROTOTYPE
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_ALLOWSCOMBINATIONANIMS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_GenericStrikeResponseBase.h"
#include "engine/CCombatAction_StrikeResponseScorpionKing.h"

#include <stdio.h>
int main()
{
    unsigned long storage[0x148 / 4] = { 0 };
    const CCombatAction_StrikeResponseScorpionKing* action = (const CCombatAction_StrikeResponseScorpionKing*)storage;
    bool r = action->CCombatAction_StrikeResponseScorpionKing::AllowsCombinationAnims();
    storage[0x74 / 4] = 0xffffffff;
    bool r2 = action->CCombatAction_StrikeResponseScorpionKing::AllowsCombinationAnims();
    if (r == true && r2 == true) { printf("AUTO_TINY_006f57b0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f57b0_TEST FAIL\n");
    return 1;
}
