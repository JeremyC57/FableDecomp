// Behaviour test for 0x008C5370 CCreatureAction_WaspQueenDie::FinishAction: loops 1..4 restart the action (code 250)
// carrying the new loop count; the fifth loop kills the creature instead.
#include <cstdio>
struct CThingCreatureBase;
struct CSoundPair { int a, b; CSoundPair() : a(0), b(0) {} };
static int g_base = 0, g_kills = 0, g_restarts = 0, g_lastLoop = -1, g_lastCode = -1;
struct CCombatActionBase {
    CThingCreatureBase* m_creature; int m_code;
    CCombatActionBase(CThingCreatureBase* c, int code, const CSoundPair&) : m_creature(c), m_code(code) {}
    void FinishAction() { ++g_base; }
    CThingCreatureBase* GetPItem() { return m_creature; }
};
struct CWaspQueenDie;
struct CThingCreatureBase {
    void Kill(bool) { ++g_kills; }
    bool SetCurrentAction(const CWaspQueenDie& a);
};
struct CWaspQueenDie : CCombatActionBase {
    int LoopNum;
    CWaspQueenDie(CThingCreatureBase* c, int n) : CCombatActionBase(c, 250, CSoundPair()), LoopNum(n) {}
    void FinishAction()
    {
        CCombatActionBase::FinishAction();
        if (++LoopNum == 5) { GetPItem()->Kill(true); return; }
        CThingCreatureBase* creature = GetPItem();
        GetPItem()->SetCurrentAction(CWaspQueenDie(creature, LoopNum));
    }
};
bool CThingCreatureBase::SetCurrentAction(const CWaspQueenDie& a) { ++g_restarts; g_lastLoop = a.LoopNum; g_lastCode = a.m_code; return true; }
int main()
{
    CThingCreatureBase creature;
    CWaspQueenDie a(&creature, 0);
    for (int i = 1; i <= 4; ++i) {
        a.FinishAction();
        if (g_restarts != i || g_lastLoop != i || g_lastCode != 250 || g_kills != 0) { std::printf("008c5370_TEST FAIL\n"); return 1; }
    }
    a.FinishAction();
    if (g_kills != 1 || g_restarts != 4 || g_base != 5 || a.LoopNum != 5) { std::printf("008c5370_TEST FAIL\n"); return 1; }
    std::printf("008c5370_TEST PASS\n");
    return 0;
}
