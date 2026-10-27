#include <cstdio>

// Behaviour model of CScriptModuleTimer::DispatchTimes (0x00C1D950): -1 without a target, else the dispatch of
// (remaining, total, frame, argument) mapped to 0 / -1.
struct Target { int m_accept; int m_last[4]; };
struct Group {
    Target* m_pTarget;
    bool InitializeAndDispatch(int a, int b, int c, int d)
    { m_pTarget->m_last[0] = a; m_pTarget->m_last[1] = b; m_pTarget->m_last[2] = c; m_pTarget->m_last[3] = d; return m_pTarget->m_accept != 0; }
};
struct Owner { void* _pad; Target* m_pTarget; };
struct Timer {
    Owner* POwner;
    unsigned int GetCurrentFrame() { return 30; }
    unsigned int GetRemainingTimeSeconds() { return 2; }
    unsigned int GetTotalTimeSeconds() { return 5; }
    int DispatchTimes(int argument)
    {
        Target* target = POwner->m_pTarget;
        if (!target) return -1;
        Group group; group.m_pTarget = target;
        return group.InitializeAndDispatch(GetRemainingTimeSeconds(), GetTotalTimeSeconds(), GetCurrentFrame(), argument) ? 0 : -1;
    }
};

int main()
{

    Owner o; o.m_pTarget = 0; Timer t; t.POwner = &o;
    if (t.DispatchTimes(9) != -1) { std::printf("00c1d950_TEST FAIL\n"); return 1; }
    Target tg; tg.m_accept = 1; o.m_pTarget = &tg;
    if (t.DispatchTimes(9) != 0) { std::printf("00c1d950_TEST FAIL\n"); return 1; }
    if (tg.m_last[0] != 2 || tg.m_last[1] != 5 || tg.m_last[2] != 30 || tg.m_last[3] != 9) { std::printf("00c1d950_TEST FAIL\n"); return 1; }
    tg.m_accept = 0;
    if (t.DispatchTimes(9) != -1) { std::printf("00c1d950_TEST FAIL\n"); return 1; }
    std::printf("00c1d950_TEST PASS\n");
    return 0;
}