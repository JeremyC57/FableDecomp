// Behaviour test for NScript::Cbandit_level_cutsceneScript::Main (0x00EFA4F0): a copy of the Main logic over a mock game that logs
// calls; checks per pass the bandits-region wait, the welcome_guy binding, the paused camera flight (preload SPLINE_01..05, then
// SPLINE_04 / 03 / 01 each waited out), the reset behind the hero, the 3 s pause and the unload wait; and the early terminate path.
#include <cstdio>
#include <cstring>

struct MockGame {
    char log[96][32]; int n, frames, terminateAt, scripted; bool loaded[64];
    void Log(const char* s) { if (n < 96) { std::strncpy(log[n], s, 31); log[n][31] = 0; ++n; } }
    void NewScriptFrame() { ++frames; if (scripted > 0) --scripted; }
    bool IsRegionLoaded() { return frames < 64 && loaded[frames]; }
    void Cam(const char* s, bool wait) { Log(s); if (wait) scripted = 2; }
    bool IsCameraInScriptedMode() { return scripted > 0; }
};
struct Script {
    MockGame* Game; int bindings;
    bool T() const { return Game->terminateAt && Game->frames >= Game->terminateAt; }
    void Main()
    {
        for (;; Game->NewScriptFrame()) {
            if (T()) return;
            while (!Game->IsRegionLoaded()) { Game->NewScriptFrame(); if (T()) return; }
            if (T()) return;
            ++bindings; Game->Log("welcome_guy");
            {
                Game->Log("Movie"); Game->Log("Preload1");
                Game->Cam("SPLINE_01", false); Game->Cam("SPLINE_02", false); Game->Cam("SPLINE_03", false); Game->Cam("SPLINE_04", false); Game->Cam("SPLINE_05", false);
                Game->Log("Preload0");
                Game->Cam("SPLINE_04", true);
                while (Game->IsCameraInScriptedMode()) { Game->NewScriptFrame(); if (T()) return; }
                if (T()) return;
                Game->Cam("SPLINE_03", true);
                while (Game->IsCameraInScriptedMode()) { Game->NewScriptFrame(); if (T()) return; }
                if (T()) return;
                Game->Cam("SPLINE_01", true);
                while (Game->IsCameraInScriptedMode()) { Game->NewScriptFrame(); if (T()) return; }
                if (T()) return;
                Game->Log("ResetBehindHero"); Game->Log("CameraDefault"); Game->Log("Pause3");
            }
            while (Game->IsRegionLoaded() != false) { Game->NewScriptFrame(); if (T()) return; }
            if (T()) return;
        }
    }
};
static int Fail(int n) { std::printf("00efa4f0_TEST FAIL %d\n", n); return 1; }
int main()
{
    static const char* pass[] = { "welcome_guy", "Movie", "Preload1", "SPLINE_01", "SPLINE_02", "SPLINE_03", "SPLINE_04", "SPLINE_05", "Preload0",
        "SPLINE_04", "SPLINE_03", "SPLINE_01", "ResetBehindHero", "CameraDefault", "Pause3" };
    MockGame g; std::memset(&g, 0, sizeof g);
    for (int f = 2; f < 12; ++f) g.loaded[f] = true;      // first visit: loaded frames 2..11, unloads at 12
    for (int f = 20; f < 30; ++f) g.loaded[f] = true;     // second visit
    g.terminateAt = 40;
    Script s; s.Game = &g; s.bindings = 0; s.Main();
    if (s.bindings != 2 || g.n != 30) return Fail(1);
    for (int i = 0; i < 30; ++i) if (std::strcmp(g.log[i], pass[i % 15]) != 0) return Fail(2);
    MockGame e; std::memset(&e, 0, sizeof e); e.loaded[0] = true; e.terminateAt = 2;
    Script t; t.Game = &e; t.bindings = 0; t.Main();
    if (t.bindings != 1 || e.n != 10 || std::strcmp(e.log[9], "SPLINE_04") != 0) return Fail(3);
    std::printf("00efa4f0_TEST PASS\n");
    return 0;
}
