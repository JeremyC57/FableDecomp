// Behaviour test for 0x00E04970 CDW5_Balv::Main: runs the reconstructed entity-script body over a recording game interface;
// checks the filter, the awareness wait, the best-enemy target and the update-until-done loop.
#include <cstdio>
static int g_aware = 1000;
struct CScriptThing {
    int id;
    CScriptThing(int i) : id(i) { }
    CScriptThing(const CScriptThing& o) : id(o.id) { }
    ~CScriptThing() { }
    virtual bool IsAwareOfHero() const { return --g_aware <= 0; }
};
static int g_updates = 0, g_removes = 0, g_targets = 0; static bool g_ra = false, g_rb = false; static int g_targetId = 0, g_targetOf = 0;
static CScriptThing g_hero(99);
struct CGameScriptInterface {
    virtual void Update() { ++g_updates; }
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
class CDW5_Balv {
public:
    CGameScriptInterface* Game; CScriptThing Me; void* ParentClass;
    CDW5_Balv(CGameScriptInterface* g) : Game(g), Me(7), ParentClass(0) { }
    void RegisterFilter(CIsThingAlive& f) { ++g_filters; g_filterId = f.m_thing.id; }
    bool IsDone() { return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
void CDW5_Balv::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->Update();
    if (IsDone())
        return;
    while (!Me.IsAwareOfHero()) {
        Game->Update();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    for (;;) {
        Game->Update();
        if (IsDone())
            return;
    }
}
int main()
{
    CGameScriptInterface game; g_aware = 3; g_doneAfter = 8;
    CDW5_Balv s(&game);
    s.Main();                                      // aware on the third poll, then idles until done
    bool ok = g_filters == 1 && g_filterId == 7 && g_targets == 1 && g_targetOf == 7 && g_targetId == 99 && g_doneCalls == 8 && g_updates == 7;
    g_doneCalls = 0; g_doneAfter = 2; g_aware = 100; s.Main();   // done while waiting: no target
    ok = ok && g_targets == 1;
    if (!ok) { std::printf("00e04970_TEST FAIL\n"); return 1; }
    std::printf("00e04970_TEST PASS\n");
    return 0;
}