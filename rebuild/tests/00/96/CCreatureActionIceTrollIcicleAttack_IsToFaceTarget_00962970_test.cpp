// Behaviour test for 0x00962970 CCreatureAction_IceTrollIcicleAttack::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_IceTrollIcicleAttack.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_IceTrollIcicleAttack)];
int main(void)
{
    const CCreatureAction_IceTrollIcicleAttack* p = (const CCreatureAction_IceTrollIcicleAttack*)g_obj;
    bool r = p->CCreatureAction_IceTrollIcicleAttack::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_962970_OK PASS\n"); return 0; }
    printf("RB_962970_OK FAIL\n");
    return 1;
}
