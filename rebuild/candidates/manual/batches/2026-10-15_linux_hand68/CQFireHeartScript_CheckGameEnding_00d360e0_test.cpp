// Behaviour test for CQ_FireHeartScript::CheckGameEnding (0x00D360E0): wait for the force-quit, then RespawnHero(true)
// unless the thread is terminating.
#include <cstdio>
struct CScript;
struct CInterface {
    CScript* s; int frames; int quitAt; int terminateAt; bool quit; int respawns; bool respawnArg;
    void NewScriptFrame();
    bool HasHeroForceQuitFireheartMinigame() const { return quit; }
    void RespawnHero(bool restore) { ++respawns; respawnArg = restore; }
};
struct CScript {
    CInterface* GSI; bool terminating;
    bool IsActiveThreadTerminating() { return terminating; }
    void CheckGameEnding()
    {
        while (!GSI->HasHeroForceQuitFireheartMinigame()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (!IsActiveThreadTerminating())
            GSI->RespawnHero(true);
    }
};
void CInterface::NewScriptFrame() { ++frames; if (frames == quitAt) quit = true; if (frames == terminateAt) s->terminating = true; }
static bool Run(int quitAt, int terminateAt, bool quit0, int wantFrames, int wantRespawns)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.quitAt = quitAt; i.terminateAt = terminateAt; i.quit = quit0; i.respawns = 0; i.respawnArg = false;
    s.GSI = &i; s.terminating = false;
    s.CheckGameEnding();
    return i.frames == wantFrames && i.respawns == wantRespawns && (wantRespawns == 0 || i.respawnArg);
}
int main()
{
    if (!Run(3, 0, false, 3, 1) || !Run(9, 2, false, 2, 0) || !Run(0, 0, true, 0, 1) || !Run(2, 2, false, 2, 0)) {
        std::printf("00d360e0_TEST FAIL\n"); return 1;
    }
    std::printf("00d360e0_TEST PASS\n");
    return 0;
}
