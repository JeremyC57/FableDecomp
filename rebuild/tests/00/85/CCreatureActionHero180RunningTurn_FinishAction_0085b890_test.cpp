// Behaviour test for 0x0085B890 CCreatureActionHero180RunningTurn::FinishAction (NF427 G2): links the row object, defines CCombatActionBase::FinishAction here (counts calls, records this),
// calls the member directly (qualified, no vftable) and checks exactly one forwarded call with the same object.
#define FABLE_RETAIL_CCREATUREACTIONHERO180RUNNINGTURN_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureActionHero180RunningTurn.h"
#include <stdio.h>
static int g_calls = 0; static const void* g_this = 0;
void CCombatActionBase::FinishAction() { ++g_calls; g_this = this; }
static unsigned char g_obj[sizeof(CCreatureActionHero180RunningTurn)];
int main(void)
{
    CCreatureActionHero180RunningTurn* p = (CCreatureActionHero180RunningTurn*)g_obj;
    p->CCreatureActionHero180RunningTurn::FinishAction();
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CCombatActionBase*>(p));
    if (ok) { printf("PASS_0085b890 PASS\n"); return 0; }
    printf("PASS_0085b890 FAIL\n");
    return 1;
}
