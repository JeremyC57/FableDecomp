// Behaviour test for NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main (0x00DC4D70): the graveyard gate man.
// Until the gravekeeper is introduced or he has been talked to: a talk says GATEMAN_ASIDE; a hero hit (heal-life excepted) marks him and
// VisitorsAttacked, runs him to GraveFriendLeavingMarker with PRE_DOOR_ATTACKED and fades him out; else, once VisitorsAttacked, he runs
// with PRE_DOOR_RUN_AWAY and fades. Then he walks to the marker, answering talk with ASIDE_LEAVING_FIRST then _SECOND, still reacts to
// hits / VisitorsAttacked the same way, and fades out when the walk ends. A state model, run for the walk-out, hit and run-away paths.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct GateMan {
    int Frames, TerminateAt, IntroducedAt, AttackedAt, ArriveAt;
    int Talks[8]; int NTalk; int Hits[8]; int NHit;
    bool GravekeeperIntroduced, VisitorsAttacked, AttackedByHero, Conscious, Walking;
    int TalkSeen;   // a talk message is consumed by the first check in its frame
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames == IntroducedAt) GravekeeperIntroduced = true;
        if (Frames == AttackedAt) VisitorsAttacked = true;
        if (Walking && Frames >= ArriveAt) Walking = false;
    }
    bool T() const { return Frames >= TerminateAt || !Conscious; }
    bool Talked() { if (TalkSeen == Frames || !In(Talks, NTalk)) return false; TalkSeen = Frames; return true; }
    bool In(const int* v, int n) const { for (int i = 0; i < n; ++i) if (v[i] == Frames) return true; return false; }
    void Flee(const char* line, bool markVisitors)
    {
        AttackedByHero = true;
        if (markVisitors) VisitorsAttacked = true;
        Log("run"); Log(line); Log("fade"); Conscious = false;
    }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        bool HaveTalked = false;
        while (!GravekeeperIntroduced && !HaveTalked) {
            NewScriptFrame();
            if (T()) return;
            if (Talked()) { Log("ASIDE"); HaveTalked = true; }
            if (In(Hits, NHit)) Flee("PRE_DOOR_ATTACKED", true);
            else if (VisitorsAttacked) Flee("PRE_DOOR_RUN_AWAY", false);
        }
        if (T()) return;
        Log("walk"); Walking = true;
        bool HaveSpoken = false;
        if (T()) return;
        while (true) {
            if (Talked()) {
                if (HaveSpoken) Log("LEAVING_SECOND");
                else { Log("LEAVING_FIRST"); HaveSpoken = true; }
                Log("walk");
            }
            if (In(Hits, NHit)) Flee("PRE_DOOR_ATTACKED", true);
            else if (VisitorsAttacked) Flee("PRE_DOOR_RUN_AWAY", false);
            if (!Walking && Conscious) { Log("fade"); Conscious = false; }
            NewScriptFrame();
            if (T()) return;
        }
    }
};
int main()
{
    GateMan a = { 0, 100, -1, -1, 8, { 3, 5, 6 }, 3, { 0 }, 0, false, false, false, true, false, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "ASIDE;walk;LEAVING_FIRST;walk;LEAVING_SECOND;walk;fade;") && !a.AttackedByHero && a.Frames == 9;
    g_log[0] = 0;
    GateMan b = { 0, 100, -1, -1, 99, { 0 }, 0, { 4 }, 1, false, false, false, true, false, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "run;PRE_DOOR_ATTACKED;fade;") && b.VisitorsAttacked && b.AttackedByHero;
    g_log[0] = 0;
    GateMan c = { 0, 100, 3, 5, 99, { 0 }, 0, { 0 }, 0, false, false, false, true, false, 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "walk;run;PRE_DOOR_RUN_AWAY;fade;") && c.AttackedByHero && c.Frames == 6;
    std::printf(ok ? "00dc4d70_TEST PASS\n" : "00dc4d70_TEST FAIL\n");
    return ok ? 0 : 1;
}
