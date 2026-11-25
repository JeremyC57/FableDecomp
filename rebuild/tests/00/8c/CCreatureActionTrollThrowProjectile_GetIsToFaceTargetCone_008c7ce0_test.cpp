// Behaviour test for 0x008C7CE0 CCreatureAction_TrollThrowProjectile::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_TrollThrowProjectile.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_TrollThrowProjectile)];
int main(void)
{
    const CCreatureAction_TrollThrowProjectile* p = (const CCreatureAction_TrollThrowProjectile*)g_obj;
    float r = p->CCreatureAction_TrollThrowProjectile::GetIsToFaceTargetCone();
    int ok = (r == 1.0f);
    if (ok) { printf("PASS_008c7ce0 PASS\n"); return 0; }
    printf("PASS_008c7ce0 FAIL\n");
    return 1;
}
