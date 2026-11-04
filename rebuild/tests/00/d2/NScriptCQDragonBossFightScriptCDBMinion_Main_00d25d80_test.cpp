// Behaviour test for 0x00D25D80 CDBSummoner::Main: runs the reconstructed entity-script body over a recording game interface;
// checks the filter, the best-enemy target, the wait on the parent state and the final RemoveThing(Me, true, true).
#include <cstdio>
static int g_aware = 1000;
class CScriptThing { public:
    int id;
    CScriptThing(int i) : id(i) { }
    CScriptThing(const CScriptThing& o) : id(o.id) { }
    ~CScriptThing() { }
    virtual bool IsAwareOfHero() const { return --g_aware <= 0; }
};
static long* g_state = 0;
static int g_updates = 0, g_removes = 0, g_targets = 0; static bool g_ra = false, g_rb = false; static int g_targetId = 0, g_targetOf = 0;
static CScriptThing g_hero(99);
class CGameScriptInterface { public:
    virtual void Update() { ++g_updates; if (g_state && g_updates >= 4) *g_state = 4; }
    virtual const CScriptThing& GetHero() { return g_hero; }
    virtual void RemoveThing(const CScriptThing& t, bool a, bool b) const { ++g_removes; g_ra = a; g_rb = b; (void)t; }
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) { ++g_targets; g_targetOf = thing.id; g_targetId = target.id; }
};
static int g_filters = 0, g_filterId = 0;
struct CIsThingAlive {
    virtual void v00() { }
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
static int g_doneAfter = 1000, g_doneCalls = 0;
struct SParent { long m_state; };
class CDBSummoner {
public:
    CGameScriptInterface* Game; CScriptThing Me; SParent* ParentClass;
    CDBSummoner(CGameScriptInterface* g) : Game(g), Me(7), ParentClass(0) { }
    void RegisterFilter(CIsThingAlive& f) { ++g_filters; g_filterId = f.m_thing.id; }
    bool IsDone() { return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
void CDBSummoner::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->Update();
    if (IsDone())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (ParentClass->m_state != 4) {
        Game->Update();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->RemoveThing(Me, true, true);
}
int main()
{
    CGameScriptInterface game; SParent parent; parent.m_state = 0; g_state = &parent.m_state;
    CDBSummoner s(&game); s.ParentClass = &parent;
    s.Main();                                      // the parent reaches state 4 after three updates
    bool ok = g_filters == 1 && g_filterId == 7 && g_targets == 1 && g_targetOf == 7 && g_targetId == 99 &&
              g_updates == 4 && g_removes == 1 && g_ra && g_rb;
    g_doneCalls = 0; g_doneAfter = 2; parent.m_state = 0; s.Main();   // done inside the wait loop: no removal
    ok = ok && g_removes == 1 && g_targets == 2;
    if (!ok) { std::printf("00d25d80_TEST FAIL\n"); return 1; }
    std::printf("00d25d80_TEST PASS\n");
    return 0;
}