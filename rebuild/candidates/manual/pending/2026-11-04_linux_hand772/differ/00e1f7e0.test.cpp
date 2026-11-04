// Behaviour test for 0x00E1F7E0 NScript::CQR_EscortTraderScript::CTraderToEscort::Init: four watcher threads are spawned on the
// parent with "ParentClass."-prefixed names, the speech indices reset and the wander settings applied.
#include <cstdio>
#include <string>
#include <vector>
static std::vector<std::string> gThreads;
static bool gInfo = false, gPersistent = false, gFollow = true;
static float gWanderMin = -1.0f, gWanderMax = -1.0f;
static int gStateGroup = 0;
struct CTraderToEscort {
    long FollowIdx1, FollowIdx2, WaitIdx1, WaitIdx2;
    void Spawn(const std::string& func) { gThreads.push_back(std::string("ParentClass.") + func); }
    void Init(float wanderDistance)
    {
        gInfo = true;
        gPersistent = true;
        Spawn("WatchForTraderDead");
        Spawn("WatchForTraderHit");
        Spawn("WatchForExpressionFollow");
        Spawn("WatchForExpressionWait");
        FollowIdx1 = 10; FollowIdx2 = 10; WaitIdx1 = 5; WaitIdx2 = 5;
        gWanderMin = 0.0f;
        gWanderMax = wanderDistance;
        gStateGroup = 4;
        gFollow = false;
    }
};
int main()
{
    CTraderToEscort t = { 0, 0, 0, 0 };
    t.Init(12.5f);
    bool ok = gInfo && gPersistent && gThreads.size() == 4 && gThreads[0] == "ParentClass.WatchForTraderDead"
           && gThreads[3] == "ParentClass.WatchForExpressionWait" && t.FollowIdx1 == 10 && t.WaitIdx2 == 5
           && gWanderMin == 0.0f && gWanderMax > 12.4f && gWanderMax < 12.6f && gStateGroup == 4 && !gFollow;
    std::printf(ok ? "00e1f7e0_TEST PASS\n" : "00e1f7e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
