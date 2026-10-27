// Behaviour test for 0x00D6CCC0 (NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion()): the row's model and body over a recording mock of the script interface.
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
static void* g_q;
static void banditFrame();
class CTimer {
public:
    int TimerIndex;
    CTimer& operator=(int time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _pad048[0x2];
    bool BanditKilled;                                  // +0x4a (Ego_r.pdb +0x5e)
    unsigned char _pad04b[0x3];
    bool BanditsLeftRegion;                             // +0x4e (Ego_r.pdb +0x62)
    unsigned char _pad04f[0x19];
    int BanditsAtNextRegion;                           // +0x68 (Ego_r.pdb +0x7c)
    unsigned char _pad06c[0x40];                        // +0x74 Executioner .. +0xac CurrentEnemyVector
    std::vector<CScriptThing> CurrentEnemyVector;       // +0xac
    std::vector<CScriptThing> CurrentBanditVector;      // +0xb8 (Ego_r.pdb +0xd0)
    void MonitorBanditRegion();
};
}

void NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion()
{
    BanditsLeftRegion = false;
    CurrentBanditVector.clear();
    Game->GetAllThingsWithScriptName("HTE_FreedomFighter", CurrentBanditVector);
    while ((unsigned int)BanditsAtNextRegion < CurrentBanditVector.size()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (BanditKilled) {
            if (IsActiveThreadTerminating()) return;
            CurrentBanditVector.clear();
            Game->GetAllThingsWithScriptName("HTE_FreedomFighter", CurrentBanditVector);
            BanditKilled = false;
        }
    }
    if (IsActiveThreadTerminating()) return;
    BanditsLeftRegion = true;
}
static void banditFrame()
{
    NScript::CQ_HangingTreeEvilScript* q = (NScript::CQ_HangingTreeEvilScript*)g_q;
    if (g_frames == 1) { q->BanditKilled = true; g_things = 2; }
    if (g_frames == 2) q->BanditsAtNextRegion = 2;
}

static int fail(int line) { std::printf("00d6ccc0_TEST FAIL (line %d) %s\n", line, g_log); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    NScript::CQ_HangingTreeEvilScript q; q.Game = &g_game; g_q = &q;
    g_log[0] = 0; g_terminate = false; g_frames = 0; g_onFrame = 0;
    q.BanditsAtNextRegion = 0; q.BanditKilled = false; q.BanditsLeftRegion = true; q.CurrentBanditVector.n = 5;
    g_things = 3; g_onFrame = banditFrame;
    q.MonitorBanditRegion();
    CHECK(strcmp(g_log, "GetAll(HTE_FreedomFighter);Frame;GetAll(HTE_FreedomFighter);Frame;") == 0);
    CHECK(q.CurrentBanditVector.size() == 2 && q.BanditsLeftRegion && !q.BanditKilled);
    g_log[0] = 0; g_terminate = false; g_frames = 0; g_onFrame = 0;
    q.BanditsAtNextRegion = 0; g_things = 1; g_terminate = true;
    q.MonitorBanditRegion();
    CHECK(strcmp(g_log, "GetAll(HTE_FreedomFighter);Frame;") == 0 && !q.BanditsLeftRegion);
    std::printf("00d6ccc0_TEST PASS\n");
    return 0;
}