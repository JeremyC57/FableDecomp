// Behaviour test for CV_TourGuideScript::CTourGuideGuide::Main (0x00EE57B0): the Initialise waypoint choice (resume after a saved
// waypoint with wrap-around at TG_LOCATIONS_MAX, keep the current one while it is valid, else restart at M_TG_LocationStart) and the
// random guide response that never repeats the previous index.
#include <cstdio>

enum { TG_LOCATIONS_MAX = 18 };
struct Guide {
    long saveWaypointIdx; long lastRandomSpeechIdx; long WaypointCounter; bool waypointAlive; int restarts;
    void Initialise()
    {
        if (saveWaypointIdx != -1) {
            WaypointCounter = saveWaypointIdx + 1;
            if (WaypointCounter >= TG_LOCATIONS_MAX)
                WaypointCounter = 0;
            saveWaypointIdx = -1;
        } else if (waypointAlive && WaypointCounter < TG_LOCATIONS_MAX) {
        } else {
            ++restarts;
            WaypointCounter = 0;
        }
    }
    long PickResponse(const long* rolls)
    {
        long idx;
        int i = 0;
        for (;;) {
            idx = rolls[i++] % 5;
            if (idx != lastRandomSpeechIdx) break;
        }
        lastRandomSpeechIdx = idx;
        return idx;
    }
};

int main()
{
    Guide g = { 17, -1, 3, true, 0 };
    g.Initialise();
    bool ok = g.WaypointCounter == 0 && g.saveWaypointIdx == -1;
    g.saveWaypointIdx = 4; g.Initialise(); ok = ok && g.WaypointCounter == 5;
    g.Initialise(); ok = ok && g.WaypointCounter == 5 && g.restarts == 0;
    g.waypointAlive = false; g.Initialise(); ok = ok && g.WaypointCounter == 0 && g.restarts == 1;
    const long rolls[] = { 7, 12, 22, 3 };
    g.lastRandomSpeechIdx = 2;
    ok = ok && g.PickResponse(rolls) == 3 && g.lastRandomSpeechIdx == 3;
    if (!ok) { std::printf("00ee57b0_TEST FAIL\n"); return 1; }
    std::printf("00ee57b0_TEST PASS\n");
    return 0;
}
