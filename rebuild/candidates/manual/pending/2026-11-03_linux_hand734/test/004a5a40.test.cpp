// Behaviour test for CWorld::UpdateFrame (0x004A5A40): a standalone model of the frame's control flow -- the save-load-failed
// prompt returns before anything else, a pending save-game load that is still loading (status 3 / 4) stops the frame, a region
// load in states 6..8 stops it, a player spawn in progress consumes the frame, and only an unpaused frame advances the counter;
// the full-update flag is true in non-scripted pause mode, else when bullet time is off or due a full update.
#include <cstdio>

struct World {
    bool saveLoadFailed, paused, nonScriptedPause, bulletTime, timeForFull;
    int saveStatus, regionStatus, spawnStatus;
    long frame, countdown, pauseAtFrame;
    bool lastFullUpdate;
};

static bool FullUpdate(const World& w)
{
    if (w.nonScriptedPause) return true;
    return !w.bulletTime || w.timeForFull;
}

static void UpdateFrame(World& w)
{
    if (w.saveLoadFailed) return;
    if (w.saveStatus != 0 && (w.saveStatus == 3 || w.saveStatus == 4)) return;
    if (w.regionStatus != 0 && (w.regionStatus == 6 || w.regionStatus == 7 || w.regionStatus == 8)) return;
    if (w.spawnStatus != 0) { if (w.spawnStatus == 1) w.spawnStatus = 0; return; }
    if (!w.paused) {
        w.lastFullUpdate = FullUpdate(w);
        ++w.frame;
        if (w.countdown > 0) --w.countdown;
    }
    if (w.pauseAtFrame > 0 && w.frame == w.pauseAtFrame && !w.paused) w.paused = true;
}

int main()
{
    World w = { false, false, false, false, false, 0, 0, 0, 0, 2, 2, false };
    UpdateFrame(w);
    if (w.frame != 1 || w.countdown != 1 || !w.lastFullUpdate || w.paused) { std::printf("004a5a40_TEST FAIL\n"); return 1; }
    w.bulletTime = true; UpdateFrame(w);                     // bullet time, not due: partial update; frame 2 hits the console pause
    if (w.frame != 2 || w.lastFullUpdate || !w.paused) { std::printf("004a5a40_TEST FAIL\n"); return 1; }
    UpdateFrame(w);                                          // paused: no frame advance
    if (w.frame != 2) { std::printf("004a5a40_TEST FAIL\n"); return 1; }
    w.paused = false; w.nonScriptedPause = true; w.pauseAtFrame = 0; UpdateFrame(w);
    if (w.frame != 3 || !w.lastFullUpdate) { std::printf("004a5a40_TEST FAIL\n"); return 1; }
    w.spawnStatus = 1; UpdateFrame(w);                       // spawn step consumes the frame and clears the status
    if (w.frame != 3 || w.spawnStatus != 0) { std::printf("004a5a40_TEST FAIL\n"); return 1; }
    w.regionStatus = 7; UpdateFrame(w);
    w.regionStatus = 0; w.saveStatus = 3; UpdateFrame(w);
    w.saveStatus = 0; w.saveLoadFailed = true; UpdateFrame(w);
    if (w.frame != 3) { std::printf("004a5a40_TEST FAIL\n"); return 1; }
    std::printf("004a5a40_TEST PASS\n");
    return 0;
}
