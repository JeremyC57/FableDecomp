// Behaviour test for 0x008EC700 CCreatureAction_UndeadReturnToEarth::FinishAction: the base finish always runs; the
// creature gets an action with code 50 only once FramesRunning >= TotalTime.
#include <cstdio>
struct CThingCreatureBase;
struct CSoundPair { int a, b; CSoundPair() : a(0), b(0) {} };
static int g_baseFinish = 0, g_setCount = 0, g_lastCode = -1, g_dtors = 0;
struct CCombatActionBase {
    CThingCreatureBase* m_creature;
    int TotalTime, FramesRunning, m_code;
    CCombatActionBase(CThingCreatureBase* c, int code, const CSoundPair&) : m_creature(c), TotalTime(0), FramesRunning(0), m_code(code) {}
    ~CCombatActionBase() { ++g_dtors; }
    void FinishAction() { ++g_baseFinish; }
    CThingCreatureBase* GetCreature() { return m_creature; }
};
struct CThingCreatureBase {
    bool SetCurrentAction(const CCombatActionBase& a) { ++g_setCount; g_lastCode = a.m_code; return true; }
};
struct CIdleUnderground : CCombatActionBase {
    CIdleUnderground(CThingCreatureBase* c) : CCombatActionBase(c, 50, CSoundPair()) {}
};
struct CReturnToEarth : CCombatActionBase {
    CReturnToEarth(CThingCreatureBase* c) : CCombatActionBase(c, 0, CSoundPair()) {}
    void FinishAction()
    {
        CCombatActionBase::FinishAction();
        if (FramesRunning >= TotalTime)
            GetCreature()->SetCurrentAction(CIdleUnderground(GetCreature()));
    }
};
int main()
{
    CThingCreatureBase creature;
    CReturnToEarth a(&creature);
    a.TotalTime = 10; a.FramesRunning = 5;
    a.FinishAction();
    if (g_baseFinish != 1 || g_setCount != 0) { std::printf("008ec700_TEST FAIL\n"); return 1; }
    a.FramesRunning = 10;
    a.FinishAction();
    if (g_baseFinish != 2 || g_setCount != 1 || g_lastCode != 50 || g_dtors != 1) { std::printf("008ec700_TEST FAIL\n"); return 1; }
    std::printf("008ec700_TEST PASS\n");
    return 0;
}
