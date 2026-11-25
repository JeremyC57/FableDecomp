// Behaviour test for 0x006F5940 CCreatureAction_CombatIdle::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_CombatIdle.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_CombatIdle)];
int main(void)
{
    const CCreatureAction_CombatIdle* p = (const CCreatureAction_CombatIdle*)g_obj;
    bool r = p->CCreatureAction_CombatIdle::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_006f5940_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5940_TEST FAIL\n");
    return 1;
}
