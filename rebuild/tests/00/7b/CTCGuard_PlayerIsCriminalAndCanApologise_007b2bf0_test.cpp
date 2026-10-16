#include <cstdio>
// Behaviour model of CTCGuard::PlayerIsCriminalAndCanApologise (0x007B2BF0).
struct Village { bool criminal; long minAgo; int asked;
    bool IsHeroCriminal() { return criminal; }
    bool CanApologise(long t) { ++asked; return t >= minAgo; } };
struct Guard { Village* village; long timeAgo;
    bool Run()
    {
        Village* v = village;
        if (v) { if (v->IsHeroCriminal()) { long t = timeAgo; if (t > 0) return v->CanApologise(t); } }
        return false;
    } };
int main()
{
    Village v; v.criminal = true; v.minAgo = 5; v.asked = 0;
    Guard g; g.village = &v; g.timeAgo = 10;
    if (!g.Run() || v.asked != 1) { std::printf("007b2bf0_TEST FAIL\n"); return 1; }
    g.timeAgo = 0; if (g.Run() || v.asked != 1) { std::printf("007b2bf0_TEST FAIL\n"); return 1; }
    g.timeAgo = 3; if (g.Run() || v.asked != 2) { std::printf("007b2bf0_TEST FAIL\n"); return 1; }
    v.criminal = false; g.timeAgo = 10; if (g.Run()) { std::printf("007b2bf0_TEST FAIL\n"); return 1; }
    g.village = 0; if (g.Run()) { std::printf("007b2bf0_TEST FAIL\n"); return 1; }
    std::printf("007b2bf0_TEST PASS\n");
    return 0;
}