#include <cstdio>
// Behaviour model of CTCGuard::CTCGuard (0x007B2DB0): initial counters.
struct Guard {
    void* def; long bribe; long pendingAdded, lastBribe, severity, lastCrime, lastApology; bool waiting;
    Guard() : def(0)
    {
        lastCrime = -0x0CCCCCCC; lastApology = -0x0CCCCCCC; bribe = 0; pendingAdded = 0; severity = 0; waiting = false;
        lastBribe = -0x40000000;
    }
};
int main()
{
    Guard g;
    if (g.def || g.bribe || g.pendingAdded || g.severity || g.waiting || g.lastCrime != -214748364L || g.lastApology != -214748364L
        || g.lastBribe != -1073741824L) { std::printf("007b2db0_TEST FAIL\n"); return 1; }
    std::printf("007b2db0_TEST PASS\n");
    return 0;
}
