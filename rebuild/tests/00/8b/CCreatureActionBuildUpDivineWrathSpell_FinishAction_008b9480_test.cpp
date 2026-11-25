// Behaviour test for 0x008B9480 CCreatureAction_BuildUpDivineWrathSpell::FinishAction (NF427 G2): links the row object, defines CCombatActionBase::FinishAction here (counts calls, records this),
// calls the member directly (qualified, no vftable) and checks exactly one forwarded call with the same object.
#define FABLE_RETAIL_CCREATUREACTION_BUILDUPDIVINEWRATHSPELL_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_BuildUpDivineWrathSpell.h"
#include <stdio.h>
static int g_calls = 0; static const void* g_this = 0;
void CCombatActionBase::FinishAction() { ++g_calls; g_this = this; }
static unsigned char g_obj[sizeof(CCreatureAction_BuildUpDivineWrathSpell)];
int main(void)
{
    CCreatureAction_BuildUpDivineWrathSpell* p = (CCreatureAction_BuildUpDivineWrathSpell*)g_obj;
    p->CCreatureAction_BuildUpDivineWrathSpell::FinishAction();
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CCombatActionBase*>(p));
    if (ok) { printf("PASS_008b9480 PASS\n"); return 0; }
    printf("PASS_008b9480 FAIL\n");
    return 1;
}
