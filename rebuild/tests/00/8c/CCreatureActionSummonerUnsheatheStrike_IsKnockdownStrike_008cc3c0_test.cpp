// Behaviour test for 0x008CC3C0 CCreatureAction_SummonerUnsheatheStrike::IsKnockdownStrike (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERUNSHEATHESTRIKE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS_VIRTUAL_MODEL
#include "engine/CCreatureAction_SummonerUnsheatheStrike.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SummonerUnsheatheStrike)];
int main(void)
{
    const CCreatureAction_SummonerUnsheatheStrike* p = (const CCreatureAction_SummonerUnsheatheStrike*)g_obj;
    bool r = p->CCreatureAction_SummonerUnsheatheStrike::IsKnockdownStrike();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008cc3c0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008cc3c0_TEST FAIL\n");
    return 1;
}
