// Behaviour test for CTCCreatureGroupBoastingCrowd::EnterCrowdState (0x00784260): no-op when already in the state or in
// state 10; leaving pops one opinion source and removes the pending deed; entering pushes the state's opinion source
// and logs its deed (only states 1-4, 7 and 9 have deeds); state 0 kills the crowd.
#include <cstdio>
#include <cstring>
static int g_cleared = 0, g_removed = 0, g_added = 0, g_killed = 0, g_lastDeedType = 0;
static const char* g_lastSource = 0;
static long g_nextHandle = 100;
struct CTCCreatureGroupBoastingCrowd {
    long CrowdState;
    long CurrentOpinionDeedHandle;
    long HeroOpinionSourcesPushed;
    bool HeroAlive;
    void EnterCrowdState(long state)
    {
        if (CrowdState == state || CrowdState == 10)
            return;
        if (HeroOpinionSourcesPushed > 0) { ++g_cleared; --HeroOpinionSourcesPushed; }
        if (CurrentOpinionDeedHandle != -1) {
            if (HeroAlive) ++g_removed;
            CurrentOpinionDeedHandle = -1;
        }
        const char* source = "";
        long deedType = 0;
        switch (state) {
        case 0: ++g_killed; break;
        case 1: source = "OPINION_SOURCE_HERO_BOASTING_ANTICIPATION"; deedType = 0x40; break;
        case 2: source = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT"; deedType = 0x41; break;
        case 3: source = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT"; deedType = 0x42; break;
        case 4: source = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT"; deedType = 0x43; break;
        case 5: source = "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_GOOD"; break;
        case 6: source = "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_EVIL"; break;
        case 7: source = "OPINION_SOURCE_HERO_BOASTING_WELL_WISHES"; deedType = 0x44; break;
        case 8: source = "OPINION_SOURCE_HERO_BOASTING_ILL_WISHES"; break;
        case 9: source = "OPINION_SOURCE_HERO_BOASTING_ANNOYED_NO_BOASTING"; deedType = 0x45; break;
        case 10: source = "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD"; break;
        }
        if (std::strcmp(source, "") != 0) { g_lastSource = source; ++g_added; ++HeroOpinionSourcesPushed; }
        if (deedType && HeroAlive) { g_lastDeedType = deedType; CurrentOpinionDeedHandle = g_nextHandle++; }
        CrowdState = state;
    }
};
static int fail() { std::printf("00784260_TEST FAIL\n"); return 1; }
int main()
{
    CTCCreatureGroupBoastingCrowd c = { 0, -1, 0, true };
    c.EnterCrowdState(0);
    if (g_killed || g_added) return fail();
    c.EnterCrowdState(2);
    if (c.CrowdState != 2 || c.HeroOpinionSourcesPushed != 1 || g_lastDeedType != 0x41 || c.CurrentOpinionDeedHandle != 100) return fail();
    c.EnterCrowdState(5);
    if (g_cleared != 1 || g_removed != 1 || c.CurrentOpinionDeedHandle != -1 || c.HeroOpinionSourcesPushed != 1) return fail();
    if (std::strcmp(g_lastSource, "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_GOOD") != 0) return fail();
    c.EnterCrowdState(0);
    if (g_killed != 1 || c.HeroOpinionSourcesPushed != 0 || c.CrowdState != 0) return fail();
    c.EnterCrowdState(10);
    c.EnterCrowdState(1);
    if (c.CrowdState != 10 || g_added != 3) return fail();
    std::printf("00784260_TEST PASS\n");
    return 0;
}