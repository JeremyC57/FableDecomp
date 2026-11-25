// Behaviour test for 0x006F5970 CCreatureAction_CombatBoast::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_COMBATBOAST_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_CombatBoast.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_CombatBoast)];
int main(void)
{
    const CCreatureAction_CombatBoast* p = (const CCreatureAction_CombatBoast*)g_obj;
    bool r = p->CCreatureAction_CombatBoast::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_006f5970_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5970_TEST FAIL\n");
    return 1;
}
