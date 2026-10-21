// Behaviour test for 00d129c0 (CQ_BanditCampHoldingScriptScript::Main): wait for the path level (once, recorded in
// PathReached), then for the entrance region, then open the raid gate unless it is already open.
#include <cstdio>
#include <cstring>

struct Game {
    int frames, levelAt, regionAt; bool doorOpen; bool hasDoor; int opens;
    void NewScriptFrame() { ++frames; }
    bool IsLevelLoaded(const char* n) const { return std::strcmp(n, "BanditCampPath_1") == 0 && frames >= levelAt; }
    bool IsRegionLoaded(const char* n) const { return std::strcmp(n, "BanditCampPathEntrance") == 0 && frames >= regionAt; }
    void OpenDoor() { ++opens; doorOpen = true; }
};
struct Script {
    Game* game; bool pathReached; int termAt;
    bool IsActiveThreadTerminating() { return game->frames >= termAt; }
    bool GateIsOpen() const { if (!game->hasDoor) return false; return game->doorOpen; }
    void Main()
    {
        if (!pathReached) {
            if (IsActiveThreadTerminating()) return;
            while (!game->IsLevelLoaded("BanditCampPath_1")) { game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            pathReached = true;
        }
        while (!game->IsRegionLoaded("BanditCampPathEntrance")) { game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (!GateIsOpen()) {
            if (IsActiveThreadTerminating()) return;
            game->OpenDoor();
        }
    }
};
int main()
{
    int ok = 1;
    Game g = { 0, 2, 5, false, true, 0 };
    Script s = { &g, false, 100 };
    s.Main();
    if (!s.pathReached || g.frames != 5 || g.opens != 1) ok = 0;
    Game h = { 0, 50, 1, true, true, 0 };
    Script t = { &h, true, 100 };
    t.Main();
    if (h.frames != 1 || h.opens != 0) ok = 0;
    Game k = { 0, 50, 1, false, true, 0 };
    Script u = { &k, false, 3 };
    u.Main();
    if (u.pathReached || k.frames != 3 || k.opens != 0) ok = 0;
    if (!ok) { std::printf("00d129c0_TEST FAIL\n"); return 1; }
    std::printf("00d129c0_TEST PASS\n");
    return 0;
}