// Behaviour test for 0x006F5640 CCreatureAction_ScreamerAdvance::GetAnimationTransitionInTime (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEFORWARD_VIRTUAL_MODEL
#include "engine/CCreatureAction_ScreamerAdvance.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ScreamerAdvance)];
int main(void)
{
    const CCreatureAction_ScreamerAdvance* p = (const CCreatureAction_ScreamerAdvance*)g_obj;
    long r = p->CCreatureAction_ScreamerAdvance::GetAnimationTransitionInTime();
    int ok = (r == 30);
    if (ok) { printf("AUTO_TINY_006f5640_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5640_TEST FAIL\n");
    return 1;
}
