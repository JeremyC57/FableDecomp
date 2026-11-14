// Behaviour test for NScript::CQ_RansomVictimScript::CVictimsBrother::Main (0x00DEA3F0): until the victim is rescued the brother
// faces a near hero (1 s timer), gives the remind line when talked to (only while his scripted entity has health), reacts to hero
// hits (a heal spell is not a hit) and, after more than one hit on him and his protection, runs to the marker and fades out; once
// rescued he gets a minimap marker, and talking to him while the victim is alive plays the evil outro (victim killed, mission
// succeeded, evil morality, brother removed).
#include <cstdio>
#include <cstring>
struct Sim {
    char log[1024]; int frame, termFrame, rescuedFrame, talkFrame1, talkFrame2, hitFrame, healFrame, runFrame, timer, distToMarker;
    bool heroNear, victimAlive, brotherHealthy, killable, missionSucceeded, victimKilled; int hits; long morality;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Frame() { ++frame; if (timer > 0) --timer; }
    bool Face() { if (timer == 0 && heroNear) { Ev("FACE"); timer = 1; } return Term(); }
    bool Hits()
    {
        bool hit = frame == hitFrame;   // a heal-spell hit (healFrame) does not count
        if (hit) { Ev("HITME"); ++hits; }
        if (frame == runFrame) hits = 2;
        if (hits > 1) {
            while (distToMarker > 5) { Frame(); if (Term()) return true; --distToMarker; }
            Ev("FADE"); hits = 0;
        }
        return false;
    }
    void Main()
    {
        Frame(); if (Term()) return;
        killable = false;
        while (frame < rescuedFrame) {
            Frame(); if (Term()) return;
            if (Face()) return;
            if (frame == talkFrame1 && brotherHealthy) Ev("REMIND");
            if (Hits()) return;
        }
        if (Term()) return;
        Ev("MARK");
        do {
            if (Face()) return;
            if (frame == talkFrame2) {
                if (victimAlive) { Ev("OUTRO"); victimKilled = true; missionSucceeded = true; morality -= 50; Ev("REMOVE"); }
                else if (brotherHealthy) Ev("REMIND");
            }
            if (Hits()) return;
            Frame();
        } while (!Term());
    }
};
static void Make(Sim& s)
{
    std::memset(&s, 0, sizeof s); s.termFrame = 20; s.rescuedFrame = 6; s.talkFrame1 = 3; s.talkFrame2 = 9; s.hitFrame = -1; s.healFrame = 2;
    s.runFrame = -1; s.brotherHealthy = true; s.victimAlive = true; s.killable = true; s.distToMarker = 8;
}
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = std::strcmp(a.log, "REMIND;MARK;OUTRO;REMOVE;") == 0 && a.victimKilled && a.missionSucceeded && a.morality == -50 && !a.killable;
    Sim b; Make(b); b.heroNear = true; b.victimAlive = false; b.hitFrame = 4; b.runFrame = 5; b.Main();
    ok = ok && std::strncmp(b.log, "FACE;FACE;REMIND;FACE;HITME;FACE;FADE;", 38) == 0 && !b.victimKilled;
    Sim c; Make(c); c.termFrame = 1; c.Main();
    ok = ok && c.log[0] == 0;
    std::printf(ok ? "00dea3f0_TEST PASS\n" : "00dea3f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
