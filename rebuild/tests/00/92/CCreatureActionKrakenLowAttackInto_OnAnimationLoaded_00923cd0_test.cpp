#include <cstdio>
static int g_baseCalls;
struct CCombatActionBase { virtual void v00() { } virtual void OnAnimationLoaded() { ++g_baseCalls; } unsigned char _pad04[0x0c]; long TotalTime; long ActionTime; };
struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase { virtual void OnAnimationLoaded(); };
void CCreatureAction_KrakenLowAttackInto::OnAnimationLoaded() { CCombatActionBase::OnAnimationLoaded(); ActionTime = TotalTime * 3 / 4; }
int main()
{
    CCreatureAction_KrakenLowAttackInto a; a.TotalTime = 101; a.ActionTime = 0; a.OnAnimationLoaded();
    if (g_baseCalls != 1 || a.ActionTime != 75 || (char*)&a.ActionTime - (char*)&a != 0x14) { std::printf("00923cd0_TEST FAIL\n"); return 1; }
    a.TotalTime = -101; a.OnAnimationLoaded(); if (a.ActionTime != -75) { std::printf("00923cd0_TEST FAIL\n"); return 1; }
    std::printf("00923cd0_TEST PASS\n");
    return 0;
}