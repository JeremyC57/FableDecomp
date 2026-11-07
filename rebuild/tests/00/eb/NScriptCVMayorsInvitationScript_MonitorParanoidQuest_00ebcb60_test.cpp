// Behaviour test for NScript::CV_MayorsInvitationScript::MonitorParanoidQuest (0x00EBCB60).
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CGameScriptInterface { public:
    int frame, completeAt;
    void ActivateQuest(const char* q) { Log("activate"); Log(q); }
    bool MsgOnQuestCompleted(const char* q) { Log("check"); return std::strcmp(q, "V_ParanoidWhispers") == 0 && frame >= completeAt; }
    void NewScriptFrame() { ++frame; Log("frame"); }
};
class CV_MayorsInvitationScript { public:
    CGameScriptInterface* Game;
    int stopAt, checks;
    bool IsActiveThreadTerminating() { ++checks; return Game->frame >= stopAt; }
    void MonitorParanoidQuest()
    {
        Game->ActivateQuest("V_ParanoidWhispers");
        while (!Game->MsgOnQuestCompleted("V_ParanoidWhispers")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
};
static int Run(int completeAt, int stopAt, const char* want, int wantChecks)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.frame = 0; g.completeAt = completeAt;
    CV_MayorsInvitationScript s; s.Game = &g; s.stopAt = stopAt; s.checks = 0;
    s.MonitorParanoidQuest();
    return std::strcmp(g_log, want) == 0 && s.checks == wantChecks;
}
int main()
{
    if (!Run(0, 100, "activate;V_ParanoidWhispers;check;", 1)
        || !Run(2, 100, "activate;V_ParanoidWhispers;check;frame;check;frame;check;", 3)
        || !Run(5, 1, "activate;V_ParanoidWhispers;check;frame;", 1)) {
        std::printf("00ebcb60_TEST FAIL\n"); return 1;
    }
    std::printf("00ebcb60_TEST PASS\n");
    return 0;
}