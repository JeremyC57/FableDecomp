// Behaviour test for 0x00DC3BD0 NScript::CQ_OpeningGraveyardSecretPassageScript::WatchForTermination: wait for MissionFailed or MissionSucceeded, then
// (unless the thread is terminating) complete or fail the active quest and deactivate it.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
class CWideString { public: const unsigned short* Text; CWideString(const unsigned short* t) : Text(t) {} };

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct CQ_OpeningGraveyardSecretPassageScript;
class CGameScriptInterface { public:
    CQ_OpeningGraveyardSecretPassageScript* Script; int Frames; int SetAfter; int Which;   // raise flag Which (1 succeeded, 2 failed) after SetAfter frames
    void NewScriptFrame();
    CCharString GetActiveQuestName() const { return CCharString("Q_Active"); }
    void SetQuestAsCompleted(const CCharString& q, bool a, bool b, bool c) const
    { char t[96]; std::sprintf(t, "complete(%s,%d,%d,%d)", q.Text, a, b, c); Log(t); }
    void SetQuestAsFailed(const CCharString& q, bool a, const CWideString& r, bool b) const
    { char t[96]; std::sprintf(t, "fail(%s,%d,%d,%d)", q.Text, a, (int)r.Text[0], b); Log(t); }
    void DeactivateQuestLater(const CCharString& q, unsigned long d) const
    { char t[96]; std::sprintf(t, "deactivate(%s,%lu)", q.Text, d); Log(t); }
    void DisplayQuestInfo(bool b) const { char t[32]; std::sprintf(t, "display(%d)", b); Log(t); }
    void SetExperienceSpendingAsEnabled(bool b) const { char t[32]; std::sprintf(t, "xp(%d)", b); Log(t); }
    void SetPrizeTavernTable(bool b) { char t[32]; std::sprintf(t, "prize(%d)", b); Log(t); }
    void Pause(float s) const { char t[32]; std::sprintf(t, "pause(%d)", (int)(s * 10.0f + 0.5f)); Log(t); }
    void FadeScreenIn() { Log("fadein"); }
};

struct CQ_OpeningGraveyardSecretPassageScript {
    CGameScriptInterface* Game;
    int TermAfter;   // IsActiveThreadTerminating returns true from this call on (1-based); 0 = never
    int TermCalls;
    bool MissionSucceeded, MissionFailed;
    bool IsActiveThreadTerminating() { ++TermCalls; return TermAfter && TermCalls >= TermAfter; }
    void WatchForTermination()
    {
        static const unsigned short reason[] = { 0, 0 };
        while (!MissionFailed && !MissionSucceeded) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (MissionSucceeded) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, reason, true);
        }
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};

void CGameScriptInterface::NewScriptFrame()
{
    ++Frames;
    Log("frame");
    if (Frames == SetAfter) { if (Which == 1) Script->MissionSucceeded = true; else Script->MissionFailed = true; }
}

static int Run(int setAfter, int which, int termAfter, bool s0, bool f0, const char* expect)
{
    CGameScriptInterface gsi; CQ_OpeningGraveyardSecretPassageScript q;
    g_log[0] = 0;
    gsi.Script = &q; gsi.Frames = 0; gsi.SetAfter = setAfter; gsi.Which = which;
    q.Game = &gsi; q.TermAfter = termAfter; q.TermCalls = 0; q.MissionSucceeded = s0; q.MissionFailed = f0;
    q.WatchForTermination();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got   %s\nwant  %s\n", g_log, expect); return 1; }
    return 0;
}

int main()
{
    int bad = 0;
    bad |= Run(2, 1, 0, false, false, "frame;frame;complete(Q_Active,1,0,0);deactivate(Q_Active,0);");
    bad |= Run(3, 2, 0, false, false, "frame;frame;frame;fail(Q_Active,1,0,1);deactivate(Q_Active,0);");
    bad |= Run(0, 0, 0, true, false, "complete(Q_Active,1,0,0);deactivate(Q_Active,0);");
    bad |= Run(0, 0, 0, false, true, "fail(Q_Active,1,0,1);deactivate(Q_Active,0);");
    bad |= Run(0, 0, 0, true, true, "complete(Q_Active,1,0,0);deactivate(Q_Active,0);");
    bad |= Run(5, 1, 2, false, false, "frame;frame;");                 // terminated while waiting
    bad |= Run(1, 1, 2, false, false, "frame;");                       // terminated at the post-wait test
    bad |= Run(1, 2, 3, false, false, "frame;");                  // terminated inside the outcome arm
    if (bad) { std::printf("00dc3bd0_TEST FAIL\n"); return 1; }
    std::printf("00dc3bd0_TEST PASS\n");
    return 0;
}