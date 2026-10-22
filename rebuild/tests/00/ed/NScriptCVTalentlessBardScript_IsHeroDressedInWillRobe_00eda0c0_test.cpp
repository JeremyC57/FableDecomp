// Behaviour test for CV_TalentlessBardScript::IsHeroDressedInWillRobe (00eda0c0): each suit is one short-circuit chain over shirt,
// gloves, trousers and boots (a missing piece stops that suit's checks); every suit is checked in the retail order
// (WILLROBE_EVIL, WILLROBE, WILLROBE_GOOD) and the result is true when any one suit is complete.
#include <cstdio>
#include <cstring>

static const char* g_worn[16];
static int g_wornCount = 0;
static int g_queries = 0;
static bool IsWearing(const char* item)
{
    ++g_queries;
    for (int i = 0; i < g_wornCount; ++i)
        if (std::strcmp(g_worn[i], item) == 0)
            return true;
    return false;
}
static bool Suit(const char* suffix)
{
    static const char* pieces[4] = { "SHIRT", "GLOVES", "TROUSERS", "BOOTS" };
    for (int i = 0; i < 4; ++i) {
        char name[64];
        std::sprintf(name, "OBJECT_HERO_%s_%s", pieces[i], suffix);
        if (!IsWearing(name))
            return false;
    }
    return true;
}
static bool Run()
{
    bool first = Suit("WILLROBE_EVIL");
    bool second = Suit("WILLROBE");
    bool third = Suit("WILLROBE_GOOD");
    return first || second || third;
}
static void Wear(const char* suffix)
{
    static char store[16][64];
    static const char* pieces[4] = { "SHIRT", "GLOVES", "TROUSERS", "BOOTS" };
    for (int i = 0; i < 4; ++i) {
        std::sprintf(store[g_wornCount], "OBJECT_HERO_%s_%s", pieces[i], suffix);
        g_worn[g_wornCount] = store[g_wornCount];
        ++g_wornCount;
    }
}

int main()
{
    bool ok = true;
    g_wornCount = 0; g_queries = 0;
    ok = ok && !Run() && g_queries == 3;                 // nothing worn: one query per suit
    g_wornCount = 0; Wear("WILLROBE_GOOD"); g_queries = 0;
    ok = ok && Run() && g_queries == 6;                  // last suit complete: 1 + 1 + 4 queries, still all three checked
    g_wornCount = 0; Wear("WILLROBE_EVIL"); g_queries = 0;
    ok = ok && Run() && g_queries == 6;                  // first suit complete: the later suits are still evaluated
    g_wornCount = 0; Wear("WILLROBE"); g_wornCount = 3; g_queries = 0;
    ok = ok && !Run() && g_queries == 6;                 // boots missing: the chain runs to the boots and fails
    std::printf(ok ? "00eda0c0_TEST PASS\n" : "00eda0c0_TEST FAIL\n");
    return ok ? 0 : 1;
}