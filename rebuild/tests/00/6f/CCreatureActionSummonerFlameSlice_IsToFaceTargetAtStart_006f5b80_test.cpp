// Behaviour test for 0x006F5B80 CCreatureAction_SummonerFlameSlice::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERFLAMESLICE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_SummonerFlameSlice.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SummonerFlameSlice)];
int main(void)
{
    const CCreatureAction_SummonerFlameSlice* p = (const CCreatureAction_SummonerFlameSlice*)g_obj;
    bool r = p->CCreatureAction_SummonerFlameSlice::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_006f5b80_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5b80_TEST FAIL\n");
    return 1;
}
