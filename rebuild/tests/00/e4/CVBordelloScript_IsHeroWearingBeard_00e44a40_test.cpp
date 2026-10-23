#include <cstdio>
#include <cstring>
struct CCharString {
    const char* p;
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
struct CScriptThing { int id; };
static const char* g_worn = "";
static int g_calls = 0;
struct GameCalls {
    CScriptThing hero;
    CScriptThing& GetHero() { return hero; }
    bool IsWearingHairstyle(CScriptThing&, const CCharString& h) const { ++g_calls; return h.is(g_worn); }
};
struct CV_BordelloScript {
    GameCalls* Game;
    bool IsHeroWearingBeard();
};

bool CV_BordelloScript::IsHeroWearingBeard()
{
    return Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_02")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_03")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_MUTTON_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_LONG_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_CHIN_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_TRAMP_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_BEARD_WATSON_01");
}

int main()
{
    GameCalls g; CV_BordelloScript s; s.Game = &g;
    g_worn = "OBJECT_HERO_BEARD_WATSON_01"; g_calls = 0;
    CHECK(s.IsHeroWearingBeard() && g_calls == 8);
    g_worn = "OBJECT_HERO_BEARD_01"; g_calls = 0;
    CHECK(s.IsHeroWearingBeard() && g_calls == 1);
    g_worn = "OBJECT_HERO_HAIR_SHORT"; g_calls = 0;
    CHECK(!s.IsHeroWearingBeard() && g_calls == 8);
    if (g_fail) { std::printf("00e44a40_TEST FAIL\n"); return 1; }
    std::printf("00e44a40_TEST PASS\n");
    return 0;
}