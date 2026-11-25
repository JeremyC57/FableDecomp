// Behaviour test for 0x0095CAA0 CCreatureAction_WaspQueenPrimaryAttack::IsMovementUsingPhysics (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
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
    bool r = p->CCreatureAction_WaspQueenPrimaryAttack::IsMovementUsingPhysics();
    int ok = (r == false);
    if (ok) { printf("RB_95caa0_OK PASS\n"); return 0; }
    printf("RB_95caa0_OK FAIL\n");
    return 1;
}
