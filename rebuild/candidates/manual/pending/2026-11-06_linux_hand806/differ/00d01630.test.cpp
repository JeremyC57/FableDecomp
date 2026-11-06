// Behaviour test for NScript::CQ_BanditCampScript::CGate1GuardOuter::Main (0x00D01630): the outer guard at the bandit camp's first
// gate. While Gate1 is shut he calls the hero over every ten seconds (CALL_OVER_FIRST once, then CALL_OVER_SECOND); talked to, he
// checks the disguise (NO_DISGUISE when nothing is worn, else the first missing piece: shirt, trousers, hat, boots, gloves), and a
// fully disguised hero gets TRIED_AND_IN (after a failed try) or STRAIGHT_IN, BANDITS_SLEEPING at sleeping time, GATE_OPEN, the
// opened gate, objective 05, Gate1Open and medium experience. Once the gate is open a talk gets ASIDE. Hit by the hero (or once
// the outer guards were attacked) he flags the attack and fights the hero for good. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { SHIRT = 1, TROUSERS = 2, HAT = 4, BOOTS = 8, GLOVES = 16 };
struct Guard {
    int Frames, TerminateAt, Timer; int TalkAt[4]; int NTalk; int WornAt[4]; int HitAt; bool Sleeping;
    bool Gate1Open, Attacked, CalledOverOnce, TriedSomething; int Experience;
    bool T() const { return Frames >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) return true; return false; }
    int Worn() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) return WornAt[i]; return 0; }
    void Main()
    {
        Timer = 0;
        for (;; ++Frames, Timer > 0 ? --Timer : 0) {
            if (T()) return;
            if (!Gate1Open) {
                if (Timer <= 0) { Log(CalledOverOnce ? "CALL_OVER_SECOND" : "CALL_OVER_FIRST"); CalledOverOnce = true; Timer = 10; }
                if (Talked()) {
                    int w = Worn();
                    if (!(w & (SHIRT | TROUSERS | HAT | BOOTS | GLOVES))) { Log("NO_DISGUISE"); TriedSomething = true; }
                    else if (!(w & SHIRT)) { Log("NO_SHIRT"); TriedSomething = true; }
                    else if (!(w & TROUSERS)) { Log("NO_TROUSERS"); TriedSomething = true; }
                    else if (!(w & HAT)) { Log("NO_HAT"); TriedSomething = true; }
                    else if (!(w & BOOTS)) { Log("NO_BOOTS"); TriedSomething = true; }
                    else if (!(w & GLOVES)) { Log("NO_GLOVES"); TriedSomething = true; }
                    else {
                        Log(TriedSomething ? "TRIED_AND_IN" : "STRAIGHT_IN");
                        if (Sleeping) Log("BANDITS_SLEEPING");
                        Log("GATE_OPEN"); Log("open_gate1"); Log("objective_05");
                        Gate1Open = true; Experience += 2;
                    }
                }
            } else if (Talked()) Log("ASIDE");
            if (Frames == HitAt) { Log("ATTACKED_NEW"); Attacked = true; }
            if (Attacked) { Log("fight"); for (;; ++Frames) if (T()) return; }
        }
    }
};
int main()
{
    Guard a = { 0, 30, 0, { 2, 4, 6 }, 3, { 0, SHIRT | TROUSERS | BOOTS, SHIRT | TROUSERS | HAT | BOOTS | GLOVES }, -1, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "CALL_OVER_FIRST;NO_DISGUISE;NO_HAT;TRIED_AND_IN;BANDITS_SLEEPING;GATE_OPEN;open_gate1;objective_05;")
        && a.Gate1Open && a.Experience == 2;
    g_log[0] = 0;
    Guard b = { 0, 30, 0, { 1, 12 }, 2, { SHIRT | TROUSERS | HAT | BOOTS | GLOVES, 0 }, 15, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "CALL_OVER_FIRST;STRAIGHT_IN;GATE_OPEN;open_gate1;objective_05;ASIDE;ATTACKED_NEW;fight;") && b.Attacked && b.Frames == 30;
    g_log[0] = 0;
    Guard c = { 0, 25, 0, { 0 }, 0, { 0 }, -1, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "CALL_OVER_FIRST;CALL_OVER_SECOND;CALL_OVER_SECOND;") && !c.Gate1Open;
    std::printf(ok ? "00d01630_TEST PASS\n" : "00d01630_TEST FAIL\n");
    return ok ? 0 : 1;
}
