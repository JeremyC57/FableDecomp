// Behaviour test for 0x00DF1230 CQ_SummoningTheShipScript::Init: two region registrations then the state reset.
#include <cstdio>
#include <cstring>
struct CCharString { char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct CGameScriptInterface {
    int calls; char q[2][32]; char r[2][32];
    CGameScriptInterface() : calls(0) { }
    virtual void AddQuestRegion(const CCharString& questName, const CCharString& regionName)
    { std::strcpy(q[calls], questName.text); std::strcpy(r[calls], regionName.text); ++calls; }
};
struct CQ_SummoningTheShipScript {
    CGameScriptInterface* Game; long SummonersAlive, CurrentAttackWave; bool LighthouseStarted, SummonerAttacksStarted, MissionFailed;
    void Init()
    {
        Game->AddQuestRegion("Q_SummoningTheShip", "HookCoast");
        Game->AddQuestRegion("Q_SummoningTheShip", "LostBay");
        SummonersAlive = 0; CurrentAttackWave = 1;
        LighthouseStarted = false; SummonerAttacksStarted = false; MissionFailed = false;
    }
};
int main()
{
    CGameScriptInterface g; CQ_SummoningTheShipScript s; s.Game = &g;
    s.SummonersAlive = 5; s.CurrentAttackWave = 9; s.LighthouseStarted = s.SummonerAttacksStarted = s.MissionFailed = true;
    s.Init();
    if (g.calls != 2 || std::strcmp(g.q[0], "Q_SummoningTheShip") || std::strcmp(g.r[0], "HookCoast")
        || std::strcmp(g.q[1], "Q_SummoningTheShip") || std::strcmp(g.r[1], "LostBay") || s.SummonersAlive != 0
        || s.CurrentAttackWave != 1 || s.LighthouseStarted || s.SummonerAttacksStarted || s.MissionFailed) {
        std::printf("00df1230_TEST FAIL\n"); return 1;
    }
    std::printf("00df1230_TEST PASS\n");
    return 0;
}
