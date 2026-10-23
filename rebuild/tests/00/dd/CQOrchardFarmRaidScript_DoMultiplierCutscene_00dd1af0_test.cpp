#include <cstdio>
#include <cstring>
// Behaviour test for CQ_OrchardFarmRaidScript::DoMultiplierCutscene (0x00DD1AF0): picks the Whisper intro cut-scene by
// location and side, always unpauses, and shows the combat-multiplier tutorial once.
struct CGSI {
    bool LakeLoaded; bool NearMarker; int Paused; int Frames; int Tutorials; char Cutscene[48]; int ClickAt;
    void PauseAllNonScriptedEntities(bool p) { Paused += p ? 1 : -1; }
    bool IsRegionLoaded(const char*) { return LakeLoaded; }
    bool DisplayTutorial(int c) { if (c == 9) ++Tutorials; return true; }
    bool MsgIsTutorialClickedPast() { return Frames >= ClickAt; }
    void NewScriptFrame() { ++Frames; }
};
static CGSI* g;
static void Run(const char* name) { std::strcpy(g->Cutscene, name); }
struct CQ_OrchardFarmRaidScript {
    CGSI* GSI; long HeroTeam; bool ShownCombatMultiplierTutorial;
    bool IsActiveThreadTerminating() { return GSI->Frames > 20; }
    void DoMultiplierCutscene()
    {
        CGSI* game = GSI;
        game->PauseAllNonScriptedEntities(true);
        if (GSI->IsRegionLoaded("GreatwoodLake") && HeroTeam == 0) {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            Run("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL");
        } else {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            if (GSI->NearMarker) {
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                if (HeroTeam == 1) Run("CS_ORCHARD_EVIL_WHISPERINTRO_GWL"); else Run("CS_ORCHARD_GOOD_WHISPERINTRO_GWL");
            } else {
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                if (HeroTeam == 1) Run("CS_ORCHARD_EVIL_WHISPERINTRO_LOP"); else Run("CS_ORCHARD_GOOD_WHISPERINTRO_LOP");
            }
        }
        if (ShownCombatMultiplierTutorial == false) {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            if (GSI->DisplayTutorial(9)) {
                while (!GSI->MsgIsTutorialClickedPast()) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
            }
            ShownCombatMultiplierTutorial = true;
        }
        game->PauseAllNonScriptedEntities(false);
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); g = &gsi; gsi.ClickAt = 3;
    CQ_OrchardFarmRaidScript s; s.GSI = &gsi; s.HeroTeam = 0; s.ShownCombatMultiplierTutorial = false;
    gsi.LakeLoaded = true; s.DoMultiplierCutscene();
    bool ok = !std::strcmp(gsi.Cutscene, "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL") && gsi.Tutorials == 1 && gsi.Frames == 3 && gsi.Paused == 0;
    s.HeroTeam = 1; gsi.NearMarker = true; s.DoMultiplierCutscene();
    ok = ok && !std::strcmp(gsi.Cutscene, "CS_ORCHARD_EVIL_WHISPERINTRO_GWL") && gsi.Tutorials == 1 && gsi.Paused == 0;
    s.HeroTeam = 0; gsi.LakeLoaded = false; gsi.NearMarker = false; s.DoMultiplierCutscene();
    ok = ok && !std::strcmp(gsi.Cutscene, "CS_ORCHARD_GOOD_WHISPERINTRO_LOP") && gsi.Paused == 0;
    if (!ok) { std::printf("00dd1af0_TEST FAIL\n"); return 1; }
    std::printf("00dd1af0_TEST PASS\n");
    return 0;
}