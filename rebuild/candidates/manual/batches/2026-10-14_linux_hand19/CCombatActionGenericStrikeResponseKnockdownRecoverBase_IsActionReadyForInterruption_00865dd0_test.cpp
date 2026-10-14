#include <cstdio>

// Model of CCombatAction_GenericStrikeResponseKnockdownRecoverBase::IsActionReadyForInterruption @ 0x00865DD0

struct CThingCreatureBase { unsigned char _pad[0x10]; unsigned char CombatState; };
static int g_baseCalls = 0; static bool g_baseResult = false;
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCombatActionBase {
    virtual void v00() {}
    unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; long TotalTime; long ActionTime; long AnimDelayTime; long FramesRunning;
    unsigned char _pad20[0xdc - 0x20]; bool m_respondsInState2; unsigned char _paddd[0x114 - 0xdd];
    bool IsActionReadyForInterruption() const { ++g_baseCalls; return g_baseResult; }
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    bool IsActionReadyForInterruption() const {
        if (PCreature.GetPItem()->CombatState == 2) { if (m_respondsInState2) return true; }
        return CCombatActionBase::IsActionReadyForInterruption();
    }
};
int main()
{
    CThingCreatureBase c; CCombatAction_GenericStrikeResponseKnockdownRecoverBase a; a.PCreature.m_pItem = &c;
    c.CombatState = 2; a.m_respondsInState2 = true;
    if (!a.IsActionReadyForInterruption() || g_baseCalls != 0) { std::printf("00865dd0_TEST FAIL\n"); return 1; }
    a.m_respondsInState2 = false; g_baseResult = true;
    if (!a.IsActionReadyForInterruption() || g_baseCalls != 1) { std::printf("00865dd0_TEST FAIL\n"); return 1; }
    c.CombatState = 1; a.m_respondsInState2 = true; g_baseResult = false;
    if (a.IsActionReadyForInterruption() || g_baseCalls != 2) { std::printf("00865dd0_TEST FAIL\n"); return 1; }
    std::printf("00865dd0_TEST PASS\n"); return 0;
}
