// Behaviour test for CQ_BountyHuntScript::WatchForQuestSuccess (0x00D150F0): both hostages safe -> QuestSucceeded
// (unless terminating); frames are pumped until termination.
#include <cstdio>
struct CScript;
struct CInterface { CScript* s; int frames; int terminateAt; int safe1At; int safe2At; void NewScriptFrame(); };
struct CScript {
    CInterface* GSI; bool QuestSucceeded; bool Hostage1Safe; bool Hostage2Safe; bool terminating;
    bool IsActiveThreadTerminating() { return terminating; }
    void WatchForQuestSuccess()
    {
        while (!IsActiveThreadTerminating()) {
            if (Hostage1Safe && Hostage2Safe) {
                if (IsActiveThreadTerminating())
                    return;
                QuestSucceeded = true;
            }
            GSI->NewScriptFrame();
        }
    }
};
void CInterface::NewScriptFrame()
{
    ++frames;
    if (frames == safe1At) s->Hostage1Safe = true;
    if (frames == safe2At) s->Hostage2Safe = true;
    if (frames == terminateAt) s->terminating = true;
}
static bool Run(int safe1At, int safe2At, int terminateAt, bool want)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.terminateAt = terminateAt; i.safe1At = safe1At; i.safe2At = safe2At;
    s.GSI = &i; s.QuestSucceeded = false; s.Hostage1Safe = s.Hostage2Safe = false; s.terminating = false;
    s.WatchForQuestSuccess();
    return s.QuestSucceeded == want && i.frames == terminateAt;
}
int main()
{
    if (!Run(1, 2, 4, true) || !Run(1, 9, 4, false) || !Run(9, 1, 4, false) || !Run(2, 3, 3, false)) {
        std::printf("00d150f0_TEST FAIL\n"); return 1;
    }
    std::printf("00d150f0_TEST PASS\n");
    return 0;
}