// Behaviour test for CQ_HangingTreeGoodScript::WatchForMissionEnd (0x00D74CE0): no guards left -> failure reason
// set and MissionFailed (unless terminating); frames pumped until termination.
#include <cstdio>
#include <cstring>
struct CCharString { const char* m_text; int assigns; CCharString& operator=(const char* t) { m_text = t; ++assigns; return *this; } };
struct CScript;
struct CInterface { CScript* s; int frames; int terminateAt; int killAt; void NewScriptFrame(); };
struct CScript {
    CInterface* GSI; bool MissionFailed; long CurrentGuards; CCharString ReasonForFailing; bool terminating;
    bool IsActiveThreadTerminating() { return terminating; }
    void WatchForMissionEnd()
    {
        while (!IsActiveThreadTerminating()) {
            if (CurrentGuards == 0) {
                if (IsActiveThreadTerminating())
                    return;
                ReasonForFailing = "TEXT_QST_030_FAILURE_GUARDS_KILLED";
                MissionFailed = true;
            }
            GSI->NewScriptFrame();
        }
    }
};
void CInterface::NewScriptFrame() { ++frames; if (frames == killAt) s->CurrentGuards = 0; if (frames == terminateAt) s->terminating = true; }
static bool Run(int killAt, int terminateAt, bool wantFailed, int wantAssigns)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.terminateAt = terminateAt; i.killAt = killAt;
    s.GSI = &i; s.MissionFailed = false; s.CurrentGuards = 3; s.ReasonForFailing.m_text = ""; s.ReasonForFailing.assigns = 0; s.terminating = false;
    s.WatchForMissionEnd();
    if (wantFailed && std::strcmp(s.ReasonForFailing.m_text, "TEXT_QST_030_FAILURE_GUARDS_KILLED") != 0) return false;
    return s.MissionFailed == wantFailed && s.ReasonForFailing.assigns == wantAssigns && i.frames == terminateAt;
}
int main()
{
    if (!Run(2, 5, true, 3) || !Run(7, 5, false, 0) || !Run(5, 5, false, 0)) { std::printf("00d74ce0_TEST FAIL\n"); return 1; }
    std::printf("00d74ce0_TEST PASS\n");
    return 0;
}