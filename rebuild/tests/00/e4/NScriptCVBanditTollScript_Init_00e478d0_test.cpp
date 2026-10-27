#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
class CScriptDef { public: int TollBanditFee; };
static CScriptDef g_def = { 250 };
static CScriptDef* pGlobalScriptDef = &g_def;
struct Script {
    bool BanditsAngered;
    bool TollPaid;
    bool AssassinateReluctantTrader;
    bool TollBanditIsAfterHero;
    bool TraderDead;
    bool SafeTraderAppeared;
    bool LeaderDead;
    bool BanditsFlee;
    bool BanditsReachedMarker;
    bool IsFactionSet;
    int BanditTollFee;
    bool GivenQuestCard;
    int Tail;

    void Init()
    {
        BanditsAngered = false; TollPaid = false; AssassinateReluctantTrader = false; TollBanditIsAfterHero = false; TraderDead = false; SafeTraderAppeared = false; LeaderDead = false; BanditsFlee = false; BanditsReachedMarker = false; IsFactionSet = false;
        BanditTollFee = pGlobalScriptDef->TollBanditFee;
        GivenQuestCard = false;
    }
};

int main()
{
    g_log[0] = 0;
    bool ok = true;
    Script s;
    std::memset(&s, 0x5a, sizeof(s));
    s.Tail = 77;
    s.Init();
    ok = (!s.BanditsAngered && !s.TollPaid && !s.AssassinateReluctantTrader && !s.TollBanditIsAfterHero && !s.TraderDead && !s.SafeTraderAppeared && !s.LeaderDead && !s.BanditsFlee && !s.BanditsReachedMarker && !s.IsFactionSet && s.BanditTollFee == 250 && !s.GivenQuestCard) && s.Tail == 77;

    if (!ok) { std::printf("00e478d0_TEST FAIL\n"); return 1; }
    std::printf("00e478d0_TEST PASS\n");
    return 0;
}