// Behaviour test for 0x00DCC040 (NScript::CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase*,
// CGameScriptInterfaceBase&)): the level-script base stores the interface and the master data, then the members are
// built in declaration order. Checks: Game and MasterScriptData set, the three timers (CommentTimer,
// RemindHeroOfObjectivesTimer, then each team's reinforcements timer) register in that order, the team counters are
// zeroed, the crate list starts empty and the strings / script things are default-built.
#include <cstdio>
class CScriptDataBase { public: int x; };
static int g_nextTimer = 100;
class CGameScriptInterfaceBase { public: int RegisterTimer() { return g_nextTimer++; } };
class CGameScriptInterface : public CGameScriptInterfaceBase { };
static CGameScriptInterface g_game;
CGameScriptInterface* g_pGameScriptInterface = &g_game;
class CTimer {
public:
    int TimerIndex;
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
};
class CCharString { public: const char* PStringData; CCharString() : PStringData("") { } };
class CWideString { public: const char* PStringData; CWideString() : PStringData("") { } };
class CScriptThing { public: void* PImp; CScriptThing() : PImp(0) { } };
namespace std {
template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
}
class CScriptBase { public: int built; CScriptBase() : built(1) { } virtual ~CScriptBase() { } };
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
    CGameScriptInterfaceBase* Game;
    TData* MasterScriptData;
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCrateTeamManager {
    public:
        CCrateTeamManager()
        {
            for (int i = 0; i < 6; ++i)
                StateCounter[i] = 0;
            MemberCount = 0;
        }
        int StateCounter[6];
        int MemberCount;
        CTimer TeamReinforcementsTimer;
        int MaxTeamMembers;
        CScriptThing CrateDropPos;
        CScriptThing TeamCrateCarrier;
        CCrateTeamManager* EnemyTeam;
    };
    CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    int CrateCount;
    CTimer CommentTimer;
    bool DoneIntroduction, HeroAtWrongEntrance, WhisperSpawned;
    std::vector<CScriptThing> CrateList;
    bool MissionSucceeded;
    int MissionFailed;
    CWideString FailReasons[4];
    int HeroTeam;
    int BanditWavesSpawned;
    CTimer RemindHeroOfObjectivesTimer;
    CCharString TextSystemScriptCode;
    bool HeroMetWhisperBeforeFarm, ShownCombatMultiplierTutorial, ShownCombatFlourishTutorial, WhisperInCutscene;
    CCrateTeamManager Teams[2];
};
}
NScript::CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
static int fail(int line) { std::printf("00dcc040_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    CScriptDataBase data;
    CGameScriptInterfaceBase game;
    NScript::CQ_OrchardFarmRaidScript* q = new NScript::CQ_OrchardFarmRaidScript(&data, game);
    CHECK(q->built == 1 && q->Game == &game && (void*)q->MasterScriptData == (void*)&data);
    CHECK(q->CommentTimer.TimerIndex == 100 && q->RemindHeroOfObjectivesTimer.TimerIndex == 101);
    CHECK(q->Teams[0].TeamReinforcementsTimer.TimerIndex == 102 && q->Teams[1].TeamReinforcementsTimer.TimerIndex == 103);
    for (int t = 0; t < 2; ++t) {
        CHECK(q->Teams[t].MemberCount == 0 && q->Teams[t].CrateDropPos.PImp == 0 && q->Teams[t].TeamCrateCarrier.PImp == 0);
        for (int i = 0; i < 6; ++i) CHECK(q->Teams[t].StateCounter[i] == 0);
    }
    CHECK(q->CrateList._M_start == 0 && q->CrateList._M_finish == 0 && q->CrateList._M_end_of_storage == 0);
    CHECK(q->FailReasons[3].PStringData[0] == 0 && q->TextSystemScriptCode.PStringData[0] == 0);
    delete q;
    std::printf("00dcc040_TEST PASS\n");
    return 0;
}