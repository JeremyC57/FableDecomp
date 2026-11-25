// Behaviour test for 0x0096C740 CCreatureAction_ThunderLightningStorm::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_THUNDERLIGHTNINGSTORM_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_ThunderLightningStorm.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ThunderLightningStorm)];
int main(void)
{
    const CCreatureAction_ThunderLightningStorm* p = (const CCreatureAction_ThunderLightningStorm*)g_obj;
    bool r = p->CCreatureAction_ThunderLightningStorm::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("RB_96c740_OK PASS\n"); return 0; }
    printf("RB_96c740_OK FAIL\n");
    return 1;
}
