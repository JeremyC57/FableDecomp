// Behaviour test for CAIStateGroup_TavernServer::FillTankard (0x00967B40): same control flow over mocks.
#include <cstdio>
struct CServer {
    bool tankardAlive, jugAlive;
    int firstAction, secondAction;      // AIAction_PerformActionAndWait results
    int idleResult;                     // AIAction_DoNothing result
    int frame, fps, validFor;           // frames advance one per idle; serving point valid for validFor idles
    int terminated, freed, pushable, idles, actions;
    float fill; int servings; bool userSet;
    void Terminate() { ++terminated; }
    void SetPushable(bool b) { pushable = b ? 1 : 0; }
    int Perform(int r) { ++actions; return r; }
    int DoNothing() { ++idles; ++frame; return idleResult; }
    bool Valid() { return idles < validFor; }
    bool FillTankard()
    {
        if (!tankardAlive || !jugAlive) { Terminate(); return false; }
        SetPushable(false);
        userSet = true;
        if (Perform(firstAction) != 2) { ++freed; SetPushable(true); return false; }
        long endFrame = frame + fps * 4;
        do {
            if (DoNothing() != 4) { ++freed; SetPushable(true); return false; }
        } while (endFrame > frame && Valid());
        fill = 1.0f;
        servings--;
        if (Perform(secondAction) != 2) { ++freed; SetPushable(true); return false; }
        ++freed; SetPushable(true);
        return true;
    }
};
static CServer Make(bool t, bool j, int a1, int a2, int idle, int valid)
{
    CServer s;
    s.tankardAlive = t; s.jugAlive = j; s.firstAction = a1; s.secondAction = a2; s.idleResult = idle;
    s.frame = 100; s.fps = 2; s.validFor = valid; s.terminated = 0; s.freed = 0; s.pushable = 1; s.idles = 0;
    s.actions = 0; s.fill = 0.0f; s.servings = 5; s.userSet = false;
    return s;
}
int main()
{
    bool ok = true;
    CServer s = Make(true, true, 2, 2, 4, 100);
    ok = ok && s.FillTankard() && s.idles == 8 && s.fill == 1.0f && s.servings == 4 && s.freed == 1 && s.pushable == 1;
    s = Make(true, true, 2, 2, 4, 3);
    ok = ok && s.FillTankard() && s.idles == 3 && s.servings == 4;
    s = Make(true, true, 2, 1, 4, 100);
    ok = ok && !s.FillTankard() && s.servings == 4 && s.freed == 1 && s.actions == 2;
    s = Make(true, true, 1, 2, 4, 100);
    ok = ok && !s.FillTankard() && s.idles == 0 && s.servings == 5 && s.freed == 1 && s.userSet;
    s = Make(true, true, 2, 2, 3, 100);
    ok = ok && !s.FillTankard() && s.idles == 1 && s.fill == 0.0f && s.freed == 1;
    s = Make(false, true, 2, 2, 4, 100);
    ok = ok && !s.FillTankard() && s.terminated == 1 && s.freed == 0 && !s.userSet && s.pushable == 1;
    if (!ok) { std::printf("00967b40_TEST FAIL\n"); return 1; }
    std::printf("00967b40_TEST PASS\n");
    return 0;
}
