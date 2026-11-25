// Behaviour test for 0x006F5600 CCreatureAction_ScreamerIdle::GetAnimationTransitionInTime (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERIDLE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_ScreamerIdle.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ScreamerIdle)];
int main(void)
{
    const CCreatureAction_ScreamerIdle* p = (const CCreatureAction_ScreamerIdle*)g_obj;
    long r = p->CCreatureAction_ScreamerIdle::GetAnimationTransitionInTime();
    int ok = (r == 15);
    if (ok) { printf("AUTO_TINY_006f5600_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5600_TEST FAIL\n");
    return 1;
}
