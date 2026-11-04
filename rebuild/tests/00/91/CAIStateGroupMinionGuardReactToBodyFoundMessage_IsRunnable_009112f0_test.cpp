// Behaviour test for CAIStateGroup_MinionGuardReactToBodyFoundMessage::IsRunnable 0x009112F0: the state group is runnable exactly when FindBodyFoundEvent finds an event.
#include <cstdio>
struct CMessageEvent { int id; };
struct CAIStateGroup_MinionGuardReactToBodyFoundMessage {
    const CMessageEvent* Found;
    const CMessageEvent* FindBodyFoundEvent() { return Found; }
    bool IsRunnable() { if (FindBodyFoundEvent()) return true; return false; }
};
int main()
{
    CMessageEvent e = { 3 };
    CAIStateGroup_MinionGuardReactToBodyFoundMessage g; g.Found = 0;
    if (g.IsRunnable()) { std::printf("009112f0_TEST FAIL\n"); return 1; }
    g.Found = &e;
    if (!g.IsRunnable()) { std::printf("009112f0_TEST FAIL\n"); return 1; }
    std::printf("009112f0_TEST PASS\n");
    return 0;
}