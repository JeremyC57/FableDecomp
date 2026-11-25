// Behaviour test for 0x009252A0 CCreatureAction_TentacleRise::IsMovementUsingPhysics (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_TENTACLERISE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_TentacleRise.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_TentacleRise)];
int main(void)
{
    const CCreatureAction_TentacleRise* p = (const CCreatureAction_TentacleRise*)g_obj;
    bool r = p->CCreatureAction_TentacleRise::IsMovementUsingPhysics();
    int ok = (r == false);
    if (ok) { printf("RB_9252a0_OK PASS\n"); return 0; }
    printf("RB_9252a0_OK FAIL\n");
    return 1;
}
