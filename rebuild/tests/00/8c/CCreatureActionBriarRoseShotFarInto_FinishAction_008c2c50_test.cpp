#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
static int g_step = 0, g_baseFinish = 0, g_set = 0, g_dtorPtr = 0, g_dtorBase = 0; static CThingCreatureBase* g_c; static CThingCreatureBase* g_t; static const void* g_action;
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } ~CBaseIntelligentPointer() { if (g_set && !g_dtorPtr) g_dtorPtr = ++g_step; } };   // the temporary's member: the first pointer destroyed after SetCurrentAction
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} };
struct CCombatActionBase { virtual void FinishAction() { g_baseFinish = ++g_step; } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xa8 - 0x10]; CBaseIntelligentPointer PTarget; unsigned char _padb0[0x114 - 0xb0];
    ~CCombatActionBase() { if (g_set && !g_dtorBase) g_dtorBase = ++g_step; } };
struct CThingCreatureBase_Calls { bool SetCurrentAction(const CCombatActionBase& action) { g_set = ++g_step; g_action = &action; return true; } };
struct CCreatureAction_BriarRoseShotFarOutof : CCombatActionBase { CIntelligentPointer PTarget; virtual void v00() {}
    CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* creature, CThingCreatureBase* target) { g_c = creature; g_t = target; } };
struct CCreatureAction_BriarRoseShotFarInto : CCombatActionBase { virtual void FinishAction(); };
void CCreatureAction_BriarRoseShotFarInto::FinishAction() { CThingCreatureBase* creature = PCreature.GetPItem(); CThingCreatureBase* target = PTarget.GetPItem(); CCombatActionBase::FinishAction(); ((CThingCreatureBase_Calls*)creature)->SetCurrentAction(CCreatureAction_BriarRoseShotFarOutof(creature, target)); }
int main()
{
    int bad = 0;
    {
        CThingCreatureBase c, t; CCreatureAction_BriarRoseShotFarInto a; a.PCreature.m_pItem = &c; a.PTarget.m_pItem = &t; a.FinishAction();
        if (g_baseFinish != 1 || g_set != 2 || g_dtorPtr != 3 || g_dtorBase != 4 || g_c != &c || g_t != &t || g_action == 0 || sizeof(CCreatureAction_BriarRoseShotFarOutof) != 0x11c) bad = 1;
    }
    if (bad) { std::printf("008c2c50_TEST FAIL\n"); return 1; }
    std::printf("008c2c50_TEST PASS\n");
    return 0;
}