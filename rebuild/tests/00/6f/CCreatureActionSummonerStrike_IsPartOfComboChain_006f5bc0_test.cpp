// Behaviour test for 0x006F5BC0 CCreatureAction_SummonerStrike::IsPartOfComboChain (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERSTRIKE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_SummonerStrike.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SummonerStrike)];
int main(void)
{
    const CCreatureAction_SummonerStrike* p = (const CCreatureAction_SummonerStrike*)g_obj;
    bool r = p->CCreatureAction_SummonerStrike::IsPartOfComboChain();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_006f5bc0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5bc0_TEST FAIL\n");
    return 1;
}
