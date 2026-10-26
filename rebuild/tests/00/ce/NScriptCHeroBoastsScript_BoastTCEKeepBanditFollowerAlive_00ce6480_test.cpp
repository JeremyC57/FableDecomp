// behaviour test for 0x00CE6480 NScript::CHeroBoastsScript::BoastTCEKeepBanditFollowerAlive: frames pass until the quest resolves; failure fails boast 0x24, completion completes it only with TCEKeepBanditFollowerAlive set, and a terminating script reports nothing
#include <cstdio>
struct World { int failAt, completeAt, frame, terminateAt; bool flag; int completed, failed; long boast; };
static bool Terminating(const World& w) { return w.terminateAt >= 0 && w.frame >= w.terminateAt; }
static void Run(World& w)                                 // the function under test
{
    while (!Terminating(w)) {
        if (w.failAt >= 0 && w.frame >= w.failAt) { if (Terminating(w)) return; w.failed++; w.boast = 0x24; return; }
        bool done = w.completeAt >= 0 && w.frame >= w.completeAt;
        if (done && w.flag) { if (Terminating(w)) return; w.completed++; w.boast = 0x24; return; }
        if (done) { if (Terminating(w)) return; w.failed++; w.boast = 0x24; return; }
        w.frame++;
    }
}
static World Make(int failAt, int completeAt, bool flag, int terminateAt)
{
    World w; w.failAt = failAt; w.completeAt = completeAt; w.flag = flag; w.terminateAt = terminateAt;
    w.frame = 0; w.completed = 0; w.failed = 0; w.boast = 0; return w;
}
int main()
{
    int fail = 0;
    World a = Make(-1, 3, true, -1); Run(a); if (a.completed != 1 || a.failed != 0 || a.frame != 3 || a.boast != 0x24) fail |= 1;
    World b = Make(-1, 2, false, -1); Run(b); if (b.completed != 0 || b.failed != 1) fail |= 2;
    World c = Make(4, -1, true, -1); Run(c); if (c.failed != 1 || c.frame != 4) fail |= 4;
    World d = Make(-1, -1, true, 5); Run(d); if (d.failed != 0 || d.completed != 0 || d.frame != 5) fail |= 8;
    if (fail) { std::printf("00ce6480_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ce6480_TEST PASS\n");
    return 0;
}