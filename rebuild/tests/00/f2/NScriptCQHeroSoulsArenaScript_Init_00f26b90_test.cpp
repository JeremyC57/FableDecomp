// Behaviour test for 0x00F26B90 NScript::CQ_HeroSoulsArenaScript::Init: Rounds copied from the script def, the count, the
// seven flags and the level reached cleared, the guard-chat timer set to 0 through the global interface, and the
// "Arena" region of "Q_HeroSoulsArena" added.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
struct Rounds { int Count; int Ids[4]; };
class CScriptDef { public: Rounds HeroSoulsRounds; };
static CScriptDef g_def;
static CScriptDef* pGlobalScriptDef = &g_def;
class CGameScriptInterface { public:
    long TimerSet; long TimerValue; int TimerCalls;
    char Quest[32]; char Region[32]; int RegionCalls;
    void SetTimer(long timer, long time) { TimerSet = timer; TimerValue = time; ++TimerCalls; }
    void AddQuestRegion(const CCharString& q, const CCharString& r)
    { std::strcpy(Quest, q.Text); std::strcpy(Region, r.Text); ++RegionCalls; }
};
static CGameScriptInterface g_iface;
static CGameScriptInterface* g_pGameScriptInterface = &g_iface;
class CTimer { public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
struct CQ_HeroSoulsArenaScript {
    CGameScriptInterface* Game;
    long CreatureCount;
    bool PlayedArenaCS, PlayerHitSummoner1, PlayerHitSummoner2, PlayerAttacksGuard, JackTauntNearHOH, FirstWaveAttack, PlayedHoHCS;
    long ArenaLevelReached;
    CTimer GuardChatTimer;
    Rounds R;
    void Init()
    {
        R = pGlobalScriptDef->HeroSoulsRounds;
        CreatureCount = 0;
        PlayedArenaCS = false;
        PlayerHitSummoner1 = false;
        PlayerHitSummoner2 = false;
        PlayerAttacksGuard = false;
        JackTauntNearHOH = false;
        FirstWaveAttack = false;
        PlayedHoHCS = false;
        ArenaLevelReached = 0;
        GuardChatTimer = 0;
        Game->AddQuestRegion("Q_HeroSoulsArena", "Arena");
    }
};

int main()
{
    g_def.HeroSoulsRounds.Count = 3; g_def.HeroSoulsRounds.Ids[0] = 11; g_def.HeroSoulsRounds.Ids[2] = 33;
    std::memset(&g_iface, 0, sizeof(g_iface));
    CQ_HeroSoulsArenaScript s;
    s.Game = &g_iface; s.CreatureCount = 9; s.ArenaLevelReached = 4; s.GuardChatTimer.TimerIndex = 6;
    s.PlayedArenaCS = s.PlayerHitSummoner1 = s.PlayerHitSummoner2 = s.PlayerAttacksGuard = true;
    s.JackTauntNearHOH = s.FirstWaveAttack = s.PlayedHoHCS = true;
    s.R.Count = 0;
    s.Init();
    int ok = s.R.Count == 3 && s.R.Ids[0] == 11 && s.R.Ids[2] == 33 && s.CreatureCount == 0 && s.ArenaLevelReached == 0
        && !s.PlayedArenaCS && !s.PlayerHitSummoner1 && !s.PlayerHitSummoner2 && !s.PlayerAttacksGuard
        && !s.JackTauntNearHOH && !s.FirstWaveAttack && !s.PlayedHoHCS
        && g_iface.TimerCalls == 1 && g_iface.TimerSet == 6 && g_iface.TimerValue == 0
        && g_iface.RegionCalls == 1 && !std::strcmp(g_iface.Quest, "Q_HeroSoulsArena") && !std::strcmp(g_iface.Region, "Arena");
    if (!ok) { std::printf("00f26b90_TEST FAIL\n"); return 1; }
    std::printf("00f26b90_TEST PASS\n");
    return 0;
}