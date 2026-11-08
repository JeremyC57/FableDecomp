// Behaviour test for NScript::CQ_ArenaHoldingScriptScript::CBowerstoneFan::Main (0x00cf2b70): the Bowerstone arena fan. Until a fan is attacked
// or the arena is over: a hit sets PlayerAttackedFans; a talk says INTRO_FIRST then INTRO_SECOND, or once Q_Arena is completed
// WELL_DONE then WELL_DONE_REPEAT (one Spoken flag serves both: after an intro the completed-quest line is the REPEAT). Then, if attacked: wait for FansCanLeaveNow and run to FanLeavingMarker; otherwise walk
// there, answering talk (WELL_DONE then WELL_DONE_REPEAT) and hits (LEAVING_HIT once at a time, then run); fade out on arrival.
// A state model, run for the peaceful walk-out path, the attacked path and a terminate exit.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Fan {
    int Frames, TerminateAt, QuestDoneAt, ArenaOverAt, CanLeaveAt, ArriveAt;
    int Talks[8]; int NTalk; int Hits[8]; int NHit;
    bool PlayerAttackedFans, FansCanLeaveNow, ArenaOver, InHitCutsceneAlready, QuestDone, Moving;
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames == QuestDoneAt) QuestDone = true;
        if (Frames == ArenaOverAt) ArenaOver = true;
        if (Frames == CanLeaveAt) FansCanLeaveNow = true;
        if (Frames >= ArriveAt) Moving = false;
    }
    bool T() const { return Frames >= TerminateAt; }
    bool In(const int* v, int n) const { for (int i = 0; i < n; ++i) if (v[i] == Frames) return true; return false; }
    bool Talked() const { return In(Talks, NTalk); }
    bool Hit() const { return In(Hits, NHit); }
    void Main()
    {
        bool Spoken = false;
        while (!PlayerAttackedFans && !ArenaOver) {
            NewScriptFrame();
            if (T()) return;
            if (Hit()) PlayerAttackedFans = true;
            if (Talked()) {
                if (QuestDone) {
                    if (Spoken) Log("WELL_DONE_REPEAT");
                    else { Log("WELL_DONE"); Spoken = true; }
                } else if (Spoken) Log("INTRO_SECOND");
                else { Log("INTRO_FIRST"); Spoken = true; }
            }
        }
        if (T()) return;
        bool SpokenLeaving = false;
        if (PlayerAttackedFans) {
            while (!FansCanLeaveNow) { NewScriptFrame(); if (T()) return; }
            Log("run"); Moving = true;
            while (Moving) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("fade");
        } else {
            Log("walk"); Moving = true;
            while (Moving) {
                NewScriptFrame();
                if (T()) return;
                if (Talked()) {
                    if (SpokenLeaving) Log("WELL_DONE_REPEAT");
                    else { Log("WELL_DONE"); SpokenLeaving = true; }
                    Log("walk");
                }
                if (Hit()) {
                    if (!InHitCutsceneAlready) { InHitCutsceneAlready = true; Log("LEAVING_HIT"); InHitCutsceneAlready = false; }
                    Log("run");
                }
            }
            if (T()) return;
            Log("fade");
        }
    }
};
int main()
{
    Fan a = { 0, 100, 3, 6, -1, 9, { 2, 4, 5, 8 }, 4, { 7 }, 1, false, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "INTRO_FIRST;WELL_DONE_REPEAT;WELL_DONE_REPEAT;walk;LEAVING_HIT;run;WELL_DONE;walk;fade;") && !a.PlayerAttackedFans;
    g_log[0] = 0;
    Fan b = { 0, 100, -1, -1, 6, 8, { 2 }, 1, { 3 }, 1, false, false, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "INTRO_FIRST;run;fade;") && b.PlayerAttackedFans && b.Frames == 8;
    g_log[0] = 0;
    Fan c = { 0, 4, -1, -1, -1, 99, { 2 }, 1, { 0 }, 0, false, false, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "INTRO_FIRST;") && c.Frames == 4;
    std::printf(ok ? "00cf2b70_TEST PASS\n" : "00cf2b70_TEST FAIL\n");
    return ok ? 0 : 1;
}
