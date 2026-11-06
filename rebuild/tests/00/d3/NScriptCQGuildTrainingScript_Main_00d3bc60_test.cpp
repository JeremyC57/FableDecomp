// Behaviour test for NScript::CQ_GuildTrainingScript::Main (0x00D3BC60): on a fresh start (GameState 0) take the hero's weapons,
// teleport to GuildArrivalHSP, wait for LookoutPoint, give the melee training card, show the start screen, run the arrival cutscene
// and fade out; always teleport to GuildTrainingHSP, wait for HeroGuildComplex, bind the fourteen entity scripts, post-add, switch
// off card guidance / weapon-out crime, guards ignore crimes and are disabled in the guild village, spawn the five threads.
// A state model, run fresh, resumed and terminated.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Script {
    int GameState; int Frames; int TerminateAt; int Bindings; int Spawned;
    bool IsActiveThreadTerminating() { return TerminateAt >= 0 && Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; }
    bool IsLevelLoaded(const char* name) { return !std::strcmp(name, "LookoutPoint") ? Frames >= 2 : Frames >= 5; }
    void Main()
    {
        static const char* const names[14] = { "AppleGirl", "ApprenticeSpeedTest", "SpeedFriend", "RaceMarker", "MeleeApprentice",
            "CombatApprentice", "SkillApprentice", "WillApprentice", "HeroBed", "SkillTarget", "WillDummy", "BirdKiller", "KillBird",
            "PreMeleeMaze" };
        if (GameState == 0) {
            if (IsActiveThreadTerminating()) return;
            Log("weapons"); Log("GuildArrivalHSP");
            while (!IsLevelLoaded("LookoutPoint")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Log("OBJECT_QUEST_CARD_TRAINING_MELEE");
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            Log("start_screen"); Log("cutscene"); Log("fade");
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        Log("GuildTrainingHSP");
        while (!IsLevelLoaded("HeroGuildComplex")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        for (int i = 0; i < 14; ++i) if (names[i][0]) ++Bindings;
        Log("post"); Log("guards_off");
        Spawned = 5;
    }
};
int main()
{
    Script a = { 0, 0, -1, 0, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "weapons;GuildArrivalHSP;OBJECT_QUEST_CARD_TRAINING_MELEE;start_screen;cutscene;fade;GuildTrainingHSP;post;guards_off;")
        && a.Bindings == 14 && a.Spawned == 5 && a.Frames == 5;
    g_log[0] = 0;
    Script b = { 1, 0, -1, 0, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "GuildTrainingHSP;post;guards_off;") && b.Bindings == 14 && b.Frames == 5;
    g_log[0] = 0;
    Script c = { 0, 0, 3, 0, 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "weapons;GuildArrivalHSP;OBJECT_QUEST_CARD_TRAINING_MELEE;") && c.Bindings == 0 && c.Spawned == 0;
    std::printf(ok ? "00d3bc60_TEST PASS\n" : "00d3bc60_TEST FAIL\n");
    return ok ? 0 : 1;
}