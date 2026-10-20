// Behaviour test for CV_RockTrollFirstEncounterScript::WatchForRockTrollKilled (0x00EC5010): predicate set on the
// troll, one yield, wait for the kill, then health bar removed, quest info hidden and MissionOver set.
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
struct Impl { int KilledAfter, calls; bool MsgIsKilledBy(const CCharString& k) { return std::strcmp(k.Text, "") == 0 && ++calls > KilledAfter; } };
struct CScriptThing {
    Impl* Data;
    bool MsgIsKilledBy(const CCharString& k) const { if (!Data) return false; return Data->MsgIsKilledBy(k); }
};
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) {} };
static int g_frames, g_termAt, g_termCalls; static Impl* g_pred; static long g_removed; static int g_display;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    void DisplayQuestInfo(bool show) const { g_display = show ? 1 : 0; }
    void RemoveQuestInfoElement(long id) const { g_removed = id; }
};
struct CV_RockTrollFirstEncounterScript {
    CGameScriptInterface* Game;
    bool MissionOver;
    long RockTrollHealthBarID;
    void SetPredicate(const CIsThingAlive& p) { g_pred = p.Thing.Data; }
    bool IsActiveThreadTerminating() { return ++g_termCalls >= g_termAt; }
    void WatchForRockTrollKilled(CScriptThing troll)
    {
        SetPredicate(CIsThingAlive(troll));
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!troll.MsgIsKilledBy("")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveQuestInfoElement(RockTrollHealthBarID);
        Game->DisplayQuestInfo(false);
        MissionOver = true;
    }
};
static bool run(Impl* impl, int termAt, bool wantOver, int wantFrames)
{
    g_frames = 0; g_termAt = termAt; g_termCalls = 0; g_pred = 0; g_removed = -1; g_display = -1;
    CGameScriptInterface g; CV_RockTrollFirstEncounterScript s; s.Game = &g; s.MissionOver = false; s.RockTrollHealthBarID = 77;
    CScriptThing t; t.Data = impl;
    s.WatchForRockTrollKilled(t);
    bool ok = g_pred == impl && s.MissionOver == wantOver && g_frames == wantFrames;
    if (wantOver) ok = ok && g_removed == 77 && g_display == 0;
    else ok = ok && g_removed == -1 && g_display == -1;
    return ok;
}
int main()
{
    Impl a = { 2, 0 }; Impl b = { 0, 0 };
    bool ok = run(&a, 100, true, 3) && run(&b, 1, false, 1) && run(0, 3, false, 3);
    if (!ok) { std::printf("00ec5010_TEST FAIL\n"); return 1; }
    std::printf("00ec5010_TEST PASS\n");
    return 0;
}
