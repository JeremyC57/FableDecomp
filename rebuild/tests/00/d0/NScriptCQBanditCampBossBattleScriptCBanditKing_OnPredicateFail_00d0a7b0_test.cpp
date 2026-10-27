#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over mock script objects; every interface call is logged.
static char g_log[2048];
static void Log(const char* text) { std::strcat(g_log, text); std::strcat(g_log, ";"); }
static void LogF(const char* fmt, int a, int b, int c, int d)
{
    char buf[128];
    std::sprintf(buf, fmt, a, b, c, d);
    Log(buf);
}
class CCharString {
public:
    const char* PData;
    CCharString(const char* text, long length = -1) : PData(text) { (void)length; }
    ~CCharString() { }
};
class CScriptThing {
public:
    int  Id;
    int  RefCount;
    bool Killed;
    bool Alive;
    bool Unconscious;
    const char* Data;
    CScriptThing(int id = 0) : Id(id), RefCount(1), Killed(false), Alive(true), Unconscious(false), Data("0") { }
    CScriptThing(const CScriptThing& o) : Id(o.Id), RefCount(o.RefCount + 1), Killed(o.Killed), Alive(o.Alive), Unconscious(o.Unconscious), Data(o.Data) { }
    CScriptThing& operator=(const CScriptThing& o) { Id = o.Id; Data = o.Data; return *this; }
    virtual ~CScriptThing() { }
    virtual bool MsgIsKilledBy(const CCharString& killer) const { return Killed && killer.PData[0] == 0; }
    virtual CCharString GetDataString() const { return CCharString(Data); }
    virtual bool IsAlive() const { return Alive; }
    virtual bool IsUnconscious() const { return Unconscious; }
};
enum EOpinionReactionType { };
class CGameScriptInterfaceBase {
public:
    void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) const { LogF("Info(%d,%d,%d,%d)", t.Id, a, b, c); }
    void EntitySetInFaction(const CScriptThing& t, const CCharString& f) const { char b[96]; std::sprintf(b, "Faction(%d,%s)", t.Id, f.PData); Log(b); }
    void SetIsPushableByHero(CScriptThing t, bool p) { LogF("Push(%d,%d,ref%d)", t.Id, p, t.RefCount, 0); }
    void EntitySetOpinionReactionEnabled(const CScriptThing& t, EOpinionReactionType r, bool e) const { LogF("Reaction(%d,%d,%d)", t.Id, (int)r, e, 0); }
    void GiveHeroObject(const CCharString& o, long n, bool b) const { char x[96]; std::sprintf(x, "Give(%s,%ld,%d)", o.PData, n, (int)b); Log(x); }
    void DisplayQuestInfo(bool s) const { LogF("Display(%d)", s, 0, 0, 0); }
    void RemoveQuestInfoElement(long e) const { LogF("Remove(%d)", (int)e, 0, 0, 0); }
    void MiniMapAddMarker(const CScriptThing& t, const CCharString& m) const { char b[96]; std::sprintf(b, "Marker(%d,%s)", t.Id, m.PData); Log(b); }
    void SetThingPersistent(const CScriptThing& t, bool p) const { LogF("Persistent(%d,%d)", t.Id, p, 0, 0); }
    void EntitySetAsHavingBoundHands(CScriptThing& t, bool b) const { LogF("Bound(%d,%d)", t.Id, b, 0, 0); }
    void EntitySetAsKillable(const CScriptThing& t, bool k, bool b) const { LogF("Killable(%d,%d,%d)", t.Id, k, b, 0); }
    void EntitySetAsDisplayingEmoteIcon(const CScriptThing& t, bool d) const { LogF("Emote(%d,%d)", t.Id, d, 0, 0); }
    void SetTimer(long timer, long time) { LogF("SetTimer(%d,%d)", (int)timer, (int)time, 0, 0); }
};
static CGameScriptInterfaceBase g_Game;
static CGameScriptInterfaceBase* g_pGameScriptInterface = &g_Game;
class CTimer {
public:
    int TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CEntityScriptBase {
public:
    CGameScriptInterfaceBase* Game;
    CScriptThing              Me;
    CEntityScriptBase() : Game(&g_Game), Me(7) { }
};
struct Parent { bool TwinBladeKilled; };
struct Master { bool BanditCampTwinbladeKilled; };
class Entity : public CEntityScriptBase {
public:
    Parent* ParentClass;
    Master* PMasterData;
    int HealthBarIndex;
    void OnPredicateFail()
    {
        if (Me.MsgIsKilledBy(""))
        {
            ParentClass->TwinBladeKilled = true;
            PMasterData->BanditCampTwinbladeKilled = true;
        }
        Game->DisplayQuestInfo(false);
        Game->RemoveQuestInfoElement(HealthBarIndex);
    }
};

int main()
{
    g_log[0] = 0;
    int ok = 1;
    Parent p; p.TwinBladeKilled = false; Master m; m.BanditCampTwinbladeKilled = false;
    Entity e; e.ParentClass = &p; e.PMasterData = &m; e.HealthBarIndex = 9;
    e.OnPredicateFail();
    ok = !p.TwinBladeKilled && !m.BanditCampTwinbladeKilled && std::strcmp(g_log, "Display(0);Remove(9);") == 0;
    g_log[0] = 0; e.Me.Killed = true;
    e.OnPredicateFail();
    ok = ok && p.TwinBladeKilled && m.BanditCampTwinbladeKilled && std::strcmp(g_log, "Display(0);Remove(9);") == 0;
    if (!ok) { std::printf("00d0a7b0_TEST FAIL\n"); std::printf("%s\n", g_log); return 1; }
    std::printf("00d0a7b0_TEST PASS\n");
    return 0;
}