// Behaviour test for 0x0087C840 CCreatureAction_InterruptableMidAttack::FrameUpdate (NF427 G2): links the row object, defines CCombatActionBase::FrameUpdate here (counts calls, records this),
// calls the member directly (qualified, no vftable) and checks exactly one forwarded call with the same object.
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEMIDATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_InterruptableMidAttack.h"
#include <stdio.h>
static int g_calls = 0; static const void* g_this = 0;
void CCombatActionBase::FrameUpdate() { ++g_calls; g_this = this; }
static unsigned char g_obj[sizeof(CCreatureAction_InterruptableMidAttack)];
int main(void)
{
    CCreatureAction_InterruptableMidAttack* p = (CCreatureAction_InterruptableMidAttack*)g_obj;
    p->CCreatureAction_InterruptableMidAttack::FrameUpdate();
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CCombatActionBase*>(p));
    if (ok) { printf("PASS_0087c840 PASS\n"); return 0; }
    printf("PASS_0087c840 FAIL\n");
    return 1;
}
