#include <cstdio>
static int g_registered = 0, g_updates = 0, g_held = 0, g_alive = 0;
struct CScriptThing { int id; CScriptThing(int i) : id(i) { ++g_alive; } CScriptThing(const CScriptThing& o) : id(o.id) { ++g_alive; } ~CScriptThing() { --g_alive; } };
struct CGameScriptInterface {
    int m_doneAfter; const CScriptThing* m_held; long m_flag;
    virtual ~CGameScriptInterface() { }
    virtual void Update() { ++g_updates; }
    virtual void HoldThing(const CScriptThing& thing, long flag) { ++g_held; m_held = &thing; m_flag = flag; }
};
struct CIsThingAlive {
    CScriptThing m_thing;
    virtual ~CIsThingAlive() { }
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
};
struct CScript {
    CGameScriptInterface* Game; CScriptThing Me; int m_filterId;
    CScript() : Me(77) { }
    void RegisterFilter(CIsThingAlive& filter) { ++g_registered; m_filterId = filter.m_thing.id; }
    bool IsDone() { return g_updates >= Game->m_doneAfter; }
    void Main()
    {
        {
            CIsThingAlive filter(Me);
            RegisterFilter(filter);
        }
        Game->Update();
        if (!IsDone()) {
            Game->HoldThing(Me, 0);
            do {
                Game->Update();
            } while (!IsDone());
        }
    }
};
int main()
{
    CGameScriptInterface g; g.m_doneAfter = 4; g.m_held = 0; g.m_flag = -1;
    CScript s; s.Game = &g;
    s.Main();
    if (g_registered != 1 || s.m_filterId != 77 || g_alive != 1 || g_updates != 4 || g_held != 1 || g.m_held != &s.Me || g.m_flag != 0)
    { std::printf("00d69f50_TEST FAIL\n"); return 1; }
    g_updates = 0; g_held = 0; g.m_doneAfter = 1;
    s.Main();
    if (g_updates != 1 || g_held != 0 || g_registered != 2) { std::printf("00d69f50_TEST FAIL\n"); return 1; }
    std::printf("00d69f50_TEST PASS\n");
    return 0;
}