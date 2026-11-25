// Behaviour test for 0x009536C0 CCreatureAction_DragonFireBlastBase::IsPreventingLookTurning (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_DragonFireBlastBase.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_DragonFireBlastBase)];
int main(void)
{
    const CCreatureAction_DragonFireBlastBase* p = (const CCreatureAction_DragonFireBlastBase*)g_obj;
    bool r = p->CCreatureAction_DragonFireBlastBase::IsPreventingLookTurning();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_009536c0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_009536c0_TEST FAIL\n");
    return 1;
}
