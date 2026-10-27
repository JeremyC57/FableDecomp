// Behaviour test for 0x00DA0160 CMC_SpawnPoint::Init: the mock interface logs every call; the body below is the source's, verbatim.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void ResetLog() { g_log[0] = 0; }
static char g_b[256];
static int Check(const char* expect) { return std::strcmp(g_log, expect) == 0; }

class CCharString {
public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    const CCharString& operator=(const char* text) { Text = text; return *this; }
    bool operator==(const char* text) const { return std::strcmp(Text, text) == 0; }
};
static long GFCharStringToInt(const CCharString& text) { return std::atol(text.Text); }

class CScriptThing {
public:
    int         Id;
    bool        Killed;
    const char* Killer;
    float       Angle;
    const char* Data;
    CScriptThing() : Id(0), Killed(false), Killer(""), Angle(0.0f), Data("") { }
    bool MsgIsKilledBy(const CCharString& killer) const
    {
        if (!Killed) return false;
        return killer.Text[0] == 0 || std::strcmp(killer.Text, Killer) == 0;
    }
    float GetAngleXY() const { return Angle; }
    CCharString GetDataString() const { return CCharString(Data); }
};

class CGame {
public:
    CScriptThing Hero;
    bool         RegionLoaded;
    CGame() : RegionLoaded(false) { Hero.Id = 99; }
    CScriptThing& GetHero() { Log("GetHero"); return Hero; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& t, const CScriptThing& a) const { std::sprintf(g_b, "Ally(%d,%d)", t.Id, a.Id); Log(g_b); }
    void EntitySetInFaction(const CScriptThing& t, const CCharString& f) const { std::sprintf(g_b, "Faction(%d,%s)", t.Id, f.Text); Log(g_b); }
    void EntitySetWillBeUsingNarrator(const CScriptThing& t, const CCharString& n) const { std::sprintf(g_b, "Narrator(%d,%s)", t.Id, n.Text); Log(g_b); }
    void MiniMapAddMarker(const CScriptThing& t, const CCharString& g) const { std::sprintf(g_b, "Marker(%d,%s)", t.Id, g.Text); Log(g_b); }
    void EntitySetCombatAbility(const CScriptThing& t, float v) const { std::sprintf(g_b, "Combat(%d,%d)", t.Id, (int)v); Log(g_b); }
    void SetThingHasInformation(const CScriptThing& t, bool i, bool x, bool y) const { std::sprintf(g_b, "Info(%d,%d%d%d)", t.Id, (int)i, (int)x, (int)y); Log(g_b); }
    void GiveHeroMorality(float m) const { std::sprintf(g_b, "Morality(%d)", (int)(m * 10.0f)); Log(g_b); }
    void RemoveQuestInfoElement(long e) const { std::sprintf(g_b, "RemoveInfo(%ld)", e); Log(g_b); }
    void EntitySetThingAsWantingMoney(const CScriptThing& t, long m) const { std::sprintf(g_b, "Money(%d,%ld)", t.Id, m); Log(g_b); }
    void EntitySetAsKillable(const CScriptThing& t, bool k, bool x) const { std::sprintf(g_b, "Killable(%d,%d,%d)", t.Id, (int)k, (int)x); Log(g_b); }
    bool IsRegionLoaded(const CCharString& r) const { std::sprintf(g_b, "IsRegionLoaded(%s)", r.Text); Log(g_b); return RegionLoaded; }
    void SetIsPushableByHero(CScriptThing t, bool p) { std::sprintf(g_b, "Pushable(%d,%d)", t.Id, (int)p); Log(g_b); }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& t, bool f) const { std::sprintf(g_b, "Follow(%d,%d)", t.Id, (int)f); Log(g_b); }
    void EntitySetOpinionReactionsEnabled(const CScriptThing& t, bool e) const { std::sprintf(g_b, "Opinion(%d,%d)", t.Id, (int)e); Log(g_b); }
    void CloseChest(const CScriptThing& t) const { std::sprintf(g_b, "Close(%d)", t.Id); Log(g_b); }
    void AddItemToContainer(const CScriptThing& t, const CCharString& i) const { std::sprintf(g_b, "AddItem(%d,%s)", t.Id, i.Text); Log(g_b); }
    void EntitySetAllStategroupsEnabled(const CScriptThing& t, bool e) const { std::sprintf(g_b, "AllSG(%d,%d)", t.Id, (int)e); Log(g_b); }
    void EntitySetStategroupEnabled(const CScriptThing& t, const CCharString& g, bool e) const { std::sprintf(g_b, "SG(%d,%s,%d)", t.Id, g.Text, (int)e); Log(g_b); }
    void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& t, bool r) const { std::sprintf(g_b, "Respond(%d,%d)", t.Id, (int)r); Log(g_b); }
};

class CThingList {                                       // stands in for list/vector<CScriptThing>::push_back
public:
    int Ids[8];
    int Count;
    CThingList() : Count(0) { }
    void push_back(const CScriptThing& t) { Ids[Count++] = t.Id; Log("push_back"); }
};

class CEntity {                                          // CEntityScriptBase model: Game, Me, the terminating test
public:
    CGame*       Game;
    CScriptThing Me;
    bool         Terminating;
    CEntity() : Game(0), Terminating(false) { Me.Id = 7; }
    bool IsActiveThreadTerminating() const { Log("IsTerminating"); return Terminating; }
};
class CQuest {                                           // quest-script model: Game, master data
public:
    CGame* Game;
};
class CParent_CMC_SpawnPoint { public: CThingList LeftSpawnPoints; CThingList RightSpawnPoints; };
class CMaster_CMC_SpawnPoint { public: int Unused; };
class CMC_SpawnPoint : public CEntity {
public:
    CParent_CMC_SpawnPoint* ParentClass;
    CMaster_CMC_SpawnPoint* PMasterData;
    bool DoneIntroMinion;
    void Init();
};

void CMC_SpawnPoint::Init()
{
    DoneIntroMinion = false;
    long side = GFCharStringToInt(Me.GetDataString());
    if (side == 0) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->LeftSpawnPoints.push_back(Me);
    } else if (side == 1) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->RightSpawnPoints.push_back(Me);
    } else {
        if (IsActiveThreadTerminating())
            return;
    }
}

#define FAIL(n) { std::printf("00da0160_TEST FAIL %d\n", n); return 1; }
int main()
{
    CGame game;
    CParent_CMC_SpawnPoint parent;
    CMC_SpawnPoint e;
    e.Game = &game; e.ParentClass = &parent;
    e.DoneIntroMinion = true; e.Me.Data = "0";
    ResetLog();
    e.Init();
    if (e.DoneIntroMinion || parent.LeftSpawnPoints.Count != 1 || parent.LeftSpawnPoints.Ids[0] != 7 || parent.RightSpawnPoints.Count != 0) FAIL(1)
    e.Me.Data = "1";
    e.Init();
    if (parent.RightSpawnPoints.Count != 1 || parent.LeftSpawnPoints.Count != 1) FAIL(2)
    e.Terminating = true;
    e.Init();
    if (parent.RightSpawnPoints.Count != 1) FAIL(3)
    e.Terminating = false; e.Me.Data = "5";
    ResetLog();
    e.Init();
    if (!Check("IsTerminating;") || parent.LeftSpawnPoints.Count != 1 || parent.RightSpawnPoints.Count != 1) FAIL(4)
    std::printf("00da0160_TEST PASS\n");
    return 0;
}