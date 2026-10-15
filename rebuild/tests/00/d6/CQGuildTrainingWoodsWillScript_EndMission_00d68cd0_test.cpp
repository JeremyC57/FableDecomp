// Behaviour test for CQ_GuildTrainingWoodsWillScript::EndMission (0x00D68CD0): wait for MissionOver one frame at a
// time; mark MissionSucceeded only when the thread is not terminating.
#include <cstdio>
struct CScript;
struct CInterface { CScript* s; int frames; int overAt; int terminateAt; void NewScriptFrame(); };
struct CScript {
    CInterface* GSI; bool MissionSucceeded; bool MissionOver; bool terminating;
    bool IsActiveThreadTerminating() { return terminating; }
    void EndMission()
    {
        while (!MissionOver) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (!IsActiveThreadTerminating())
            MissionSucceeded = true;
    }
};
void CInterface::NewScriptFrame() { ++frames; if (frames == overAt) s->MissionOver = true; if (frames == terminateAt) s->terminating = true; }
static bool Run(int overAt, int terminateAt, bool over0, bool term0, int wantFrames, bool wantSucceeded)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.overAt = overAt; i.terminateAt = terminateAt;
    s.GSI = &i; s.MissionSucceeded = false; s.MissionOver = over0; s.terminating = term0;
    s.EndMission();
    return i.frames == wantFrames && s.MissionSucceeded == wantSucceeded;
}
int main()
{
    if (!Run(3, 0, false, false, 3, true) || !Run(9, 2, false, false, 2, false) || !Run(0, 0, true, false, 0, true)
        || !Run(0, 0, true, true, 0, false) || !Run(2, 2, false, false, 2, false)) {
        std::printf("00d68cd0_TEST FAIL\n"); return 1;
    }
    std::printf("00d68cd0_TEST PASS\n");
    return 0;
}