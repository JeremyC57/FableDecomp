// Behaviour test for 0x008C5C50 CCreatureAction_WaspQueenDiveAttack::IsToRetainSafeDistance (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENDIVEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_WaspQueenDiveAttack.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_WaspQueenDiveAttack)];
int main(void)
{
    const CCreatureAction_WaspQueenDiveAttack* p = (const CCreatureAction_WaspQueenDiveAttack*)g_obj;
    bool r = p->CCreatureAction_WaspQueenDiveAttack::IsToRetainSafeDistance();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_008c5c50_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008c5c50_TEST FAIL\n");
    return 1;
}
