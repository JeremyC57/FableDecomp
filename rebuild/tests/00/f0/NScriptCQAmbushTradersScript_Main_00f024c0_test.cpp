// Behaviour test for NScript::CQ_AmbushTradersScript::Main (0x00F024C0): ten persistent bindings, nine watcher threads, the first
// objective, DoQuest, then a wait for failure or success. Failure picks the contact-dead / spy-dead / spy-escaped reason in that order.
// Success completes the quest and waits for the spy to leave or a region to load. Either way WatchForRegionClear is spawned, the region
// is waited out, the three phase sub-quests and the quest are deactivated, and three regions' creature generators are switched back on.
#include <cstdio>
#include <string>
struct Sim {
    std::string log; int frame, termFrame, bindings, threads;
    bool QuestFailed, QuestSucceeded, ContactDead, SpyDead, SpyEscaped, SpyExited, RegionClear;
    int endFrame, regionFrame, clearFrame;
    bool Term() const { return frame >= termFrame; }
    bool RegionLoaded() const { return frame == regionFrame; }
    void Ev(const char* e) { log += e; log += ';'; }
    void Frame() { ++frame; if (frame == endFrame) { if (ContactDead || SpyDead || SpyEscaped) QuestFailed = true; else QuestSucceeded = true; } if (frame >= clearFrame) RegionClear = true; }
    void Main()
    {
        bindings = 10; Ev("POST"); threads = 9; Ev("OBJ01"); ++threads;
        while (!QuestFailed && !QuestSucceeded) { Frame(); if (Term()) return; }
        if (Term()) return;
        if (QuestFailed) {
            if (Term()) return;
            if (ContactDead) { if (Term()) return; Ev("FAIL_CONTACT_DEAD"); }
            else if (SpyDead) { if (Term()) return; Ev("FAIL_SPY_DEAD"); }
            else if (SpyEscaped) { if (Term()) return; Ev("FAIL_SPY_ESCAPED"); }
            else if (Term()) return;
        } else if (QuestSucceeded) {
            if (Term()) return;
            Ev("COMPLETE");
            while (!SpyExited && !RegionLoaded()) { Frame(); if (Term()) return; }
            if (Term()) return;
        }
        ++threads;
        while (!RegionClear && !RegionLoaded()) { Frame(); if (Term()) return; }
        if (Term()) return;
        Ev("DEACT1"); Ev("DEACT2"); Ev("DEACT3"); Ev("DEACT_QUEST"); Ev("GEN_JAIL"); Ev("GEN_GIBBET"); Ev("GEN_FARM");
    }
};
static Sim Make()
{
    Sim s; s.frame = 0; s.termFrame = 1000; s.bindings = s.threads = 0;
    s.QuestFailed = s.QuestSucceeded = s.ContactDead = s.SpyDead = s.SpyEscaped = s.SpyExited = s.RegionClear = false;
    s.endFrame = 3; s.regionFrame = 6; s.clearFrame = 9;
    return s;
}
int main()
{
    Sim a = Make(); a.Main();
    bool ok = a.log == "POST;OBJ01;COMPLETE;DEACT1;DEACT2;DEACT3;DEACT_QUEST;GEN_JAIL;GEN_GIBBET;GEN_FARM;" && a.frame == 6
        && a.bindings == 10 && a.threads == 11;
    Sim b = Make(); b.SpyDead = true; b.SpyEscaped = true; b.regionFrame = 100; b.Main();
    ok = ok && b.log == "POST;OBJ01;FAIL_SPY_DEAD;DEACT1;DEACT2;DEACT3;DEACT_QUEST;GEN_JAIL;GEN_GIBBET;GEN_FARM;" && b.frame == 9;
    Sim c = Make(); c.termFrame = 2; c.Main();
    ok = ok && c.log == "POST;OBJ01;" && c.threads == 10;
    std::printf(ok ? "00f024c0_TEST PASS\n" : "00f024c0_TEST FAIL\n");
    return ok ? 0 : 1;
}