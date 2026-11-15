// Behaviour test for NScript::CV_BodyGuardScript::CBodyGuard::Main (0x00E5FD40): a state model of the hired loop of the generated
// script. Per frame (stop on termination): override state 2 fires the henchman; when the hero hits him, the anger timer picks the
// warning (< 20 LOW, < 45 MEDIUM, < 70 HIGH which also un-hires him, else nothing) and only a warning that was actually spoken adds 30
// to the anger timer; the health state (critical / low / fine) is commented on a change (HEALED when it went up) and remembered only
// when the comment was spoken; summoner hits are commented once per 30 s; a new state group is commented (FOLLOWING / ATTACKING);
// when the hero cannot afford help the henchman says MADE_REDUNDANT, is un-hired and the loop ends.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Guard {
    int Frame, TerminateAt;
    int HitAt[4]; long Anger; bool Hired; long Health[16]; long Crit, Low; int CurHealth, PrevHealth;
    unsigned SilentMask;            // comment N (in call order) fails to be spoken when bit N is set
    int CommentNo; int SummonerAt; long SummonerTimer; int Group[16]; long LastGroup; int BrokeAt;
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (Anger > 0) --Anger; if (SummonerTimer > 0) --SummonerTimer; }
    bool Comment(const char* s) { bool said = !((SilentMask >> CommentNo++) & 1); if (said) Log(s); return said; }
    bool Hit() const { for (int i = 0; i < 4; ++i) if (HitAt[i] == Frame) return true; return false; }
    void Main()
    {
        for (; !false; NSF()) {
            if (T()) return;
            if (Hit()) {
                bool commented = false;
                if (Anger < 20) commented = Comment("WARNING_LOW");
                else if (Anger < 45) commented = Comment("WARNING_MEDIUM");
                else if (Anger < 70) { if (Comment("WARNING_HIGH")) { Hired = false; Log("unhired"); commented = true; } }
                if (commented) Anger += 30;
            }
            long h = Health[Frame & 15];
            CurHealth = h < Crit ? 2 : h < Low ? 1 : 0;
            if (CurHealth != PrevHealth) {
                const char* c = CurHealth < PrevHealth ? "HEALED" : CurHealth == 0 ? "HEALTH_FINE" : CurHealth == 1 ? "HEALTH_LOW" : "HEALTH_VERY_LOW";
                if (Comment(c)) PrevHealth = CurHealth;
            }
            if (SummonerTimer == 0 && Frame == SummonerAt) { if (Comment("SUMMONERS")) SummonerTimer = 30; }
            long g = Group[Frame & 15];
            if (LastGroup != g) { if (g == 2) Comment("FOLLOWING"); else if (g == 1) Comment("ATTACKING"); }
            LastGroup = g;
            if (Frame == BrokeAt) { Log("MADE_REDUNDANT"); Hired = false; break; }
        }
    }
};

static Guard Make(int terminateAt)
{
    g_log[0] = 0;
    Guard g; std::memset(&g, 0, sizeof g);
    g.TerminateAt = terminateAt; for (int i = 0; i < 4; ++i) g.HitAt[i] = -1; g.Hired = true;
    for (int i = 0; i < 16; ++i) g.Health[i] = 100;
    g.Crit = 20; g.Low = 50; g.SummonerAt = -1; g.BrokeAt = -1;
    return g;
}

int main()
{
    bool ok = true;
    Guard a = Make(6); a.HitAt[0] = 1; a.HitAt[1] = 2; a.HitAt[2] = 3; a.Main();                  // LOW (+30), MEDIUM (+30), HIGH un-hires (+30)
    ok = ok && std::strcmp(g_log, "WARNING_LOW,WARNING_MEDIUM,WARNING_HIGH,unhired,") == 0 && !a.Hired && a.Anger == 85;
    Guard b = Make(4); b.HitAt[0] = 1; b.HitAt[1] = 2; b.SilentMask = 1; b.Main();                // unspoken LOW adds nothing
    ok = ok && std::strcmp(g_log, "WARNING_LOW,") == 0 && b.Anger == 28;
    Guard c = Make(3); c.Anger = 80; c.HitAt[0] = 1; c.Main();                                     // beyond HIGH: silent
    ok = ok && g_log[0] == 0 && c.Anger == 77 && c.Hired;
    Guard d = Make(3); d.Anger = 60; d.HitAt[0] = 0; d.SilentMask = 1; d.Main();                   // unspoken HIGH keeps him hired
    ok = ok && g_log[0] == 0 && d.Hired && d.Anger == 57;
    Guard e = Make(6); e.Health[1] = 40; e.Health[2] = 10; e.SilentMask = 2; e.Main();
    ok = ok && std::strcmp(g_log, "HEALTH_LOW,HEALED,") == 0 && e.PrevHealth == 0;                // VERY_LOW unspoken (state not kept), then healed
    Guard f = Make(5); f.SummonerAt = 2; f.Group[1] = 2; f.Group[2] = 2; f.Group[3] = 1; f.Main();
    ok = ok && std::strcmp(g_log, "FOLLOWING,SUMMONERS,ATTACKING,") == 0 && f.SummonerTimer == 27;
    Guard g = Make(9); g.BrokeAt = 2; g.HitAt[0] = 3; g.Main();                                   // redundant ends the loop
    ok = ok && std::strcmp(g_log, "MADE_REDUNDANT,") == 0 && !g.Hired && g.Frame == 2;
    if (!ok) { std::printf("%s 00e5fd40_TEST FAIL\n", g_log); return 1; }
    std::printf("00e5fd40_TEST PASS\n");
    return 0;
}