// Behaviour test for NScript::CQ_WhiteBalverineWWScript::Main (0x00E18630): three bindings, generators off, wait for WitchWood_7,
// objective 07, fetch the balverine and keep it persistent, two threads, the CS_WBW_DRINK cut-scene, scripted control, wait until the
// hero is within 8 units or hits the balverine (a heal spell does not count), boss music + enemy target, wait for fail / success, then
// fail the quest or mark WhiteBalverineFinished, deactivate it and hide the quest info. A state model, run for success and failure.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    bool MissionFailed, MissionSucceeded, WhiteBalverineFinished, Terminate;
    int Frames, FailAt, HitKind;           // HitKind: 0 near, 1 hit, 2 heal-spell then hit
    float Distance;
    bool IsLevelLoaded(const char*) { return Frames >= 2; }
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames == 4) Distance = 5.0f;
        if (Frames == FailAt) { if (HitKind == 1) MissionFailed = true; else MissionSucceeded = true; }
    }
    bool IsActiveThreadTerminating() { return Terminate; }
    bool Near() { return HitKind == 0 && Distance < 8.0f; }
    bool HitBy() { return HitKind == 1 && Frames >= 4; }
    bool HitByAnySpecial() { return HitKind == 2 && Frames >= 3; }
    bool HitBySpecialHeal() { return HitKind == 2 && Frames < 5; }
    void Main()
    {
        Log("bind3"); Log("post"); Log("gen_off");
        while (!IsLevelLoaded("WitchWood_7")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("obj07"); Log("persist"); Log("threads2"); Log("CS_WBW_DRINK");
        while (!(Near() || HitBy() || (HitByAnySpecial() && !HitBySpecialHeal()))) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Log("boss_music");
        while (!MissionFailed && !MissionSucceeded) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (MissionSucceeded) { if (IsActiveThreadTerminating()) return; WhiteBalverineFinished = true; }
        else { if (IsActiveThreadTerminating()) return; Log("failed"); }
        Log("deactivate_later"); Log("hide_info");
    }
};
int main()
{
    Quest a = { false, false, false, false, 0, 7, 0, 20.0f };
    a.Main();
    bool ok = !std::strcmp(g_log, "bind3;post;gen_off;obj07;persist;threads2;CS_WBW_DRINK;boss_music;deactivate_later;hide_info;")
        && a.WhiteBalverineFinished && a.Frames == 7;
    g_log[0] = 0;
    Quest b = { false, false, false, false, 0, 6, 1, 20.0f };
    b.Main();
    ok = ok && !std::strcmp(g_log, "bind3;post;gen_off;obj07;persist;threads2;CS_WBW_DRINK;boss_music;failed;deactivate_later;hide_info;")
        && !b.WhiteBalverineFinished && b.Frames == 6;
    g_log[0] = 0;
    Quest c = { false, false, false, false, 0, 9, 2, 20.0f };
    c.Main();
    ok = ok && c.Frames == 9 && c.WhiteBalverineFinished;   // the heal spell (frames 3-4) does not start the fight; frame 5 does
    g_log[0] = 0;
    Quest d = { false, false, false, true, 0, 9, 0, 20.0f };
    d.Main();
    ok = ok && !std::strcmp(g_log, "bind3;post;gen_off;") && d.Frames == 1;
    std::printf(ok ? "00e18630_TEST PASS\n" : "00e18630_TEST FAIL\n");
    return ok ? 0 : 1;
}