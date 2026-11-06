// Behaviour test for NScript::CQ_EndGameFocalSitesScript::Main (0x00D2AA40): guild master quest check, guild complex wait and
// lock-down, eight bindings, then for regions 1..4: the region wait + objective (2-4), the guild master line (2-4), the battle loop
// (generator re-teleported next to the hero; a guard family while fewer than 3 guards, a region-specific minion family while fewer
// than 3 minions; counter wraps at 3) until the end cutscene plays, Jack removed when alive; finally the closing line and the quest
// completed or failed. A simulation of the same control flow with a scripted game.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[4096]; int frame, termFrame, bindings, region, counter, minions, guards, randSeq[64], nRand, cutsceneAt[5], spawned;
    bool questActive, missionFailed, endCutscene, jackAlive;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Frame() { ++frame; }
    int Rand() { return randSeq[nRand++ % 64]; }
    bool RegionLoaded(int r) const { return frame >= r; }
    void Main()
    {
        if (!questActive) { if (Term()) return; Ev("ACT"); }
        while (!RegionLoaded(1)) { Frame(); if (Term()) return; }
        if (Term()) return;
        Ev("LOCK");
        while (!RegionLoaded(2)) { Frame(); if (Term()) return; }
        if (Term()) return;
        bindings = 8;
        region = 1; missionFailed = false;
        do {
            Frame();
            if (Term()) return;
            if (region >= 2) { if (Term()) return; Ev(region == 2 ? "OBJ2" : region == 3 ? "OBJ3" : "OBJ4"); }
            if (region != 1) { if (Term()) return; Ev("GM"); }
            endCutscene = false; counter = 0;
            do {
                Frame();
                if (Term()) return;
                if (frame >= cutsceneAt[region]) endCutscene = true;
                if (endCutscene) break;
                if (Term()) return;
                int nMinions = minions, nGuards = guards;
                if (nGuards < 3) { if (Term()) return; Rand(); Ev(Rand() % 2 == 0 ? "APPRENTICE" : "GUARD"); ++guards; ++spawned; }
                if (nMinions < 3) {
                    if (Term()) return;
                    Rand();
                    switch (region) {
                    case 1: { int f = Rand() % 4; Ev(f == 0 ? "LSWORD" : f == 1 ? "MINION" : f == 2 ? "USWORD" : "UAXE"); ++minions; ++spawned; break; }
                    case 2: { int f = Rand() % 4; Ev(f == 0 ? "SCREAMER" : f == 1 ? "MINION" : f == 2 ? "LAXE" : "UAXE"); ++minions; ++spawned; break; }
                    case 3: { int f = Rand() % 3; Ev(f == 0 ? "SCREAMER" : f == 1 ? "MINION" : "LAXE"); ++minions; ++spawned; break; }
                    case 4: Ev("MINION"); ++minions; ++spawned; break;
                    }
                }
                if (++counter >= 3) { if (Term()) return; counter = 0; }
            } while (!endCutscene);
            if (Term()) return;
            if (jackAlive) { if (Term()) return; Ev("REMOVEJACK"); jackAlive = false; }
            minions = guards = 0;
            ++region;
        } while (region <= 4);
        if (Term()) return;
        Ev("END");
        if (missionFailed) { if (Term()) return; Ev("FAILED"); } else { if (Term()) return; Ev("COMPLETED"); }
        Ev("DEACT");
    }
};
static void Make(Sim& s)
{
    std::memset(&s, 0, sizeof(s));
    s.termFrame = 1000; s.questActive = false; s.jackAlive = true;
    for (int i = 0; i < 64; ++i) s.randSeq[i] = i;
    s.cutsceneAt[1] = 6; s.cutsceneAt[2] = 9; s.cutsceneAt[3] = 12; s.cutsceneAt[4] = 15;
}
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = a.bindings == 8 && a.region == 5 && std::strstr(a.log, "ACT;LOCK;") == a.log
        && std::strstr(a.log, "OBJ2;GM;") && std::strstr(a.log, "OBJ4;GM;") && std::strstr(a.log, "REMOVEJACK;")
        && std::strstr(a.log, "END;COMPLETED;DEACT;") && a.spawned > 0;
    Sim b; Make(b); b.questActive = true; b.termFrame = 4; b.Main();
    ok = ok && std::strcmp(b.log, "LOCK;") == 0 && b.region == 1;
    Sim c; Make(c); c.missionFailed = true; c.Main();
    ok = ok && std::strstr(c.log, "END;COMPLETED;") != 0;   // Main clears MissionFailed before the regions
    std::printf(ok ? "00d2aa40_TEST PASS\n" : "00d2aa40_TEST FAIL\n");
    return ok ? 0 : 1;
}