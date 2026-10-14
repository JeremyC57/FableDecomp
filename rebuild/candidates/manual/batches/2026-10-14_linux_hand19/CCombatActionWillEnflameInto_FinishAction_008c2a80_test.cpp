#include <cstdio>

// Model of CCombatAction_WillEnflameInto::FinishAction @ 0x008C2A80
struct CCreatureActionBase;
static int g_baseFinish = 0; static int g_ctor = 0, g_dtor = 0; static long g_level = -1; static void* g_ctorCreature = 0; static int g_ctorAfterBase = -1;
static const void* g_setAction = 0; static int g_setLiveTemps = -1;
struct CThingCreatureBase { bool SetCurrentAction(const CCreatureActionBase& a) { g_setAction = &a; g_setLiveTemps = g_ctor - g_dtor; return true; } };
struct CCreatureAction_CastWillEnflameSpell {
    unsigned char _body[0x11c];
    CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* c, long level) { ++g_ctor; g_ctorCreature = c; g_level = level; g_ctorAfterBase = g_baseFinish; }
    ~CCreatureAction_CastWillEnflameSpell() { ++g_dtor; }
    operator const CCreatureActionBase&() const { return *reinterpret_cast<const CCreatureActionBase*>(this); }
};
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCombatActionBase { virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0x114 - 0x10]; void FinishAction() { ++g_baseFinish; } };
struct CCombatAction_WillEnflameInto : CCombatActionBase {
    long Level;
    void FinishAction() { CThingCreatureBase* creature = PCreature.GetPItem(); CCombatActionBase::FinishAction(); creature->SetCurrentAction(CCreatureAction_CastWillEnflameSpell(creature, Level)); }
};
int main()
{
    CThingCreatureBase c; CCombatAction_WillEnflameInto a; a.PCreature.m_pItem = &c; a.Level = 3;
    a.FinishAction();
    bool ok = g_baseFinish == 1 && g_ctor == 1 && g_dtor == 1 && g_level == 3 && g_ctorCreature == &c && g_ctorAfterBase == 1 && g_setAction != 0 && g_setLiveTemps == 1;
    if (!ok) { std::printf("008c2a80_TEST FAIL\n"); return 1; }
    std::printf("008c2a80_TEST PASS\n"); return 0;
}
