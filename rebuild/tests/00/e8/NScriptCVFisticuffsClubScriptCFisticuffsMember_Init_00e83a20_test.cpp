// Behaviour test for 0x00E83A20 NScript::CV_FisticuffsClubScript::CFisticuffsMember::Init(): status, cheer flag and the
// shout timer reset, not pushable by the hero, every state group off then the three combat groups on, opinion and deed
// reactions on, the neutral faction, and the cheer / boo text pair picked by rand() % 3 -- checked for each pick through a
// logging game interface and a deterministic rand.
#include <cstdio>
#include <cstring>
static char gLog[4096];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }
static int gRandValue;
static int TestRand() { return gRandValue; }
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text = "") : PStringData(text) { }
    const CCharString& operator=(const char* text) { PStringData = text; return *this; }
};
static int gThingsAlive = 0;
class CScriptThing { public:
    int Id;
    struct InlineCopy { };
    CScriptThing(int id) : Id(id) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other) : Id(other.Id) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other, InlineCopy) : Id(other.Id) { ++gThingsAlive; }
    ~CScriptThing() { --gThingsAlive; }
};
class CGameScriptInterfaceBase { public:
    void SetTimer(long index, long time) { char t[64]; std::sprintf(t, "SetTimer %ld=%ld", index, time); Log(t); }
    void SetIsPushableByHero(CScriptThing thing, bool pushable) { char t[64]; std::sprintf(t, "Pushable %d %d", thing.Id, (int)pushable); Log(t); }
    void EntitySetAllStategroupsEnabled(const CScriptThing& thing, bool enabled) const { char t[64]; std::sprintf(t, "AllGroups %d %d", thing.Id, (int)enabled); Log(t); }
    void EntitySetOpinionReactionsEnabled(const CScriptThing& thing, bool enabled) const { char t[64]; std::sprintf(t, "Opinion %d %d", thing.Id, (int)enabled); Log(t); }
    void EntitySetDeedReactionsEnabled(const CScriptThing& thing, bool enabled) const { char t[64]; std::sprintf(t, "Deed %d %d", thing.Id, (int)enabled); Log(t); }
    void EntitySetStategroupEnabled(const CScriptThing& thing, const CCharString& group, bool enabled) const { char t[96]; std::sprintf(t, "Group %d %s %d", thing.Id, group.PStringData, (int)enabled); Log(t); }
    void EntitySetInFaction(const CScriptThing& thing, const CCharString& faction) const { char t[96]; std::sprintf(t, "Faction %d %s", thing.Id, faction.PStringData); Log(t); }
};
static CGameScriptInterfaceBase* g_pGameScriptInterface;
class CTimer { public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CEntityScriptBase { public:
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
    CEntityScriptBase() : Game(0), Me(4) { }
    virtual ~CEntityScriptBase() { }
};
namespace NScript {
class CV_FisticuffsClubScript { public:
    class CFisticuffsMember;
};
class CV_FisticuffsClubScript::CFisticuffsMember : public CEntityScriptBase { public:
    CV_FisticuffsClubScript* ParentClass;
    void*                    PMasterData;
    bool                     PerformingCheerOrBoo;
    long                     FC_Status;
    CTimer                   ShoutTimer;
    CCharString              CheerName;
    CCharString              BooName;
    void Init();
};
}

void NScript::CV_FisticuffsClubScript::CFisticuffsMember::Init()
{
    FC_Status = 0;
    PerformingCheerOrBoo = false;
    ShoutTimer = 0;
    Game->SetIsPushableByHero(CScriptThing(Me, CScriptThing::InlineCopy()), false);
    Game->EntitySetAllStategroupsEnabled(Me, false);
    Game->EntitySetOpinionReactionsEnabled(Me, true);
    Game->EntitySetDeedReactionsEnabled(Me, true);
    Game->EntitySetStategroupEnabled(Me, "SG_SEEK_ENEMY", true);
    Game->EntitySetStategroupEnabled(Me, "SG_SEEK_SCRIPT_TARGET", true);
    Game->EntitySetStategroupEnabled(Me, "SG_MELEE_COMBAT_NEARBY2", true);
    Game->EntitySetInFaction(Me, "FACTION_NEUTRAL");
    switch (TestRand() % 3) {
    case 0:
        CheerName = "TEXT_QST_008_PUNCH_CLUB_1_CHEERER";
        BooName = "TEXT_QST_008_PUNCH_CLUB_1_BOOER";
        break;
    case 1:
        CheerName = "TEXT_QST_008_PUNCH_CLUB_2_CHEERER";
        BooName = "TEXT_QST_008_PUNCH_CLUB_2_BOOER";
        break;
    case 2:
        CheerName = "TEXT_QST_008_PUNCH_CLUB_3_CHEERER";
        BooName = "TEXT_QST_008_PUNCH_CLUB_3_BOOER";
        break;
    default:
        CheerName = "TEXT_QST_008_PUNCH_CLUB_1_CHEERER";
        BooName = "TEXT_QST_008_PUNCH_CLUB_1_BOOER";
        break;
    }
}

static int Run(int randValue, const char* club)
{
    gLog[0] = 0; gRandValue = randValue;
    CGameScriptInterfaceBase game;
    g_pGameScriptInterface = &game;
    NScript::CV_FisticuffsClubScript::CFisticuffsMember m;
    m.Game = &game; m.ParentClass = 0; m.PMasterData = 0;
    m.PerformingCheerOrBoo = true; m.FC_Status = 3; m.ShoutTimer.TimerIndex = 8;
    m.Init();
    const char* want = "SetTimer 8=0;Pushable 4 0;AllGroups 4 0;Opinion 4 1;Deed 4 1;Group 4 SG_SEEK_ENEMY 1;"
                       "Group 4 SG_SEEK_SCRIPT_TARGET 1;Group 4 SG_MELEE_COMBAT_NEARBY2 1;Faction 4 FACTION_NEUTRAL;";
    char cheer[64], boo[64];
    std::sprintf(cheer, "TEXT_QST_008_PUNCH_CLUB_%s_CHEERER", club);
    std::sprintf(boo, "TEXT_QST_008_PUNCH_CLUB_%s_BOOER", club);
    int ok = std::strcmp(gLog, want) == 0 && m.FC_Status == 0 && !m.PerformingCheerOrBoo
             && std::strcmp(m.CheerName.PStringData, cheer) == 0 && std::strcmp(m.BooName.PStringData, boo) == 0;
    if (!ok) std::printf("%d: %s %s %s\n", randValue, gLog, m.CheerName.PStringData, m.BooName.PStringData);
    return ok;
}

int main()
{
    int ok = Run(0, "1") && Run(4, "2") && Run(11, "3") && Run(32766, "1") && Run(32765, "3");
    ok = ok && gThingsAlive == 0;
    if (!ok) { std::printf("00e83a20_TEST FAIL\n"); return 1; }
    std::printf("00e83a20_TEST PASS\n");
    return 0;
}