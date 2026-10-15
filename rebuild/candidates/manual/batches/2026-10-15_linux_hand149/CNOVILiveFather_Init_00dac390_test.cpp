#include <cstdio>
#include <cstring>
struct CScriptThing { int id; int* refs; CScriptThing(int i, int* r) : id(i), refs(r) { } CScriptThing(const CScriptThing& o) : id(o.id), refs(o.refs) { ++*refs; } };
static char g_log[512];
static bool g_badThing = false;
struct CGame {
    void Log(const char* f, const CScriptThing& t, int n, bool a, bool b, bool c)
    {
        if (t.id != 77) g_badThing = true;
        char buf[64]; std::sprintf(buf, "%s(%d", f, a ? 1 : 0);
        if (n > 1) std::sprintf(buf + std::strlen(buf), ",%d", b ? 1 : 0);
        if (n > 2) std::sprintf(buf + std::strlen(buf), ",%d", c ? 1 : 0);
        std::strcat(buf, ");"); std::strcat(g_log, buf);
    }
    void EntitySetAsDamageable(const CScriptThing& t, bool a) { Log("EntitySetAsDamageable", t, 1, a, 0, 0); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& t, bool a) { Log("EntitySetAsToAddToComboMultiplierWhenHit", t, 1, a, 0, 0); }
    void EntitySetAsUseMovementInActions(const CScriptThing& t, bool a) { Log("EntitySetAsUseMovementInActions", t, 1, a, 0, 0); }
    void EntitySetAsScared(const CScriptThing& t, bool a) { Log("EntitySetAsScared", t, 1, a, 0, 0); }
    void EntitySetDeedReactionsEnabled(const CScriptThing& t, bool a) { Log("EntitySetDeedReactionsEnabled", t, 1, a, 0, 0); }
    void EntitySetAsKillable(const CScriptThing& t, bool a, bool b) { Log("EntitySetAsKillable", t, 2, a, b, 0); }
    void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) { Log("SetThingHasInformation", t, 3, a, b, c); }
    void SetIsPushableByHero(CScriptThing t, bool a) { Log("SetIsPushableByHero", t, 1, a, 0, 0); }
};
struct CNOVI_LiveFather {
    CGame* Game; CScriptThing Me;
    long                        PenniesGiven;              
    CNOVI_LiveFather(CGame* g, int* refs) : Game(g), Me(77, refs) { }
    void Init()
    {
        PenniesGiven = 0;
        Game->EntitySetAsDamageable(Me, false);
        Game->EntitySetAsKillable(Me, false, false);
        Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
        Game->SetThingHasInformation(Me, false, true, false);
        Game->SetIsPushableByHero(Me, false);
        Game->EntitySetDeedReactionsEnabled(Me, false);
    }
};
int main()
{
    int refs = 1; CGame g; g_log[0] = 0;
    CNOVI_LiveFather s(&g, &refs);
    s.PenniesGiven = 1;
    s.Init();
    bool fail = std::strcmp(g_log, "EntitySetAsDamageable(0);EntitySetAsKillable(0,0);EntitySetAsToAddToComboMultiplierWhenHit(0);SetThingHasInformation(0,1,0);SetIsPushableByHero(0);EntitySetDeedReactionsEnabled(0);") != 0 || g_badThing || refs != 2;
    if (s.PenniesGiven != 0) fail = true;
    if (fail) { std::printf("00dac390_TEST FAIL\n"); return 1; }
    std::printf("00dac390_TEST PASS\n");
    return 0;
}
