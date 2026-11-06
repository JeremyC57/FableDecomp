// Behaviour test for NScript::CQ_BountyHuntScript::Main (0x00D13580): seven bindings, the fisherman quest deactivated only when it is
// active and its region is not loaded, the intro cutscene, the entrance wait (only GreatwoodEntrance reached from the lookout point
// counts), the second intro, then the teleport loop (a stored gang member is re-created when the hero enters the region it was sent
// to under that region's flags, and its record is cleared), finally the fail text (intro / hostage 1 / hostage 2) or completion with
// the time-limit flag. A simulation of the same control flow with a scripted game.
#include <cstdio>
#include <cstring>
struct TD { char to[24]; bool used; };
struct Sim {
    char log[2048]; int frame, termFrame, bindings, nTD, succeedAt, failAt, regionAt[8], nRegion, entranceAt;
    char regions[8][24]; TD td[4];
    bool fisherActive, fisherRegion, fishermanDeactivated, fishComplete, deputy1Killed, villagerKilled, villagerIntro, h1Killed, h2Killed, boastLimit, timeLeft, withinLimit, introComplete, failed, succeeded;
    bool T() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void NSF() { ++frame; if (frame == succeedAt) succeeded = true; if (frame == failAt) failed = true; }
    const char* Msg() { for (int i = 0; i < nRegion; ++i) if (regionAt[i] == frame) return regions[i]; return 0; }
    void Main()
    {
        bindings = 7; Ev("BOASTS");
        if (fisherActive && !fisherRegion) { if (T()) return; Ev("DEACTFISHER"); fishermanDeactivated = true; }
        Ev("OBJ01");
        if (T()) return;
        Ev("INTRO");
        if (T()) return;
        while (frame < entranceAt) { NSF(); if (T()) return; }
        introComplete = true; Ev("WATCHERS"); Ev("INTRO2"); Ev("OBJ02");
        if (T()) return;
        while (!succeeded && !failed) {
            const char* r = Msg();
            if (r) {
                if (T()) return;
                for (int i = 0; i < nTD; ++i) {
                    bool go = (!std::strcmp(r, "GreatwoodEntrance") && !std::strcmp(td[i].to, r) && fishComplete)
                           || (!std::strcmp(r, "GreatwoodLake") && !std::strcmp(td[i].to, r) && fishComplete && deputy1Killed)
                           || (!std::strcmp(r, "Fisherman") && !std::strcmp(td[i].to, r));
                    if (go) { if (T()) return; Ev("CREATE"); td[i].to[0] = 0; td[i].used = true; }
                }
            }
            NSF(); if (T()) return;
        }
        if (T()) return;
        if (fishermanDeactivated) Ev("ACTFISHER");
        if (failed) {
            if (villagerKilled && !villagerIntro) Ev("FAIL_INTRO"); else if (h1Killed) Ev("FAIL_H1"); else if (h2Killed) Ev("FAIL_H2");
        } else if (succeeded) {
            if (boastLimit && timeLeft) withinLimit = true;
            Ev("MORALITY"); Ev("COMPLETED");
        }
        Ev("ALLGRUNTS"); Ev("GROUPSON"); Ev("DEACTLATER");
    }
};
static void Make(Sim& s)
{
    std::memset(&s, 0, sizeof(s)); s.termFrame = 1000; s.entranceAt = 3; s.succeedAt = 12; s.failAt = 0;
    s.nTD = 2; std::strcpy(s.td[0].to, "Fisherman"); std::strcpy(s.td[1].to, "GreatwoodLake");
    s.nRegion = 2; s.regionAt[0] = 5; std::strcpy(s.regions[0], "Fisherman"); s.regionAt[1] = 8; std::strcpy(s.regions[1], "GreatwoodLake");
    s.fisherActive = true; s.boastLimit = true; s.timeLeft = true;
}
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = a.bindings == 7 && a.introComplete && a.td[0].used && !a.td[1].used && a.withinLimit
        && std::strcmp(a.log, "BOASTS;DEACTFISHER;OBJ01;INTRO;WATCHERS;INTRO2;OBJ02;CREATE;ACTFISHER;MORALITY;COMPLETED;ALLGRUNTS;GROUPSON;DEACTLATER;") == 0;
    Sim b; Make(b); b.fisherRegion = true; b.fishComplete = true; b.deputy1Killed = true; b.succeedAt = 0; b.failAt = 10; b.h2Killed = true; b.Main();
    ok = ok && b.td[1].used && std::strstr(b.log, "FAIL_H2;") && !std::strstr(b.log, "DEACTFISHER") && !b.withinLimit;
    Sim c; Make(c); c.termFrame = 2; c.Main();
    ok = ok && !c.introComplete && std::strcmp(c.log, "BOASTS;DEACTFISHER;OBJ01;INTRO;") == 0;
    std::printf(ok ? "00d13580_TEST PASS\n" : "00d13580_TEST FAIL\n");
    return ok ? 0 : 1;
}
