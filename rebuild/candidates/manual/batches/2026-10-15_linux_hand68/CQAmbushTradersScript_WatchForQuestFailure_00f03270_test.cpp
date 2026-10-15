// Behaviour test for CQ_AmbushTradersScript::WatchForQuestFailure (0x00F03270): any of the four failure flags fails
// the quest (unless terminating); frames are pumped until the thread terminates.
#include <cstdio>
struct CScript;
struct CInterface { CScript* s; int frames; int terminateAt; int deadAt; int which; void NewScriptFrame(); };
struct CScript {
    CInterface* GSI; bool QuestFailed; bool ContactDead; bool HeroDead; bool SpyDead; bool SpyEscaped; bool terminating; int checks;
    bool IsActiveThreadTerminating() { ++checks; return terminating; }
    void WatchForQuestFailure()
    {
        while (!IsActiveThreadTerminating()) {
            if (HeroDead || ContactDead || SpyDead || SpyEscaped) {
                if (IsActiveThreadTerminating())
                    return;
                QuestFailed = true;
            }
            GSI->NewScriptFrame();
        }
    }
};
void CInterface::NewScriptFrame()
{
    ++frames;
    if (frames == deadAt) { if (which == 0) s->HeroDead = true; if (which == 1) s->ContactDead = true; if (which == 2) s->SpyDead = true; if (which == 3) s->SpyEscaped = true; }
    if (frames == terminateAt) s->terminating = true;
}
static bool Run(int which, int deadAt, int terminateAt, bool wantFailed, int wantFrames)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.terminateAt = terminateAt; i.deadAt = deadAt; i.which = which;
    s.GSI = &i; s.QuestFailed = false; s.ContactDead = s.HeroDead = s.SpyDead = s.SpyEscaped = false; s.terminating = false; s.checks = 0;
    s.WatchForQuestFailure();
    return s.QuestFailed == wantFailed && i.frames == wantFrames;
}
int main()
{
    bool ok = Run(0, 2, 5, true, 5) && Run(1, 2, 5, true, 5) && Run(2, 2, 5, true, 5) && Run(3, 2, 5, true, 5)
           && Run(4, 2, 5, false, 5) && Run(0, 6, 5, false, 5);
    if (!ok) { std::printf("00f03270_TEST FAIL\n"); return 1; }
    std::printf("00f03270_TEST PASS\n");
    return 0;
}
