#include <cstdio>
#include <cstring>
#include <vector>
// behaviour test: CCOE_Victim::Main lets Me follow the hero, allies Me with the hero, every other victim and every
// live evil acolyte (both directions), then idles until the thread terminates
static int Fail(const char* what, const char* log) { std::printf("00e6b400_TEST FAIL %s [%s]\n", what, log); return 1; }
#define CHECK(cond) do { if (!(cond)) return Fail(#cond, g_log); } while (0)
static char g_log[4096];
static int g_frame = 0, g_termAt = 1000;
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    const char* Name; bool Alive;
    CScriptThing(const char* n = "", bool alive = true) : Name(n), Alive(alive) { }
    bool IsAlive() const { return Alive; }
    bool operator==(const CScriptThing& o) const { return std::strcmp(Name, o.Name) == 0; }
    bool operator!=(const CScriptThing& o) const { return !(*this == o); }
};
struct CGame {
    std::vector<CScriptThing> Victims, Acolytes; CScriptThing Hero;
    long GetAllThingsWithScriptName(const char* n, std::vector<CScriptThing>& out) const
    { out = std::strcmp(n, "COE_Victim") == 0 ? Victims : std::strcmp(n, "COE_AcolyteEvil") == 0 ? Acolytes : std::vector<CScriptThing>(); return (long)out.size(); }
    CScriptThing& GetHero() { return Hero; }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& t, bool b) const { char x[64]; std::sprintf(x, "follow %s %d", t.Name, (int)b); Log(x); }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) const { char x[64]; std::sprintf(x, "%s>%s", a.Name, b.Name); Log(x); }
    void NewScriptFrame() const { ++g_frame; }
};
struct CCOE_Victim {
    CGame* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CCOE_Victim::Main()
{
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    std::vector<CScriptThing> victims;
    Game->GetAllThingsWithScriptName("COE_Victim", victims);
    std::vector<CScriptThing> acolytes;
    long acolyteCount = Game->GetAllThingsWithScriptName("COE_AcolyteEvil", acolytes);
    for (unsigned int i = 0; i < victims.size(); ++i) {
        Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        if (Me != victims[i]) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsAllyOfThing(Me, victims[i]);
            Game->EntitySetThingAsAllyOfThing(victims[i], Me);
        }
        for (long j = 0; j < acolyteCount; ++j) {
            if (acolytes[j].IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetThingAsAllyOfThing(Me, acolytes[j]);
                Game->EntitySetThingAsAllyOfThing(acolytes[j], Me);
            }
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
int main()
{
    CGame game; game.Hero = CScriptThing("H");
    game.Victims.push_back(CScriptThing("V1")); game.Victims.push_back(CScriptThing("V2"));
    game.Acolytes.push_back(CScriptThing("A1", false)); game.Acolytes.push_back(CScriptThing("A2"));
    CCOE_Victim v; v.Game = &game; v.Me = CScriptThing("V1"); g_termAt = 5;
    v.Main();
    CHECK(std::strcmp(g_log, "follow V1 1;V1>H;H>V1;V1>A2;A2>V1;V1>H;H>V1;V1>V2;V2>V1;V1>A2;A2>V1;") == 0 && g_frame == 5);
    g_log[0] = 0; g_frame = 0; g_termAt = 0;
    v.Main();
    CHECK(std::strcmp(g_log, "follow V1 1;V1>H;H>V1;") == 0 && g_frame == 0);
    std::printf("00e6b400_TEST PASS\n");
    return 0;
}
