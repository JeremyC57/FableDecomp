// Behaviour test for 00da0c40 (CQ_MinionCampScript::RunSpawnPointControl): while in stage 3, whenever the minion count
// is below the maximum and the spawn timer has run out, a spawn point is chosen (right side if it holds the furthest
// point, else left), the timer is re-armed with a period between min and max unless this is the last minion, and a
// spawn is flagged.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_seed = 0;
static int FakeRand() { g_seed = (g_seed * 1103 + 12345) % 32768; return g_seed; }   /* MSVCRT-range rand */
struct Model {
    long stage, minions, maxMinions, timer; float minP, maxP; bool shouldSpawn; int frames, termAt, picksLeft, picksRight, rearms;
    bool rightFurthest; long lastPeriod;
    bool Term() { return frames >= termAt; }
    void Run()
    {
        while (stage == 3) {
            ++frames; if (timer > 0) --timer;
            if (Term()) return;
            if (minions < maxMinions && timer <= 0) {
                if (Term()) return;
                if (!rightFurthest) { if (Term()) return; ++picksLeft; } else { if (Term()) return; ++picksRight; }
                if (minions < maxMinions - 1) {
                    if (Term()) return;
                    float t = FakeRand() * (1.0f / 32767.0f);
                    lastPeriod = (long)(minP + (maxP - minP) * t); timer = lastPeriod; ++rearms;
                }
                shouldSpawn = true; ++minions;
                if (minions >= maxMinions) stage = 4;
            }
        }
        if (Term()) return;
    }
};
int main()
{
    Model m; std::memset(&m, 0, sizeof m); m.stage = 3; m.maxMinions = 3; m.minP = 2.0f; m.maxP = 4.0f; m.termAt = 1000; m.rightFurthest = true;
    m.Run();
    int ok = m.minions == 3 && m.picksRight == 3 && m.picksLeft == 0 && m.rearms == 2 && m.shouldSpawn && m.lastPeriod >= 2 && m.lastPeriod <= 4 && m.stage == 4;
    Model n; std::memset(&n, 0, sizeof n); n.stage = 2; n.Run();
    if (n.frames != 0 || n.shouldSpawn) ok = 0;
    if (!ok) { std::printf("00da0c40_TEST FAIL\n"); return 1; }
    std::printf("00da0c40_TEST PASS\n");
    return 0;
}