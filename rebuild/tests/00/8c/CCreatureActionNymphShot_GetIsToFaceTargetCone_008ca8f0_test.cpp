// Behaviour test for 0x008CA8F0 CCreatureAction_NymphShot::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_NYMPHSHOT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_NymphShot.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_NymphShot)];
int main(void)
{
    const CCreatureAction_NymphShot* p = (const CCreatureAction_NymphShot*)g_obj;
    float r = p->CCreatureAction_NymphShot::GetIsToFaceTargetCone();
    int ok = (r == 1.0f);
    if (ok) { printf("PASS_008ca8f0 PASS\n"); return 0; }
    printf("PASS_008ca8f0 FAIL\n");
    return 1;
}
