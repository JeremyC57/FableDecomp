#include <cstdio>
// Behaviour test for NScript::CQ_HeroSoulsNostroScript::CNostro::Main (0x00D89380): Nostro is made invulnerable and
// faded, waits for the vulnerability timer, becomes vulnerable and visible until the timer runs out, and the loop
// stops as soon as the thread terminates.
static int g_frames = 0, g_stopAt = 0, g_invuln = 0, g_vuln = 0, g_sounds = 0, g_fadeIn = 0, g_fadeOut = 0;
static long g_timer[64];
struct Game {
    void NewScriptFrame() { ++g_frames; }
    long GetTimer(long frame) { return g_timer[frame < 64 ? frame : 63]; }
    void SetVulnerable(bool on) { if (on) ++g_vuln; else ++g_invuln; }
    void PlaySound() { ++g_sounds; }
};
static Game g_game;
struct CNostro {
    bool Terminating() { return g_frames >= g_stopAt; }
    void Main()
    {
        g_game.NewScriptFrame();
        if (Terminating()) return;
        while (!Terminating()) {
            g_game.SetVulnerable(false); ++g_fadeOut; g_game.PlaySound();
            while (g_game.GetTimer(g_frames) <= 0) { g_game.NewScriptFrame(); if (Terminating()) return; }
            if (Terminating()) return;
            g_game.SetVulnerable(true); ++g_fadeIn;
            while (g_game.GetTimer(g_frames)) { g_game.NewScriptFrame(); if (Terminating()) return; }
            if (Terminating()) return;
            g_game.NewScriptFrame();
        }
    }
};
int main()
{
    int bad = 0;
    for (int i = 0; i < 64; ++i) g_timer[i] = (i >= 3 && i < 6) ? 5 : 0;
    CNostro n;
    g_stopAt = 12;
    n.Main();
    bad |= g_invuln != 2 || g_vuln != 1 || g_sounds != 2 || g_fadeIn != 1 || g_fadeOut != 2 || g_frames != 12;
    g_frames = 0; g_stopAt = 1; g_invuln = g_vuln = g_sounds = 0;
    n.Main();
    bad |= g_invuln != 0 || g_sounds != 0 || g_frames != 1;
    if (bad) { std::printf("00d89380_TEST FAIL\n"); return 1; }
    std::printf("00d89380_TEST PASS\n");
    return 0;
}
