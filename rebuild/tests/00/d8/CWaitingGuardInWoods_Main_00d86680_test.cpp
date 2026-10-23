// Behaviour test for CWaitingGuardInWoods::Main (0x00D86680): wait for the cutscene flag, add the orb marker, then wait
// for the distance stage / a hero hit / the hero coming close, then target the hero and idle.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Guard {
    bool playedCS; long distanceReached;
    int frames, terminateAt, csAt, hitAt, nearAt, farAt;
    bool onScreen;
    void NewScriptFrame() { ++frames; Log("f"); if (frames == csAt) playedCS = true; }
    bool IsActiveThreadTerminating() const { return frames >= terminateAt; }
    bool MsgIsHitBy(const char*) const { return frames == hitAt; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) const { return false; }
    bool MsgIsHitBySpecialAbilityFrom(long, const char*) const { return false; }
    bool Under(float d) const { return d < 5.0f ? frames >= nearAt : frames >= farAt; }
    void Main()
    {
        Log("pred");
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (IsActiveThreadTerminating()) return;
        while (!playedCS) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("orb");
        while (distanceReached != 2 && !MsgIsHitBy("SCRIPT_NAME_HERO")
               && (!MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") || MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))
               && !Under(2.0f) && (!Under(10.0f) || !onScreen)) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Log("target");
        do { NewScriptFrame(); } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    Guard g; g.playedCS = false; g.distanceReached = 0; g.frames = 0; g.terminateAt = 9; g.csAt = 2; g.hitAt = 99; g.nearAt = 99; g.farAt = 4; g.onScreen = true;
    g_log[0] = 0; g.Main();
    if (std::strcmp(g_log, "pred,f,f,orb,f,f,target,f,f,f,f,f,") != 0) { std::printf("%s 00d86680_TEST FAIL\n", g_log); return 1; }
    Guard h = g; h.playedCS = false; h.frames = 0; h.onScreen = false; h.terminateAt = 6;
    g_log[0] = 0; h.Main();
    if (std::strcmp(g_log, "pred,f,f,orb,f,f,f,f,") != 0) { std::printf("%s 00d86680_TEST FAIL\n", g_log); return 1; }
    Guard k = g; k.playedCS = false; k.frames = 0; k.distanceReached = 2; k.farAt = 99;
    g_log[0] = 0; k.Main();
    if (std::strcmp(g_log, "pred,f,f,orb,target,f,f,f,f,f,f,f,") != 0) { std::printf("%s 00d86680_TEST FAIL\n", g_log); return 1; }
    std::printf("00d86680_TEST PASS\n");
    return 0;
}