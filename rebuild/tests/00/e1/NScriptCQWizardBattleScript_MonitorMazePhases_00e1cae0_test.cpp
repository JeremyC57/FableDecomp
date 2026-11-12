#include <cstdio>
// behaviour test: NScript::CQ_WizardBattleScript::MonitorMazePhases -- mark the maze on the minimap and as an evil opinion
// source once, then while the maze lives wait (one frame at a time, the hero kept more than 5 units away) for each boss-phase
// change, recording it in MazePhase; every script frame checks for thread termination
static int g_frame = 0, g_markers = 0, g_opinionSources = 0, g_phaseReads = 0, g_terminateAt = 100000;
struct Script {
    bool MazeDefeated; long MazePhase;
    bool IsActiveThreadTerminating() { return g_frame >= g_terminateAt; }
    void NewScriptFrame() { ++g_frame; }
    bool MazeAlive() { return g_frame < 400; }
    long BossPhase() { ++g_phaseReads; return g_frame < 100 ? 1 : g_frame < 250 ? 2 : 3; }
    bool HeroFarFromMaze() { return (g_frame / 10) % 3 != 0; }
    void MonitorMazePhases()
    {
        ++g_markers;
        MazePhase = BossPhase();
        ++g_opinionSources;
        while (MazeAlive()) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            long phase = MazePhase;
            do {
                NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                while (!HeroFarFromMaze()) {
                    NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                MazePhase = BossPhase();
            } while (MazePhase == phase);
            if (IsActiveThreadTerminating()) return;
            if (MazeAlive() && MazeDefeated == false) {
                if (IsActiveThreadTerminating()) return;
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    Script s; s.MazeDefeated = false; s.MazePhase = 0;
    s.MonitorMazePhases();
    if (g_markers != 1 || g_opinionSources != 1 || s.MazePhase != 3 || g_frame < 400 || g_phaseReads < 3) {
        std::printf("00e1cae0_TEST FAIL\n"); return 1;
    }
    // a terminating thread stops at the next frame check, mid-phase
    g_frame = 0; g_terminateAt = 50; g_phaseReads = 0;
    Script t; t.MazeDefeated = false; t.MazePhase = 0;
    t.MonitorMazePhases();
    if (g_frame != 50 || t.MazePhase != 1) { std::printf("00e1cae0_TEST FAIL\n"); return 1; }
    std::printf("00e1cae0_TEST PASS\n");
    return 0;
}