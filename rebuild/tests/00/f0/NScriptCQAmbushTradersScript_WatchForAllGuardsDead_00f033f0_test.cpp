// Behaviour test for 0x00F033F0 NScript::CQ_AmbushTradersScript::WatchForAllGuardsDead: the thread waits for DesertedFarm,
// then GibbetWoods, then for GuardAliveCount to reach 0 (one frame per check) and raises AllGuardsDead; a terminating thread
// leaves AllGuardsDead untouched.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
struct CQ_AmbushTradersScript;
class CGameScriptInterface { public:
    int Frames, FarmAt, WoodsAt, GuardsDieAt; long* Guards;
    void NewScriptFrame() { ++Frames; if (Frames >= GuardsDieAt) *Guards = 0; }
    bool IsRegionLoaded(const CCharString& n) const
    {
        if (!std::strcmp(n.Text, "DesertedFarm")) return Frames >= FarmAt;
        if (!std::strcmp(n.Text, "GibbetWoods")) return Frames >= WoodsAt;
        return false;
    }
};
struct CQ_AmbushTradersScript {
    CGameScriptInterface* Game; bool AllGuardsDead; long GuardAliveCount; int TermAt, TermCalls;
    bool IsActiveThreadTerminating() { ++TermCalls; return TermAt && TermCalls >= TermAt; }
    void WatchForAllGuardsDead()
    {
        while (!Game->IsRegionLoaded("DesertedFarm")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("GibbetWoods")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (IsActiveThreadTerminating())
            return;
        do {
            if (GuardAliveCount == 0) {
                if (IsActiveThreadTerminating())
                    return;
                AllGuardsDead = true;
                if (IsActiveThreadTerminating())
                    return;
                return;
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    CQ_AmbushTradersScript q; CGameScriptInterface g;
    g.Frames = 0; g.FarmAt = 2; g.WoodsAt = 4; g.GuardsDieAt = 7; g.Guards = &q.GuardAliveCount;
    q.Game = &g; q.AllGuardsDead = false; q.GuardAliveCount = 3; q.TermAt = 0; q.TermCalls = 0;
    q.WatchForAllGuardsDead();
    int ok = q.AllGuardsDead && g.Frames == 7;
    // terminating during the guard wait
    g.Frames = 0; q.AllGuardsDead = false; q.GuardAliveCount = 3; q.TermCalls = 0; q.TermAt = 9;
    q.WatchForAllGuardsDead();
    ok = ok && !q.AllGuardsDead && q.GuardAliveCount == 3;
    if (!ok) { std::printf("00f033f0_TEST FAIL\n"); return 1; }
    std::printf("00f033f0_TEST PASS\n");
    return 0;
}