#include <cstdio>
#include <cstring>

static char g_log[1024];
static char g_b[128];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void ResetLog() { g_log[0] = 0; }
static int Check(const char* expect) { return std::strcmp(g_log, expect) == 0; }

class CCharString {
public:
    const char* Text;
    CCharString() : Text("") { }
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
};

class CScriptThing {
public:
    int         Id;
    bool        Killed;
    const char* Killer;
    CScriptThing() : Id(0), Killed(false), Killer("") { }
    bool MsgIsKilledBy(const CCharString& killer) const { return Killed && std::strcmp(killer.Text, Killer) == 0; }
};

class CGame {
public:
    CScriptThing Hero;
    bool         RegionLoaded;
    int          PreunloadAfter;
    CGame() : RegionLoaded(false), PreunloadAfter(0) { Hero.Id = 99; }
    void NewScriptFrame() const { Log("Frame"); }
    bool IsRegionLoaded(const CCharString& r) const { std::sprintf(g_b, "IsRegionLoaded(%s)", r.Text); Log(g_b); return RegionLoaded; }
    bool MsgOnRegionPreunload(CCharString& r) { Log("Preunload?"); r.Text = "R"; if (PreunloadAfter == 0) return true; --PreunloadAfter; return false; }
    CScriptThing& GetHero() { Log("GetHero"); return Hero; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { std::sprintf(g_b, "Remove(%d,%d,%d)", t.Id, (int)a, (int)b); Log(g_b); }
    void EntityFollowThing(const CScriptThing& t, const CScriptThing& u, float d, bool r) const { std::sprintf(g_b, "Follow(%d,%d,%d,%d)", t.Id, u.Id, (int)(d * 10.0f), (int)r); Log(g_b); }
    void DeactivateQuestLater(const CCharString& q, unsigned long d) const { std::sprintf(g_b, "Deactivate(%s,%lu)", q.Text, d); Log(g_b); }
    void RemoveQuestInfoElement(long e) const { std::sprintf(g_b, "RemoveInfo(%ld)", e); Log(g_b); }
    void DisplayTime(bool d) const { std::sprintf(g_b, "DisplayTime(%d)", (int)d); Log(g_b); }
    void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) const { std::sprintf(g_b, "Info(%d,%d%d%d)", t.Id, (int)a, (int)b, (int)c); Log(g_b); }
    void SetThingPersistent(const CScriptThing& t, bool p) const { std::sprintf(g_b, "Persistent(%d,%d)", t.Id, (int)p); Log(g_b); }
    void EntitySetAsDamageable(const CScriptThing& t, bool d) const { std::sprintf(g_b, "Damageable(%d,%d)", t.Id, (int)d); Log(g_b); }
    void EntitySetThingAsAllyOfThing(const CScriptThing& t, const CScriptThing& a) const { std::sprintf(g_b, "Ally(%d,%d)", t.Id, a.Id); Log(g_b); }
    void EntitySetInFaction(const CScriptThing& t, const CCharString& f) const { std::sprintf(g_b, "Faction(%d,%s)", t.Id, f.Text); Log(g_b); }
    void EntitySetPersonalityOverride(const CScriptThing& t, const CCharString& p) const { std::sprintf(g_b, "Personality(%d,%s)", t.Id, p.Text); Log(g_b); }
    CCharString GetActiveQuestName() const { Log("GetActiveQuestName"); return CCharString("Q_TOGGLE"); }
};
class CEntity {
public:
    CGame*       Game;
    CScriptThing Me;
    CEntity() : Game(0) { Me.Id = 7; }
};
class CArena { public: long TotalCreatures[3]; long NewCrowdPoints; };
class CSUMMONED_CREATURE : public CEntity {
public:
    CArena* ParentClass;
    void OnPredicateFail()
    {
        --ParentClass->TotalCreatures[0];
        if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO"))
            ParentClass->NewCrowdPoints += 8;
    }
};

int main()
{
    CArena arena; arena.TotalCreatures[0] = 5; arena.TotalCreatures[1] = 1; arena.NewCrowdPoints = 10;
    CSUMMONED_CREATURE e; e.ParentClass = &arena;
    e.OnPredicateFail();
    if (arena.TotalCreatures[0] != 4 || arena.TotalCreatures[1] != 1 || arena.NewCrowdPoints != 10) do { std::printf("00f1b1b0_TEST FAIL\n"); return 1; } while (0);
    e.Me.Killed = true; e.Me.Killer = "SCRIPT_NAME_HERO"; e.OnPredicateFail();
    if (arena.TotalCreatures[0] != 3 || arena.NewCrowdPoints != 18) do { std::printf("00f1b1b0_TEST FAIL\n"); return 1; } while (0);
    e.Me.Killer = "SOMEONE"; e.OnPredicateFail();
    if (arena.NewCrowdPoints != 18) do { std::printf("00f1b1b0_TEST FAIL\n"); return 1; } while (0);
    std::printf("00f1b1b0_TEST PASS\n");
    return 0;
}