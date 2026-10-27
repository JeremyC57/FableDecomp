// Behaviour test for 0x00D374A0 (NScript::CQ_FireHeartScript::CFH_Villager::Init()): the row's model and body over a recording mock of the script interface.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void logs(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static bool g_terminate = false;
static int g_frames = 0;
static void (*g_onFrame)() = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString() : PStringData(0) { }
    CCharString(const char* text, int length = -1) : PStringData(text) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { }
    const CCharString& operator=(const char* text) { PStringData = text; return *this; }
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing {
public:
    int Id;
    const char* Killer;
    const char* User;
    const char* Data;
    C3DVector Pos;
    CScriptThing() : Id(0), Killer(0), User(0), Data("") { Pos.X = Pos.Y = Pos.Z = 0.0f; }
    bool MsgIsKilledBy(const CCharString& killer) const
    {
        if (!Killer) return false;
        if (killer.PStringData[0] == 0) return true;
        return strcmp(Killer, killer.PStringData) == 0;
    }
    bool MsgIsUsedBy(const CCharString& user) const
    {
        if (!User) return false;
        bool r = strcmp(User, user.PStringData) == 0;
        return r;
    }
    CCharString GetDataString() const { return CCharString(Data); }
    const C3DVector& GetPos() const { return Pos; }
};
static CScriptThing g_hero;
static bool g_questDone = false;
static int g_digFrames = 0;
static int g_nextTimer = 100;
static int g_things = 0;
enum ETargetingType { ETT_RANGE_ = 0x7fffffff };
namespace std {
template <class T> class vector {
public:
    T items[16];
    unsigned int n;
    vector() : n(0) { }
    void clear() { n = 0; }
    unsigned int size() const { return n; }
    T& operator[](unsigned int i) { return items[i]; }
    void push_back(const T& v) { items[n++] = v; }
};
}
static char g_buf[256];
#define LOGF(fmt, args) do { sprintf args; logs(g_buf); } while (0)
class CGameScriptInterfaceBase {
public:
    void NewScriptFrame() const { ++g_frames; logs("Frame"); if (g_onFrame) g_onFrame(); }
    CScriptThing& GetHero() { return g_hero; }
    int GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const
    { LOGF("%s", (g_buf, "GetAll(%s)", name.PStringData)); for (int i = 0; i < g_things; ++i) { CScriptThing t; t.Id = i + 1; things.push_back(t); } return g_things; }
    int RegisterTimer() { return g_nextTimer++; }
    void SetTimer(int timer, int time) { LOGF("%s", (g_buf, "SetTimer(%d,%d)", timer, time)); }
    void RemoveThing(const CScriptThing& thing, bool a, bool b) const { LOGF("%s", (g_buf, "RemoveThing(%d,%d,%d)", thing.Id, a, b)); }
    void DisplayGameInfo(const CCharString& text) const { LOGF("%s", (g_buf, "DisplayGameInfo(%s)", text.PStringData)); }
    void GiveHeroObject(const CCharString& object, int count, bool b) const { LOGF("%s", (g_buf, "GiveHeroObject(%s,%d,%d)", object.PStringData, count, b)); }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "AllowedToFollowHero(%d,%d)", thing.Id, v)); }
    void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "RespondFollowWait(%d,%d)", thing.Id, v)); }
    void AddQuestRegion(const CCharString& quest, const CCharString& region) const { LOGF("%s", (g_buf, "AddQuestRegion(%s,%s)", quest.PStringData, region.PStringData)); }
    void DeactivateQuest(const CCharString& quest, unsigned int flags) const { LOGF("%s", (g_buf, "DeactivateQuest(%s,%u)", quest.PStringData, flags)); }
    bool IsQuestCompleted(const CCharString& quest) const { LOGF("%s", (g_buf, "IsQuestCompleted(%s)", quest.PStringData)); return g_questDone; }
    void SetQuestAsCompleted(const CCharString& quest, bool a, bool b, bool c) const { LOGF("%s", (g_buf, "SetQuestAsCompleted(%s,%d,%d,%d)", quest.PStringData, a, b, c)); }
    void MiniMapAddMarker(const CScriptThing& thing, const CCharString& g) const { LOGF("%s", (g_buf, "MiniMapAddMarker(%d,%s)", thing.Id, g.PStringData)); }
    void SetThingHasInformation(const CScriptThing& thing, bool info, bool a, bool b) const { LOGF("%s", (g_buf, "HasInfo(%d,%d,%d,%d)", thing.Id, info, a, b)); }
    void EntitySetWillBeUsingNarrator(const CScriptThing& thing, const CCharString& n) const { LOGF("%s", (g_buf, "Narrator(%d,%s)", thing.Id, n.PStringData)); }
    void OpenDoor(const CScriptThing& door) const { LOGF("%s", (g_buf, "OpenDoor(%d)", door.Id)); }
    void CloseDoor(const CScriptThing& door) const { LOGF("%s", (g_buf, "CloseDoor(%d)", door.Id)); }
    void EntitySetTargetingType(const CScriptThing& thing, ETargetingType t) const { LOGF("%s", (g_buf, "TargetingType(%d,%d)", thing.Id, (int)t)); }
    void SetThingPersistent(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "Persistent(%d,%d)", thing.Id, v)); }
    void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "WalkThrough(%d,%d)", thing.Id, v)); }
    void EntitySetAsRespondToHit(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "RespondToHit(%d,%d)", thing.Id, v)); }
    void EntitySetAsDamageable(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "Damageable(%d,%d)", thing.Id, v)); }
    void EntitySetAsKillable(const CScriptThing& thing, bool v, bool b) const { LOGF("%s", (g_buf, "Killable(%d,%d,%d)", thing.Id, v, b)); }
    void EntitySetAllowBossPhaseChanges(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "BossPhase(%d,%d)", thing.Id, v)); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "Combo(%d,%d)", thing.Id, v)); }
    void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing& g, const CCharString& n) const { LOGF("%s", (g_buf, "GenScriptName(%d,%s)", g.Id, n.PStringData)); }
    void SetCreatureGeneratorEnabled(const CScriptThing& g, bool v) const { LOGF("%s", (g_buf, "GenEnabled(%d,%d)", g.Id, v)); }
    void SetCreatureGeneratorsEnabled(const CCharString& group, bool v) const { LOGF("%s", (g_buf, "GensEnabled(%s,%d)", group.PStringData, v)); }
    bool IsDiggingSpotEnabled(const CScriptThing& spot) const { logs("IsDigging"); return g_digFrames-- > 0; }
    void AddItemToContainer(const CScriptThing& c, const CCharString& item) const { LOGF("%s", (g_buf, "AddItem(%d,%s)", c.Id, item.PStringData)); }
    void EntitySetDeathContainerAsEnabled(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "DeathContainer(%d,%d)", thing.Id, v)); }
    void EntitySetStategroupEnabled(const CScriptThing& thing, const CCharString& g, bool v) const { LOGF("%s", (g_buf, "Stategroup(%d,%s,%d)", thing.Id, g.PStringData, v)); }
    void EntitySetAllStategroupsEnabled(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "AllStategroups(%d,%d)", thing.Id, v)); }
    void EntitySetThingAsAllyOfThing(const CScriptThing& thing, const CScriptThing& ally) const { LOGF("%s", (g_buf, "Ally(%d,%d)", thing.Id, ally.Id)); }
    void EntitySetInFaction(const CScriptThing& thing, const CCharString& f) const { LOGF("%s", (g_buf, "Faction(%d,%s)", thing.Id, f.PStringData)); }
    void EntitySetAsOpinionSource(const CScriptThing& thing, const CCharString& s) const { LOGF("%s", (g_buf, "Opinion(%d,%s)", thing.Id, s.PStringData)); }
    void EntitySetAsAwareOfThing(const CScriptThing& thing, const CScriptThing& o) const { LOGF("%s", (g_buf, "Aware(%d,%d)", thing.Id, o.Id)); }
    CCharString GetActiveQuestName() const { return CCharString("Q_Active"); }
    void SetIsThingForcePushable(const CScriptThing& thing, bool v) const { LOGF("%s", (g_buf, "ForcePushable(%d,%d)", thing.Id, v)); }
    void SetIsPushableByHero(CScriptThing thing, bool v) { LOGF("%s", (g_buf, "PushableByHero(%d,%d)", thing.Id, v)); }
};
class CGameScriptInterface : public CGameScriptInterfaceBase { };
static CGameScriptInterface g_game;
CGameScriptInterface* g_pGameScriptInterface = &g_game;
class CEntityScriptBase {
public:
    bool IsActiveThreadTerminating() const { return g_terminate; }
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
};
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue)
    { LOGF("%s", (g_buf, "%s=%d/%d", name, (int)value, (int)defaultValue)); }
};
class CScriptBase { public: bool IsActiveThreadTerminating() { return g_terminate; } };
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;
    TData* MasterScriptData;
};
#include <cstdlib>
int GFCharStringToInt(const CCharString& text) { return std::atoi(text.PStringData); }
class CTimer {
public:
    int TimerIndex;
    CTimer& operator=(int time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript {
public:
    unsigned char _pad000[0xec];
    std::vector<bool> ShouldVillagerSpeak;              // +0xec (Ego_r.pdb +0x128)
    std::vector<bool> CommentGivenThisRound;            // +0xf8
    std::vector<CScriptThing> Prophets;                 // +0x104
    std::vector<CScriptThing> Villagers;                // +0x110 (Ego_r.pdb +0x160)
    class CFH_Villager;
};
class CQ_FireHeartScript::CFH_Villager : public CEntityScriptBase {
public:
    CQ_FireHeartScript*       ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    int                      MyID;                       // +0x1c
public:
    virtual void Init();
};
}

void NScript::CQ_FireHeartScript::CFH_Villager::Init()
{
    MyID = GFCharStringToInt(Me.GetDataString());
    ParentClass->Villagers[MyID] = Me;
    ParentClass->ShouldVillagerSpeak[MyID] = false;
    Game->SetIsPushableByHero(Me, false);
}

static int fail(int line) { std::printf("00d374a0_TEST FAIL (line %d) %s\n", line, g_log); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    NScript::CQ_FireHeartScript::CFH_Villager e;
    e.Game = &g_game; e.Me.Id = 7;
    static NScript::CQ_FireHeartScript parent; std::memset(&parent, 0, sizeof parent); e.ParentClass = &parent;
    g_log[0] = 0; g_terminate = false; g_frames = 0; g_onFrame = 0;
    e.Me.Data = "3"; parent.ShouldVillagerSpeak.items[3] = true; parent.ShouldVillagerSpeak.items[2] = true;
    e.Init();
    CHECK(e.MyID == 3 && parent.Villagers.items[3].Id == 7 && parent.Villagers.items[2].Id == 0);
    CHECK(!parent.ShouldVillagerSpeak.items[3] && parent.ShouldVillagerSpeak.items[2]);
    CHECK(strcmp(g_log, "PushableByHero(7,0);") == 0);
    std::printf("00d374a0_TEST PASS\n");
    return 0;
}