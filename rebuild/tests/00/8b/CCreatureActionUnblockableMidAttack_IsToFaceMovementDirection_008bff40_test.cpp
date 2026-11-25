// Behaviour test for 0x008BFF40 CCreatureAction_UnblockableMidAttack::IsToFaceMovementDirection (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_UNBLOCKABLEMIDATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_UnblockableMidAttack.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_UnblockableMidAttack)];
int main(void)
{
    const CCreatureAction_UnblockableMidAttack* p = (const CCreatureAction_UnblockableMidAttack*)g_obj;
    bool r = p->CCreatureAction_UnblockableMidAttack::IsToFaceMovementDirection();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_008bff40_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008bff40_TEST FAIL\n");
    return 1;
}
