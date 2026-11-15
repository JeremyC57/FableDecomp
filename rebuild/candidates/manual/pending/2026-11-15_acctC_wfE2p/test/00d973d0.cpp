// Behaviour test for NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main (0x00D973D0): a state model of the generated script's
// decisions. Caged: calls for help each time the timer runs out until the side-room hobbes are killed. Cut scene: the let-out
// question; a yes lets him out, a no repeats the question each time the hero talks to him. Following: until the brat is released or
// the caves are left (the region test is NEGATED: he follows while GreatwoodCaves is NOT loaded). Nymph loop: while the nymph lives he
// is sacrificed once (health bar removed), the cage effect is re-created when it died, and hero hits are counted; when she dies a
// sacrificed bandit is rescued (red health bar again, CRGBColour(255, 0, 0, 255)). Out of the caves: with treasure, the share question
// reuses the SAME yes flag (reset to false first): yes and enough gold -> pays and leaves; otherwise (no, or not enough gold: "lost
// money" only when he said yes) he turns hostile; without treasure he just leaves. Termination stops the script at any frame.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Bandit {
    int Frame, TerminateAt;
    int KilledAt;            // frame the side-room hobbes die
    int CallDelay, Timer;    // trapped call-for-help timer
    long Answers[4]; int AnswerNo;   // answers to the questions in order (1 yes, 0 no)
    int TalkAt[4];           // frames the hero talks to him (repeat question)
    int LeaveAt;             // frame the follow loop ends (brat released / caves left)
    int NymphDeadAt;         // frame the nymph is dead
    int CageDeadAt;          // frame the cage effect dies once
    int HitAt[4]; int HitCount;
    int GotTreasure; long Gold, Demand;
    long HealthBarColour;    // packed R<<24|G<<16|B<<8|A of the last health bar
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (Timer > 0) --Timer; }
    long Ask() { return Answers[AnswerNo++]; }
    bool Talk() const { for (int i = 0; i < 4; ++i) if (TalkAt[i] == Frame) return true; return false; }
    bool Hit() const { for (int i = 0; i < 4; ++i) if (HitAt[i] == Frame) return true; return false; }
    void Bar(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { HealthBarColour = (long)r << 24 | (long)g << 16 | (long)b << 8 | a; Log("BAR"); }
    void Main()
    {
        NSF(); if (T()) return;
        while (Frame < KilledAt) { NSF(); if (T()) return; if (Timer == 0) { Log("TRAPPED"); Timer = CallDelay; } }
        if (T()) return;
        bool saidYes = false;
        { Log("Q_LETOUT"); if (Ask() == 1) { if (T()) return; saidYes = true; } }
        while (!saidYes) {
            NSF(); if (T()) return;
            while (!Talk()) { NSF(); if (T()) return; }
            if (T()) return;
            Log("Q_LETOUT"); if (Ask() == 1) { if (T()) return; saidYes = true; }
        }
        if (T()) return;
        Bar(255, 0, 0, 255);
        while (Frame < LeaveAt && true) { NSF(); if (T()) return; }
        if (T()) return;
        bool sacrificed = false;
        for (; !false; NSF()) {
            if (T()) return;
            if (Frame >= NymphDeadAt) break;
            if (!sacrificed) { if (T()) return; sacrificed = true; Log("SACRIFICE"); }
            if (Frame == CageDeadAt) { if (T()) return; Log("CAGE"); }
            if (Hit()) { if (T()) return; ++HitCount; }
        }
        if (T()) return;
        if (sacrificed) { if (T()) return; Log("RESCUED"); Bar(255, 0, 0, 255); }
        if (T()) return;
        saidYes = false;
        if (GotTreasure > 0) {
            if (T()) return;
            Log("Q_SHARE"); if (Ask() == 1) { if (T()) return; saidYes = true; }
            if (saidYes && Gold > Demand) { Gold -= Demand; Log("PAID"); }
            else { if (saidYes) Log("LOST_MONEY"); Log("HOSTILE"); }
        } else { Log("NO_MONEY"); }
    }
};

static Bandit Make()
{
    g_log[0] = 0;
    Bandit b; std::memset(&b, 0, sizeof b);
    b.TerminateAt = 1000; b.KilledAt = 5; b.CallDelay = 3; b.LeaveAt = 0; b.NymphDeadAt = 0; b.CageDeadAt = -1;
    for (int i = 0; i < 4; ++i) { b.TalkAt[i] = -1; b.HitAt[i] = -1; }
    b.Demand = 100;
    return b;
}

int main()
{
    bool ok = true;
    // let out at once, nymph already dead, treasure: yes and enough gold -> pays
    Bandit a = Make(); a.Answers[0] = 1; a.Answers[1] = 1; a.GotTreasure = 1; a.Gold = 150; a.Main();
    ok = ok && std::strcmp(g_log, "TRAPPED,TRAPPED,Q_LETOUT,BAR,Q_SHARE,PAID,") == 0 && a.Gold == 50 && a.HealthBarColour == (long)0xFF0000FF;
    // let out at once, then NO to the share: the flag is reset, so no payment and no "lost money" (mutant without the reset pays)
    Bandit b = Make(); b.Answers[0] = 1; b.Answers[1] = 0; b.GotTreasure = 1; b.Gold = 150; b.Main();
    ok = ok && std::strcmp(g_log, "TRAPPED,TRAPPED,Q_LETOUT,BAR,Q_SHARE,HOSTILE,") == 0 && b.Gold == 150;
    // no in the cut scene, asked again on talk; yes but too poor -> lost money, hostile
    Bandit c = Make(); c.Answers[0] = 0; c.Answers[1] = 1; c.Answers[2] = 1; c.TalkAt[0] = 8; c.GotTreasure = 2; c.Gold = 50; c.Main();
    ok = ok && std::strcmp(g_log, "TRAPPED,TRAPPED,Q_LETOUT,Q_LETOUT,BAR,Q_SHARE,LOST_MONEY,HOSTILE,") == 0 && c.Frame == 8;
    // sacrificed while the nymph lives, cage re-created once, two hits counted, rescued; no treasure
    Bandit d = Make(); d.Answers[0] = 1; d.LeaveAt = 6; d.NymphDeadAt = 10; d.CageDeadAt = 8; d.HitAt[0] = 7; d.HitAt[1] = 9; d.HitAt[2] = 12; d.Main();
    ok = ok && std::strcmp(g_log, "TRAPPED,TRAPPED,Q_LETOUT,BAR,SACRIFICE,CAGE,RESCUED,BAR,NO_MONEY,") == 0 && d.HitCount == 2;
    // termination inside the let-out repeat loop stops everything
    Bandit e = Make(); e.Answers[0] = 0; e.TerminateAt = 7; e.Main();
    ok = ok && std::strcmp(g_log, "TRAPPED,TRAPPED,Q_LETOUT,") == 0 && e.Frame == 7;
    if (!ok) { std::printf("%s 00d973d0_TEST FAIL\n", g_log); return 1; }
    std::printf("00d973d0_TEST PASS\n");
    return 0;
}
