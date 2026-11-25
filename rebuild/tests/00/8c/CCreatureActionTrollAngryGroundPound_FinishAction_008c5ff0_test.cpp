// Behaviour test for 0x008C5FF0 CCreatureAction_TrollAngryGroundPound::FinishAction (NF427 G2): links the row object, defines CCombatActionBase::FinishAction here (counts calls, records this),
// calls the member directly (qualified, no vftable) and checks exactly one forwarded call with the same object.
#define FABLE_RETAIL_CCREATUREACTION_TROLLANGRYGROUNDPOUND_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS
#include "engine/CCreatureAction_TrollAngryGroundPound.h"
#include <stdio.h>
static int g_calls = 0; static const void* g_this = 0;
void CCombatActionBase::FinishAction() { ++g_calls; g_this = this; }
static unsigned char g_obj[sizeof(CCreatureAction_TrollAngryGroundPound)];
int main(void)
{
    CCreatureAction_TrollAngryGroundPound* p = (CCreatureAction_TrollAngryGroundPound*)g_obj;
    p->CCreatureAction_TrollAngryGroundPound::FinishAction();
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CCombatActionBase*>(p));
    if (ok) { printf("PASS_008c5ff0 PASS\n"); return 0; }
    printf("PASS_008c5ff0 FAIL\n");
    return 1;
}
