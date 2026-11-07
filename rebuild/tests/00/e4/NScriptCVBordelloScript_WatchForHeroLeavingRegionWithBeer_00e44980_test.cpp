// Behaviour test for 0x00E44980 NScript::CV_BordelloScript::WatchForHeroLeavingRegionWithBeer: the thread waits while the
// Bordello is loaded and the beer flag is set, then (unless terminating) clears the flag and resets the beer tankard's
// quick-access preference (category 2, time -1).
#include <cstdio>
#include <cstring>
struct Game {
    int framesLoaded;     // IsRegionLoaded("Bordello") stays true for this many frames
    int frames;
    int termAt;           // IsActiveThreadTerminating() true from this call on (count)
    int termCalls;
    int setCalls; char lastItem[32]; long lastA, lastB;
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "Bordello") == 0 && frames < framesLoaded; }
    void NewScriptFrame() { ++frames; }
    void SetPreferredQuickAccessItem(const char* item, long a, long b) { ++setCalls; std::strcpy(lastItem, item); lastA = a; lastB = b; }
};
struct Script {
    Game* G; bool BeerSetToDPad;
    bool IsActiveThreadTerminating() { return ++G->termCalls >= G->termAt; }
    void WatchForHeroLeavingRegionWithBeer()
    {
        for (;;) {
            bool inRegion = G->IsRegionLoaded("Bordello") && BeerSetToDPad;
            if (!inRegion)
                break;
            G->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        BeerSetToDPad = false;
        G->SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, -1);
    }
};
int main()
{
    Game g = { 3, 0, 1000, 0, 0, "", 0, 0 };
    Script s = { &g, true };
    s.WatchForHeroLeavingRegionWithBeer();
    if (g.frames != 3 || s.BeerSetToDPad || g.setCalls != 1 || std::strcmp(g.lastItem, "OBJECT_BEER_TANKARD") || g.lastA != 2 || g.lastB != -1) { std::printf("00e44980_TEST FAIL\n"); return 1; }
    Game g2 = { 100, 0, 2, 0, 0, "", 0, 0 };   // terminates inside the wait loop
    Script s2 = { &g2, true };
    s2.WatchForHeroLeavingRegionWithBeer();
    if (g2.frames != 2 || !s2.BeerSetToDPad || g2.setCalls != 0) { std::printf("00e44980_TEST FAIL\n"); return 1; }
    Game g3 = { 100, 0, 1000, 0, 0, "", 0, 0 };  // flag already clear: no wait, still resets
    Script s3 = { &g3, false };
    s3.WatchForHeroLeavingRegionWithBeer();
    if (g3.frames != 0 || g3.setCalls != 1 || g3.termCalls != 1) { std::printf("00e44980_TEST FAIL\n"); return 1; }
    std::printf("00e44980_TEST PASS\n");
    return 0;
}