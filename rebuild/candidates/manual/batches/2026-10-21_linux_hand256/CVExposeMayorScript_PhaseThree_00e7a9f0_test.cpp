// Behaviour test for 0x00E7A9F0 (CV_ExposeMayorScript::PhaseThree): wait for the lights, then stage 1/2 -> 3.
#include <cstdio>
#include <cstring>
#include <string>
// Host model: logs 'F' frame, 'X' ghost cut-scene (fade, "NULL" music), 'U' cellar door usable, 'O' third objective;
// the lights flag is raised at frame 'flashAt'; terminating is a countdown.
struct Game {
    std::string log; int frames; int flashAt; bool* flag;
    Game() : frames(0), flashAt(0), flag(0) {}
    void NewScriptFrame() { log += "F"; ++frames; if (frames >= flashAt) *flag = true; }
    std::string GetActiveQuestName() { return "Q_ExposeMayor"; }
    std::string GetThingWithScriptName(const char* n) { return n; }
    void SetThingAsUsable(const std::string& t, bool u) { log += (t == "HH_CellarDoor" && u) ? "U" : "?"; }
    void SetQuestCardObjective(const std::string& q, const std::string& t, const std::string& r1, const std::string& r2)
    { log += (q == "Q_ExposeMayor" && t == "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_03" && r1 == "HauntedHouseCellar" && r2.empty()) ? "O" : "?"; }
};
struct Script {
    Game* GSI; long StageNumber; bool HeroFlashedLightsCorrectly; int termAfter; int checks;
    Script(Game* g, long stage, bool flashed, int after)
        : GSI(g), StageNumber(stage), HeroFlashedLightsCorrectly(flashed), termAfter(after), checks(0) { g->flag = &HeroFlashedLightsCorrectly; }
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void PlayCutscene(std::string name, bool fade, std::string music)
    { GSI->log += (name == "CS_MAYOREXPOSE_GHOSTAPPEAR" && fade && music == "NULL") ? "X" : "?"; }
    void PhaseThree()
    {
        while (!HeroFlashedLightsCorrectly) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (StageNumber == 2 || StageNumber == 1) {
            if (IsActiveThreadTerminating()) return;
            PlayCutscene("CS_MAYOREXPOSE_GHOSTAPPEAR", true, "NULL");
            std::string door = GSI->GetThingWithScriptName("HH_CellarDoor");
            GSI->SetThingAsUsable(door, true);
            GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_03", "HauntedHouseCellar", "");
            StageNumber = 3;
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s(&g, 1, true, 5); s.PhaseThree(); CHECK(g.log == "XUO" && s.StageNumber == 3); }
    { Game g; Script s(&g, 2, true, 5); s.PhaseThree(); CHECK(g.log == "XUO" && s.StageNumber == 3); }
    { Game g; Script s(&g, 0, true, 5); s.PhaseThree(); CHECK(g.log == "" && s.StageNumber == 0); }
    { Game g; Script s(&g, 3, true, 5); s.PhaseThree(); CHECK(g.log == "" && s.StageNumber == 3); }
    { Game g; g.flashAt = 2; Script s(&g, 1, false, 5); s.PhaseThree(); CHECK(g.log == "FFXUO"); }
    { Game g; g.flashAt = 9; Script s(&g, 1, false, 2); s.PhaseThree(); CHECK(g.log == "FFF" && s.StageNumber == 1); }
    { Game g; Script s(&g, 1, true, 1); s.PhaseThree(); CHECK(g.log == "" && s.StageNumber == 1); }
    if (g_fail) { std::printf("00e7a9f0_TEST FAIL\n"); return 1; }
    std::printf("00e7a9f0_TEST PASS\n");
    return 0;
}
