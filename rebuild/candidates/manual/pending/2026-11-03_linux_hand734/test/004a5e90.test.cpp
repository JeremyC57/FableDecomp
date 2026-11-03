// Behaviour test for CWorld::AutoLoadRespawn (0x004A5E90): a standalone model of the choice -- between a quest's start and finish
// screens an existing checkpoint save wins (and its result is final); otherwise the most recent manual save (type 1), the
// quest-start autosave (type 3, only between the screens) or the plain autosave; LoadGameState(name, false) only queues a load
// when none is pending (status 0) and the name resolves, and a queued respawn load is marked status 3.
#include <cstdio>
#include <string>

struct World { int saveStatus; int mostRecentSaveType; bool betweenQuestScreens; bool checkPointExists; std::string queued; };

static bool LoadGameState(World& w, const std::string& name, bool resolves)
{
    if (w.saveStatus != 0) return false;
    if (!resolves) return false;
    w.saveStatus = 1; w.queued = name; return true;
}

static bool AutoLoadRespawn(World& w, bool resolves)
{
    if (w.betweenQuestScreens && w.checkPointExists) {
        if (LoadGameState(w, "AutoSave.cp", resolves)) { w.saveStatus = 3; return true; }
        return false;
    }
    std::string name;
    if (w.mostRecentSaveType == 1) name = "manual";
    else if (w.mostRecentSaveType == 3 && w.betweenQuestScreens) name = "AutoSave.qs";
    else name = "AutoSave";
    if (LoadGameState(w, name, resolves)) { w.saveStatus = 3; return true; }
    return false;
}

int main()
{
    World a = { 0, 1, true, true, "" };
    if (!AutoLoadRespawn(a, true) || a.queued != "AutoSave.cp" || a.saveStatus != 3) { std::printf("004a5e90_TEST FAIL\n"); return 1; }
    World b = { 0, 1, true, false, "" };
    if (!AutoLoadRespawn(b, true) || b.queued != "manual") { std::printf("004a5e90_TEST FAIL\n"); return 1; }
    World c = { 0, 3, false, false, "" };
    if (!AutoLoadRespawn(c, true) || c.queued != "AutoSave") { std::printf("004a5e90_TEST FAIL\n"); return 1; }
    World d = { 0, 3, true, false, "" };
    if (!AutoLoadRespawn(d, true) || d.queued != "AutoSave.qs") { std::printf("004a5e90_TEST FAIL\n"); return 1; }
    World e = { 1, 0, false, false, "" };                    // a load already pending: nothing queued
    if (AutoLoadRespawn(e, true) || e.saveStatus != 1) { std::printf("004a5e90_TEST FAIL\n"); return 1; }
    World f = { 0, 0, true, true, "" };                      // checkpoint branch is final even when its name does not resolve
    if (AutoLoadRespawn(f, false) || f.saveStatus != 0) { std::printf("004a5e90_TEST FAIL\n"); return 1; }
    std::printf("004a5e90_TEST PASS\n");
    return 0;
}
