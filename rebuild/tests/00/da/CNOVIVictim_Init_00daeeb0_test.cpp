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
struct CNOVI_Victim {
    CGame* Game; CScriptThing Me;
    bool                        DisplayedGameInfo;         
    bool                        DoneThanks;                
    CNOVI_Victim(CGame* g, int* refs) : Game(g), Me(77, refs) { }
    void Init()
    {
        DoneThanks = false;
        DisplayedGameInfo = false;
        Game->EntitySetAsDamageable(Me, false);
        Game->EntitySetAsKillable(Me, false, false);
        Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
        Game->SetThingHasInformation(Me, false, false, false);
        Game->SetIsPushableByHero(Me, false);
        Game->EntitySetAsUseMovementInActions(Me, false);
        Game->EntitySetAsScared(Me, true);
    }
};
int main()
{
    int refs = 1; CGame g; g_log[0] = 0;
    CNOVI_Victim s(&g, &refs);
    s.DoneThanks = true;
    s.DisplayedGameInfo = true;
    s.Init();
    bool fail = std::strcmp(g_log, "EntitySetAsDamageable(0);EntitySetAsKillable(0,0);EntitySetAsToAddToComboMultiplierWhenHit(0);SetThingHasInformation(0,0,0);SetIsPushableByHero(0);EntitySetAsUseMovementInActions(0);EntitySetAsScared(1);") != 0 || g_badThing || refs != 2;
    if (s.DoneThanks != false) fail = true;
    if (s.DisplayedGameInfo != false) fail = true;
    if (fail) { std::printf("00daeeb0_TEST FAIL\n"); return 1; }
    std::printf("00daeeb0_TEST PASS\n");
    return 0;
}