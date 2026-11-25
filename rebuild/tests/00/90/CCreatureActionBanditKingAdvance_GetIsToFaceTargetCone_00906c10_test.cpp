// Behaviour test for 0x00906C10 CCreatureAction_BanditKingAdvance::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEFORWARD_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_STRAFEFORWARD_RTTI_DECLS
#include "engine/CCreatureAction_BanditKingAdvance.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_BanditKingAdvance)];
int main(void)
{
    const CCreatureAction_BanditKingAdvance* p = (const CCreatureAction_BanditKingAdvance*)g_obj;
    float r = p->CCreatureAction_BanditKingAdvance::GetIsToFaceTargetCone();
    int ok = (r == 0.125f);
    if (ok) { printf("PASS_00906c10 PASS\n"); return 0; }
    printf("PASS_00906c10 FAIL\n");
    return 1;
}
