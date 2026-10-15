// Behaviour test for CQ_BountyHuntScript::WatchForQuestFailure (0x00D15090): a villager killed before the intro or
// a killed hostage fails the quest and ends the watch; otherwise frames are pumped until termination.
#include <cstdio>
struct CScript;
struct CInterface { CScript* s; int frames; int terminateAt; int eventAt; int which; void NewScriptFrame(); };
struct CScript {
    CInterface* GSI; bool QuestFailed; bool VillagerKilled; bool VillagerIntroDone; bool Hostage1Killed; bool Hostage2Killed; bool terminating; int checks;
    bool IsActiveThreadTerminating() { ++checks; return terminating; }
    void WatchForQuestFailure()
    {
        while (!IsActiveThreadTerminating()) {
            if ((VillagerKilled && !VillagerIntroDone) || Hostage1Killed || Hostage2Killed) {
                if (IsActiveThreadTerminating())
                    return;
                QuestFailed = true;
                IsActiveThreadTerminating();
                return;
            }
            GSI->NewScriptFrame();
        }
    }
};
void CInterface::NewScriptFrame()
{
    ++frames;
    if (frames == eventAt) { if (which == 0) s->VillagerKilled = true; if (which == 1) s->Hostage1Killed = true; if (which == 2) s->Hostage2Killed = true; }
    if (frames == terminateAt) s->terminating = true;
}
static bool Run(int which, bool introDone, int eventAt, int terminateAt, bool wantFailed, int wantFrames, int wantChecks)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.terminateAt = terminateAt; i.eventAt = eventAt; i.which = which;
    s.GSI = &i; s.QuestFailed = false; s.VillagerKilled = false; s.VillagerIntroDone = introDone; s.Hostage1Killed = s.Hostage2Killed = false;
    s.terminating = false; s.checks = 0;
    s.WatchForQuestFailure();
    return s.QuestFailed == wantFailed && i.frames == wantFrames && s.checks == wantChecks;
}
int main()
{
    bool ok = Run(0, false, 2, 9, true, 2, 5) && Run(0, true, 2, 4, false, 4, 5) && Run(1, true, 3, 9, true, 3, 6)
           && Run(2, false, 1, 9, true, 1, 4) && Run(3, false, 1, 3, false, 3, 4);
    if (!ok) { std::printf("00d15090_TEST FAIL\n"); return 1; }
    std::printf("00d15090_TEST PASS\n");
    return 0;
}
