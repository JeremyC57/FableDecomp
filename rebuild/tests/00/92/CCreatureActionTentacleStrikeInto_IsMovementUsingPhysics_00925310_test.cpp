// Behaviour test for 0x00925310 CCreatureAction_TentacleStrikeInto::IsMovementUsingPhysics (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_TENTACLESTRIKEINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_TentacleStrikeInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_TentacleStrikeInto)];
int main(void)
{
    const CCreatureAction_TentacleStrikeInto* p = (const CCreatureAction_TentacleStrikeInto*)g_obj;
    bool r = p->CCreatureAction_TentacleStrikeInto::IsMovementUsingPhysics();
    int ok = (r == false);
    if (ok) { printf("RB_925310_OK PASS\n"); return 0; }
    printf("RB_925310_OK FAIL\n");
    return 1;
}
