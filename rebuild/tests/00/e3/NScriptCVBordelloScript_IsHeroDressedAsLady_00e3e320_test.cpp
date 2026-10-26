#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* p;
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
class CScriptThing { public: int id; };
static const char* g_worn[8];
static int g_nworn = 0;
struct GameCalls {
    CScriptThing hero;
    CScriptThing& GetHero() { return hero; }
    bool IsWearingClothingItem(const CScriptThing&, const CCharString& c) const
    {
        for (int i = 0; i < g_nworn; ++i) if (c.is(g_worn[i])) return true;
        return false;
    }
};
struct CV_BordelloScript {
    GameCalls* Game;
    bool IsHeroDressedAsLady();
};

bool CV_BordelloScript::IsHeroDressedAsLady()
{
    long count = 0;
    if (Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS")
        || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS_GOOD")
        || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS_EVIL"))
        ++count;
    if (Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS")
        || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS_GOOD")
        || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS_EVIL"))
        ++count;
    if (Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS")
        || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS_GOOD")
        || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS_EVIL"))
        ++count;
    if (Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_WHOREWIG")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_NO_BOOTS"))
        ++count;
    bool result = count == 4;
    return result;
}

int main()
{
    GameCalls g; CV_BordelloScript s; s.Game = &g;
    g_worn[0] = "OBJECT_HERO_TROUSERS_DRESS_EVIL"; g_worn[1] = "OBJECT_HERO_SHIRT_DRESS"; g_worn[2] = "OBJECT_HERO_GLOVES_DRESS_GOOD";
    g_worn[3] = "OBJECT_HERO_HAT_WHOREWIG"; g_worn[4] = "OBJECT_HERO_NO_BOOTS"; g_nworn = 5;
    CHECK(s.IsHeroDressedAsLady());
    g_nworn = 4;
    CHECK(!s.IsHeroDressedAsLady());
    g_worn[4] = "OBJECT_HERO_NO_BOOTS"; g_worn[1] = "OBJECT_HERO_SHIRT_PLAIN"; g_nworn = 5;
    CHECK(!s.IsHeroDressedAsLady());
    if (g_fail) { std::printf("00e3e320_TEST FAIL\n"); return 1; }
    std::printf("00e3e320_TEST PASS\n");
    return 0;
}