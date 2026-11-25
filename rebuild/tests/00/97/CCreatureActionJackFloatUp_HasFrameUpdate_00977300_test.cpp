// Behaviour test for 0x00977300 CCreatureAction_JackFloatUp::HasFrameUpdate (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKFLOATUP_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackFloatUp.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackFloatUp)];
int main(void)
{
    const CCreatureAction_JackFloatUp* p = (const CCreatureAction_JackFloatUp*)g_obj;
    bool r = p->CCreatureAction_JackFloatUp::HasFrameUpdate();
    int ok = (r == true);
    if (ok) { printf("RB_977300_OK PASS\n"); return 0; }
    printf("RB_977300_OK FAIL\n");
    return 1;
}
