// Behaviour test for CPrisonRaceStartingGuard::Main (0x00DDD430): the intro cutscene is the short one on a repeat race
// (PrisonRaceNumber >= 1), then the race starts; a hero hit (not a heal-life spell) makes the guard hostile for 20 frames, every
// further hit renewing the timer; after that it clears the target and walks home until within 2 units.
#include <cstdio>

struct Guard {
    long RaceNumber; bool RaceStarted, Hostile; long Timer, HostileFrames; float Home;
    const char* Intro() { RaceStarted = true; return RaceNumber >= 1 ? "CS_PRISON_RACE_INTRO_SHORT" : "CS_PRISON_RACE_INTRO"; }
    void Hit(bool heal) { if (heal) return; Hostile = true; Timer = 20; }
    void Frame(bool hitAgain) { if (!Hostile) return; ++HostileFrames; if (hitAgain) Timer = 20; else --Timer; if (Timer <= 0) Hostile = false; }
    long WalkHome(float pos) { long steps = 0; while (pos - Home > 2.0f) { pos -= 1.0f; ++steps; } return steps; }
};

int main()
{
    Guard g = { 0, false, false, 0, 0, 0.0f };
    bool ok = g.Intro()[20] == 0 && g.RaceStarted;
    g.RaceNumber = 2; ok = ok && g.Intro()[20] == '_';
    g.Hit(true); ok = ok && !g.Hostile;
    g.Hit(false); for (int i = 0; i < 10; ++i) g.Frame(false);
    g.Frame(true); ok = ok && g.Hostile && g.Timer == 20;
    while (g.Hostile) g.Frame(false);
    ok = ok && g.HostileFrames == 31 && g.WalkHome(6.5f) == 5;
    if (!ok) { std::printf("00ddd430_TEST FAIL\n"); return 1; }
    std::printf("00ddd430_TEST PASS\n");
    return 0;
}
