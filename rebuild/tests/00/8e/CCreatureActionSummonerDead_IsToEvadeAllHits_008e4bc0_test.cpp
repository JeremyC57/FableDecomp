// Behaviour test for 0x008E4BC0 CCreatureAction_SummonerDead::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERDEAD_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_SummonerDead.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SummonerDead)];
int main(void)
{
    const CCreatureAction_SummonerDead* p = (const CCreatureAction_SummonerDead*)g_obj;
    bool r = p->CCreatureAction_SummonerDead::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("RB_8e4bc0_OK PASS\n"); return 0; }
    printf("RB_8e4bc0_OK FAIL\n");
    return 1;
}
