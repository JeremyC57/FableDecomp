// Behaviour test for 0x009629D0 CCreatureAction_IceTrollIcicleAssaultInto::IsStrikeAction (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEASSAULTINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_IceTrollIcicleAssaultInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_IceTrollIcicleAssaultInto)];
int main(void)
{
    const CCreatureAction_IceTrollIcicleAssaultInto* p = (const CCreatureAction_IceTrollIcicleAssaultInto*)g_obj;
    bool r = p->CCreatureAction_IceTrollIcicleAssaultInto::IsStrikeAction();
    int ok = (r == true);
    if (ok) { printf("RB_9629d0_OK PASS\n"); return 0; }
    printf("RB_9629d0_OK FAIL\n");
    return 1;
}
