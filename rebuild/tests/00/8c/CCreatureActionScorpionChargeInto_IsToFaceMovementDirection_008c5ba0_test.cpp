// Behaviour test for 0x008C5BA0 CCreatureAction_ScorpionChargeInto::IsToFaceMovementDirection (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_ScorpionChargeInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ScorpionChargeInto)];
int main(void)
{
    const CCreatureAction_ScorpionChargeInto* p = (const CCreatureAction_ScorpionChargeInto*)g_obj;
    bool r = p->CCreatureAction_ScorpionChargeInto::IsToFaceMovementDirection();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_008c5ba0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008c5ba0_TEST FAIL\n");
    return 1;
}
