// Behaviour test for NScript::CV_HeroDuelScript::DuelChallenge 0x00E9BBC0
#include <cstdio>
#include <string>
static std::string g_log;
static void Log(const std::string& s) { g_log += s; g_log += ';'; }
class CGame { public:
    int Frames;
    int ScriptableAfter;                                // StartScriptingEntity succeeds from this frame on
    bool StartScriptingEntity() { Log("start"); return Frames >= ScriptableAfter; }
    void NewScriptFrame() { ++Frames; }
    void StartMovieSequence() { Log("movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
};
struct CMasterData { bool WhisperKilledByHero; };
struct CV_HeroDuelScript {
    CGame* Game;
    CMasterData* MasterScriptData;
    int TerminateAtFrame;
    bool IsActiveThreadTerminating() { return TerminateAtFrame >= 0 && Game->Frames >= TerminateAtFrame; }
    void DuelChallenge()
    {
        while (!Game->StartScriptingEntity()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->StartMovieSequence();
        CGame* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        if (MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            Log("CS_GIBBETHILL_HERODUEL_CHALLENGE_W");
        } else {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            Log("CS_GIBBETHILL_HERODUEL_CHALLENGE");
        }
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
};
int main()
{
    CGame g; CMasterData m; CV_HeroDuelScript s;
    s.Game = &g; s.MasterScriptData = &m; s.TerminateAtFrame = -1;
    g.Frames = 0; g.ScriptableAfter = 2; m.WhisperKilledByHero = true;
    s.DuelChallenge();
    if (g_log != "start;start;start;movie;pause;fix;CS_GIBBETHILL_HERODUEL_CHALLENGE_W;unfix;unpause;") { std::printf("00e9bbc0_TEST FAIL\n"); return 1; }
    g_log = ""; g.Frames = 0; g.ScriptableAfter = 0; m.WhisperKilledByHero = false;
    s.DuelChallenge();
    if (g_log != "start;movie;pause;fix;CS_GIBBETHILL_HERODUEL_CHALLENGE;unfix;unpause;") { std::printf("00e9bbc0_TEST FAIL\n"); return 1; }
    g_log = ""; g.Frames = 0; g.ScriptableAfter = 5; s.TerminateAtFrame = 1;
    s.DuelChallenge();
    if (g_log != "start;") { std::printf("00e9bbc0_TEST FAIL\n"); return 1; }
    std::printf("00e9bbc0_TEST PASS\n");
    return 0;
}