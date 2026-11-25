// Behaviour test for 0x009773A0 CCreatureAction_JackFall::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKFALL_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackFall.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackFall)];
int main(void)
{
    const CCreatureAction_JackFall* p = (const CCreatureAction_JackFall*)g_obj;
    bool r = p->CCreatureAction_JackFall::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("RB_9773a0_OK PASS\n"); return 0; }
    printf("RB_9773a0_OK FAIL\n");
    return 1;
}
