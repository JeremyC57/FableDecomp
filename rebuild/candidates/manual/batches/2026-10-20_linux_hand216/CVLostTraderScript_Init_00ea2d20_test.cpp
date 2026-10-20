// Behaviour test for 0x00EA2D20 CV_LostTraderScript::Init: two region registrations, flags cleared, distance 10.
#include <cstdio>
#include <cstring>
struct CCharString { char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct CGameScriptInterface {
    int calls; char q[2][32]; char r[2][32];
    CGameScriptInterface() : calls(0) { }
    virtual void AddQuestRegion(const CCharString& questName, const CCharString& regionName)
    { std::strcpy(q[calls], questName.text); std::strcpy(r[calls], regionName.text); ++calls; }
};
struct CV_LostTraderScript {
    CGameScriptInterface* Game; bool f[5]; long DistanceForBrotherToReunite;
    void Init()
    {
        Game->AddQuestRegion("V_LostTrader", "BanditCampPath1");
        Game->AddQuestRegion("V_LostTrader", "DemonDoor_BanditCampPath");
        f[0] = false; f[1] = false; f[2] = false; f[3] = false; f[4] = false;
        DistanceForBrotherToReunite = 10;
    }
};
int main()
{
    CGameScriptInterface g; CV_LostTraderScript s; s.Game = &g;
    for (int i = 0; i < 5; ++i) s.f[i] = true;
    s.DistanceForBrotherToReunite = 0;
    s.Init();
    if (g.calls != 2 || std::strcmp(g.q[0], "V_LostTrader") || std::strcmp(g.r[0], "BanditCampPath1")
        || std::strcmp(g.r[1], "DemonDoor_BanditCampPath") || s.f[0] || s.f[4] || s.DistanceForBrotherToReunite != 10) {
        std::printf("00ea2d20_TEST FAIL\n"); return 1;
    }
    std::printf("00ea2d20_TEST PASS\n");
    return 0;
}
