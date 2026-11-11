// Behaviour test for CTCOracleMinigame::FrameUpdate (0x007E6750): the oracle minigame's per-frame state machine.
#include <stdio.h>
#include <list>
#include <string>
static long g_frame = 100, g_fps = 30;
static bool g_displayingText = false;
static long g_symbols = 0, g_pops = 0, g_miniInfo = 0, g_tints = 0, g_quits = 0, g_fired = 0;
static bool g_outputsFiring = false;
static std::string g_button2, g_sounds;
static unsigned long g_lastText = 0;
struct Def { float WaitBetweenRoundsSeconds, PauseBetweenOutputsLevelMultiplier; long MaxLevel; unsigned long TextWatch, TextRepeat, TextWonRound; };
struct Game {
    long Level, Stage, PostWaitStage, WaitTime; float TimeBetweenOutputs; long NumOutputsFired; bool Success; long NextOutputFrame;
    std::list<int> InputsPending; const Def* PDef;
    void UpdateTints() { ++g_tints; }
    bool OutputsFiring() { return g_outputsFiring; }
    void SetText(unsigned long t) { g_lastText = t; }
    void FireOutput() { ++g_fired; ++NumOutputsFired; }
    void PlaySoundW(const std::string& s) { g_sounds += s; g_sounds += ";"; }
    void Quit() { ++g_quits; Stage = 5; }
    void FrameUpdate();
};
void Game::FrameUpdate()
{
    if (Stage != 0)
        UpdateTints();
    if ((unsigned long)g_symbols > InputsPending.size() + 1
        || (InputsPending.size() == 0 && g_symbols != 0 && !OutputsFiring())) {
        if (g_symbols) { --g_symbols; ++g_pops; }
    }
    switch (Stage) {
    case 1:
        if (!g_displayingText) {
            Stage = 2; PostWaitStage = 3;
            SetText(PDef->TextWatch);
            WaitTime = (long)(g_frame + g_fps * PDef->WaitBetweenRoundsSeconds);
            g_button2 = "TEXT_GUI_FISHING_STOP";
            ++g_miniInfo;
            PlaySoundW("SND_MM_GLYPH_MINIGAME_START");
        }
        break;
    case 2:
        if (WaitTime < g_frame) {
            Stage = PostWaitStage;
            SetText(PDef->TextWatch);
            NextOutputFrame = g_frame;
            NumOutputsFired = 0;
            if (Stage == 4) SetText(PDef->TextRepeat);
            if (Stage == 3) SetText(PDef->TextWatch);
        }
        break;
    case 3:
        if (NextOutputFrame <= g_frame) {
            if (NumOutputsFired < Level) {
                FireOutput();
                NextOutputFrame = (long)(g_frame + g_fps * TimeBetweenOutputs);
            } else {
                PostWaitStage = 4; WaitTime = 0; NumOutputsFired = 0; Stage = 2;
            }
        }
        break;
    case 4:
        if (NumOutputsFired == Level && !OutputsFiring()) {
            PlaySoundW("SND_MM_EXPRESSION_SEQUENCE_COMPLETE");
            const Def* def = PDef;
            ++Level;
            if (Level > def->MaxLevel) { Success = true; Quit(); }
            else {
                Stage = 2; PostWaitStage = 3;
                WaitTime = (long)(g_frame + g_fps * def->WaitBetweenRoundsSeconds);
                TimeBetweenOutputs = PDef->PauseBetweenOutputsLevelMultiplier * TimeBetweenOutputs;
                SetText(PDef->TextWonRound);
            }
        }
        break;
    case 5:
        if (WaitTime < g_frame) Stage = 0;
        break;
    }
}
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
int main(void)
{
    Def def = { 2.0f, 0.5f, 2, 11, 12, 13 };
    Game g; g.PostWaitStage = 0; g.WaitTime = 0; g.PDef = &def; g.Success = false; g.NextOutputFrame = 0;
    g.Level = 1; g.Stage = 0; g.TimeBetweenOutputs = 1.0f; g.NumOutputsFired = 0;
    g.FrameUpdate(); CHECK(g_tints == 0 && g.Stage == 0);                 // idle: nothing happens
    g_symbols = 3; g.FrameUpdate(); CHECK(g_pops == 1 && g_symbols == 2);   // more symbols than inputs + 1: pop one
    g_outputsFiring = true; g_symbols = 1; g.FrameUpdate(); CHECK(g_pops == 1);   // leftovers stay while outputs fire
    g_outputsFiring = false; g.FrameUpdate(); CHECK(g_pops == 2 && g_symbols == 0);
    g.Stage = 1; g_displayingText = true; g.FrameUpdate(); CHECK(g.Stage == 1 && g_tints == 1);
    g_displayingText = false; g.FrameUpdate();
    CHECK(g.Stage == 2 && g.PostWaitStage == 3 && g.WaitTime == 160 && g_lastText == 11 && g_miniInfo == 1);
    CHECK(g_button2 == "TEXT_GUI_FISHING_STOP" && g_sounds == "SND_MM_GLYPH_MINIGAME_START;");
    g.FrameUpdate(); CHECK(g.Stage == 2);                                  // still waiting
    g_frame = 161; g.FrameUpdate(); CHECK(g.Stage == 3 && g.NextOutputFrame == 161 && g.NumOutputsFired == 0);
    g.FrameUpdate(); CHECK(g_fired == 1 && g.NextOutputFrame == 191);
    g.FrameUpdate(); CHECK(g_fired == 1);                                  // not due yet
    g_frame = 191; g.FrameUpdate(); CHECK(g.Stage == 2 && g.PostWaitStage == 4 && g.WaitTime == 0 && g.NumOutputsFired == 0);
    g.FrameUpdate(); CHECK(g.Stage == 4 && g_lastText == 12);
    g.NumOutputsFired = 1; g_outputsFiring = true; g.FrameUpdate(); CHECK(g.Level == 1);
    g_outputsFiring = false; g.FrameUpdate();
    CHECK(g.Level == 2 && g.Stage == 2 && g.PostWaitStage == 3 && g.WaitTime == 251 && g.TimeBetweenOutputs == 0.5f && g_lastText == 13);
    g.Stage = 4; g.NumOutputsFired = 2; g.FrameUpdate(); CHECK(g.Level == 3 && g.Success && g_quits == 1 && g.Stage == 5);
    g.WaitTime = 300; g.FrameUpdate(); CHECK(g.Stage == 5);
    g_frame = 301; g.FrameUpdate(); CHECK(g.Stage == 0);
    if (fails) { printf("007e6750_TEST FAIL\n"); return 1; }
    printf("007e6750_TEST PASS\n");
    return 0;
}