// Behaviour test for 0x0095CA80 CCreatureAction_WaspQueenPrimaryAttack::IsBlockable (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENPRIMARYATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_WaspQueenPrimaryAttack.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_WaspQueenPrimaryAttack)];
int main(void)
{
    const CCreatureAction_WaspQueenPrimaryAttack* p = (const CCreatureAction_WaspQueenPrimaryAttack*)g_obj;
    bool r = p->CCreatureAction_WaspQueenPrimaryAttack::IsBlockable();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_0095ca80_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_0095ca80_TEST FAIL\n");
    return 1;
}
