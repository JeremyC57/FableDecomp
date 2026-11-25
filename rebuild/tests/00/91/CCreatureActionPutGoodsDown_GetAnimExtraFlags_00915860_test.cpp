// Behaviour test for 0x00915860 CCreatureAction_PutGoodsDown::GetAnimExtraFlags (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_PUTGOODSDOWN_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_PutGoodsDown.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_PutGoodsDown)];
int main(void)
{
    const CCreatureAction_PutGoodsDown* p = (const CCreatureAction_PutGoodsDown*)g_obj;
    unsigned long r = p->CCreatureAction_PutGoodsDown::GetAnimExtraFlags();
    int ok = (r == 8);
    if (ok) { printf("RB_915860_OK PASS\n"); return 0; }
    printf("RB_915860_OK FAIL\n");
    return 1;
}
