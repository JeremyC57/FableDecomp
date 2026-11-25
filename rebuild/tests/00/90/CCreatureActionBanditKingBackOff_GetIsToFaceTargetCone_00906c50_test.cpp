// Behaviour test for 0x00906C50 CCreatureAction_BanditKingBackOff::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGBACKOFF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_RTTI_DECLS
#include "engine/CCreatureAction_BanditKingBackOff.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_BanditKingBackOff)];
int main(void)
{
    const CCreatureAction_BanditKingBackOff* p = (const CCreatureAction_BanditKingBackOff*)g_obj;
    float r = p->CCreatureAction_BanditKingBackOff::GetIsToFaceTargetCone();
    int ok = (r == 0.125f);
    if (ok) { printf("PASS_00906c50 PASS\n"); return 0; }
    printf("PASS_00906c50 FAIL\n");
    return 1;
}
