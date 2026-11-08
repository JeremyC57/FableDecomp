// Behaviour test for NScript::CQ_BreakSiegeScript::CKG_Sheriff::Main (0x00D22F30): wait for the bandit demands and the chief's refusal,
// mark the sheriff on the mini map, then until the siege starts: ask the open-gates question when talked to (yes leaves the loop) and
// heal + ally when hit; on yes run CS_BREAKSIEGE_GATESOPEN (followers) or _GATESOPENNOMEN, start the siege, then say the escape line once
// the bandit leader is free and idle until the thread ends. A frame-driven state model of the flags the retail body reads.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Sheriff {
    int Frame, TalkFrame, HitFrame, Answer, Followers, EndFrame;
    bool DemandsKnown, ChiefRefuses, LeaderFree, SiegeStart, EndDone;
    bool T() const { return Frame >= EndFrame; }
    void NSF()
    {
        ++Frame;
        if (Frame == 2) DemandsKnown = true;
        if (Frame == 3) ChiefRefuses = true;
        if (Frame == 12) LeaderFree = true;
        if (Frame == 14) EndDone = true;
    }
    void Main()
    {
        NSF();
        if (T()) return;
        while (!DemandsKnown) { NSF(); if (T()) return; }
        while (!ChiefRefuses) { NSF(); if (T()) return; }
        if (!SiegeStart) Log("marker");
        while (!SiegeStart) {
            NSF();
            if (T()) return;
            if (Frame == TalkFrame) {
                Log("question");
                if (Answer == 1) break;
            }
            if (Frame == HitFrame) Log("heal_ally");
        }
        if (!SiegeStart) {
            Log(Followers ? "CS_BREAKSIEGE_GATESOPEN" : "CS_BREAKSIEGE_GATESOPENNOMEN");
            Log("siege");
            SiegeStart = true;
        }
        while (!EndDone) {
            NSF();
            if (T()) return;
            if (LeaderFree) { Log("TEXT_QST_018_SHERIFF_ESCAPE_1"); break; }
        }
        while (!EndDone) { NSF(); if (T()) return; }
        if (T()) return;
        do NSF(); while (!T());
    }
};
int main()
{
    Sheriff a = { 0, 6, 5, 1, 2, 20, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "marker;heal_ally;question;CS_BREAKSIEGE_GATESOPEN;siege;TEXT_QST_018_SHERIFF_ESCAPE_1;") && a.Frame == 20;
    g_log[0] = 0;
    Sheriff b = { 0, 5, 0, 1, 0, 20, false, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "marker;question;CS_BREAKSIEGE_GATESOPENNOMEN;siege;TEXT_QST_018_SHERIFF_ESCAPE_1;");
    g_log[0] = 0;
    Sheriff c = { 0, 5, 0, 0, 0, 8, false, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "marker;question;") && c.Frame == 8 && !c.SiegeStart;
    std::printf(ok ? "00d22f30_TEST PASS\n" : "00d22f30_TEST FAIL\n");
    return ok ? 0 : 1;
}