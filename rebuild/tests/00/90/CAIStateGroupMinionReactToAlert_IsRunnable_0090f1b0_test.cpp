// Behaviour test for CAIStateGroup_MinionReactToAlert::IsRunnable 0x0090F1B0: the state group is runnable exactly when FindAlertEvent finds an event.
#include <cstdio>
struct CMessageEvent { int id; };
struct CAIStateGroup_MinionReactToAlert {
    const CMessageEvent* Found;
    const CMessageEvent* FindAlertEvent() { return Found; }
    bool IsRunnable() { if (FindAlertEvent()) return true; return false; }
};
int main()
{
    CMessageEvent e = { 3 };
    CAIStateGroup_MinionReactToAlert g; g.Found = 0;
    if (g.IsRunnable()) { std::printf("0090f1b0_TEST FAIL\n"); return 1; }
    g.Found = &e;
    if (!g.IsRunnable()) { std::printf("0090f1b0_TEST FAIL\n"); return 1; }
    std::printf("0090f1b0_TEST PASS\n");
    return 0;
}