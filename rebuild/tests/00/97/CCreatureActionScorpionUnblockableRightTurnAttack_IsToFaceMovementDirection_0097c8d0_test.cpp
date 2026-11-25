// Behaviour test for 0x0097C8D0 CCreatureAction_ScorpionUnblockableRightTurnAttack::IsToFaceMovementDirection (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONUNBLOCKABLERIGHTTURNATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_ScorpionUnblockableRightTurnAttack.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ScorpionUnblockableRightTurnAttack)];
int main(void)
{
    const CCreatureAction_ScorpionUnblockableRightTurnAttack* p = (const CCreatureAction_ScorpionUnblockableRightTurnAttack*)g_obj;
    bool r = p->CCreatureAction_ScorpionUnblockableRightTurnAttack::IsToFaceMovementDirection();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_0097c8d0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_0097c8d0_TEST FAIL\n");
    return 1;
}
