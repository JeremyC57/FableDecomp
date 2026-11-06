// Behaviour test for NScript::CQ_AwakeningTheOracleScript::Main (0x00CFB080): eleven bindings (PretendGuard the one flag-1 binding),
// generators off in Northern Wastes 3, objective 1; the opening block (wait for NW3, autosave once, objective at NW3, wait for Snowspire,
// cut-scene, objective 2, fade in) runs only when the opening cut-scene has not played; then NW3 generators on, objective 2,
// WatchForAliveTablets, NW2 generators off, the necropolis block once, wait for four tablets, complete, deactivate later, fade in.
// A state model run fresh and from a save with both cut-scenes played.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    bool PlayedOpeningCS, PlayedNecroCS, AutosavedInNW3;
    long TabletsCollected;
    int Frames, Flags;
    bool IsRegionLoaded(const char*) { return (Frames & 1) != 0; }
    void NewScriptFrame() { ++Frames; if (Frames >= 6) TabletsCollected = 4; }
    bool IsActiveThreadTerminating() { return false; }
    void Main()
    {
        for (int i = 0; i < 11; ++i) Flags += i == 10 ? 1 : 0;
        Log("gen_off_NW3"); Log("obj1_LostBay");
        if (!PlayedOpeningCS) {
            if (IsActiveThreadTerminating()) return;
            while (!IsRegionLoaded("NorthernWastes3")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            if (!AutosavedInNW3) { if (IsActiveThreadTerminating()) return; AutosavedInNW3 = true; Log("autosave"); }
            Log("obj1_NW3");
            while (!IsRegionLoaded("SnowspireVillage")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Log("CS_SNOWSPIRE_INTRO"); PlayedOpeningCS = true; Log("obj2"); Log("fade");
        }
        Log("gen_on_NW3"); Log("obj2"); Log("WatchForAliveTablets"); Log("gen_off_NW2");
        if (!PlayedNecroCS) {
            if (IsActiveThreadTerminating()) return;
            while (!IsRegionLoaded("NorthernWastes2")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Log("CS_ORACLE_NECROPOLIS_INTRO"); PlayedNecroCS = true; Log("start_screen"); Log("fade");
        }
        while (TabletsCollected != 4) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("completed"); Log("deactivate_later"); Log("fade");
    }
};
int main()
{
    Quest fresh = { false, false, false, 0, 0, 0 };
    fresh.Main();
    bool ok = !std::strcmp(g_log, "gen_off_NW3;obj1_LostBay;autosave;obj1_NW3;CS_SNOWSPIRE_INTRO;obj2;fade;gen_on_NW3;obj2;"
                                  "WatchForAliveTablets;gen_off_NW2;CS_ORACLE_NECROPOLIS_INTRO;start_screen;fade;completed;deactivate_later;fade;")
        && fresh.Flags == 1 && fresh.PlayedOpeningCS && fresh.PlayedNecroCS && fresh.TabletsCollected == 4;
    g_log[0] = 0;
    Quest loaded = { true, true, true, 4, 0, 0 };
    loaded.Main();
    ok = ok && !std::strcmp(g_log, "gen_off_NW3;obj1_LostBay;gen_on_NW3;obj2;WatchForAliveTablets;gen_off_NW2;completed;deactivate_later;fade;")
        && loaded.Frames == 0;
    std::printf(ok ? "00cfb080_TEST PASS\n" : "00cfb080_TEST FAIL\n");
    return ok ? 0 : 1;
}