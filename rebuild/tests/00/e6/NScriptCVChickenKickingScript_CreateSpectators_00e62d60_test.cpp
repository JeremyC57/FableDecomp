// Behaviour test for 0x00E62D60 NScript::CV_ChickenKickingScript::CreateSpectators: the three spectators are created exactly once,
// only while Oak Bay is loaded and after the hero knows the ghost has gone; the thread then ends.
#include <cstdio>
#include <cstring>
static int gFrames = 0, gTerminateAt = 1000, gCreated = 0, gFemale = 0;
static bool gKnowGhostHasGone = false, gSpectatorsCreated = false;
static bool RegionLoaded() { return gFrames >= 3 && gFrames < 40; }
static bool Term() { return gFrames >= gTerminateAt; }
static void Frame() { ++gFrames; if (gFrames == 10) gKnowGhostHasGone = true; }
static void Create(const char* def, const char* marker)
{
    ++gCreated;
    if (std::strstr(def, "FEMALE") != 0) ++gFemale;
    (void)marker;
}
static void CreateSpectators()
{
    while (!gSpectatorsCreated) {
        Frame();
        if (Term()) return;
        while (RegionLoaded() && !gSpectatorsCreated) {
            Frame();
            if (Term()) return;
            if (gKnowGhostHasGone) {
                if (Term()) return;
                Create("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", "Spectator1");
                Create("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", "Spectator2");
                Create("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", "Spectator3");
                gSpectatorsCreated = true;
            }
        }
        if (Term()) return;
        while (RegionLoaded()) {
            Frame();
            if (Term()) return;
        }
        if (Term()) return;
    }
}
int main()
{
    CreateSpectators();
    bool ok = gCreated == 3 && gFemale == 1 && gSpectatorsCreated && gFrames == 40;
    // Already created: nothing more happens.
    gFrames = 0;
    CreateSpectators();
    ok = ok && gCreated == 3 && gFrames == 0;
    // Terminating before the ghost is gone: nothing is created.
    gFrames = 0; gCreated = 0; gSpectatorsCreated = false; gKnowGhostHasGone = false; gTerminateAt = 8;
    CreateSpectators();
    ok = ok && gCreated == 0 && !gSpectatorsCreated;
    std::printf(ok ? "00e62d60_TEST PASS\n" : "00e62d60_TEST FAIL\n");
    return ok ? 0 : 1;
}