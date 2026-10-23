#include <cstdio>
// behaviour test: CRingFighter::Main -- wait for the round, cheer/boo bookkeeping per frame, time-out exit, and the
// loser path once health falls to the threshold (round flag reset, ring cleared, walk to marker, removal)
static int g_frame = 0, g_removed = 0, g_cleared = 0, g_moves = 0, g_targetCleared = 0;
struct Club { bool RoundStarted, TimeOut, RingCleared, TylerInFight; float CheerNumber, BooNumber; };
struct Fighter {
    Club* club; float health; long threshold; int heroHits; int fighterHits; float dist;
    bool IsDone() { return g_frame > 200; }
    void Frame() { ++g_frame; if (g_frame == 3) club->RoundStarted = true; health -= 10.0f; }
    void Main()
    {
        Frame();
        if (IsDone()) return;
        while (!club->RoundStarted) { Frame(); if (IsDone()) return; }
        while (!IsDone()) {
            if (heroHits > 0) { --heroHits; club->CheerNumber += 0.25f; }
            if (fighterHits > 0) { --fighterHits; club->BooNumber += 4.0f; }
            if (club->TimeOut) { ++g_targetCleared; return; }
            if (health <= threshold) {
                club->RoundStarted = false;
                club->RingCleared = true; ++g_cleared;
                while (dist > 2.0f) { Frame(); if (IsDone()) return; ++g_moves; dist -= 1.0f; }
                ++g_removed;
                return;
            }
            Frame();
        }
    }
};
int main()
{
    Club c = { false, false, false, false, 0.0f, 0.0f };
    Fighter f; f.club = &c; f.health = 100.0f; f.threshold = 30; f.heroHits = 2; f.fighterHits = 1; f.dist = 5.0f;
    f.Main();
    Club t = { true, true, false, false, 0.0f, 0.0f };
    Fighter g; g.club = &t; g.health = 100.0f; g.threshold = 30; g.heroHits = 0; g.fighterHits = 0; g.dist = 0.0f;
    g.Main();
    if (g_removed != 1 || g_cleared != 1 || g_moves != 3 || c.RoundStarted || !c.RingCleared || c.CheerNumber != 0.5f
        || c.BooNumber != 4.0f || g_targetCleared != 1) {
        std::printf("00e8b020_TEST FAIL %d %d %d %d\n", g_removed, g_cleared, g_moves, g_targetCleared); return 1;
    }
    std::printf("00e8b020_TEST PASS\n");
    return 0;
}