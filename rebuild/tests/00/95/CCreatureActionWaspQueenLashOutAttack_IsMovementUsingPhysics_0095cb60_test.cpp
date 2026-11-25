// Behaviour test for 0x0095CB60 CCreatureAction_WaspQueenLashOutAttack::IsMovementUsingPhysics (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENLASHOUTATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_WaspQueenLashOutAttack.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_WaspQueenLashOutAttack)];
int main(void)
{
    const CCreatureAction_WaspQueenLashOutAttack* p = (const CCreatureAction_WaspQueenLashOutAttack*)g_obj;
    bool r = p->CCreatureAction_WaspQueenLashOutAttack::IsMovementUsingPhysics();
    int ok = (r == false);
    if (ok) { printf("RB_95cb60_OK PASS\n"); return 0; }
    printf("RB_95cb60_OK FAIL\n");
    return 1;
}
