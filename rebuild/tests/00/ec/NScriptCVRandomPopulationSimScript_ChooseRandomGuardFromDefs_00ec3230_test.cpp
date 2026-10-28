#include <cstdio>
#include <cstring>

// CV_RandomPopulationSimScript::ChooseRandomGuardFromDefs: returns "CREATURE_BS_GUARD" by value and
// advances the guard generator index (the other indices are untouched).
static int g_live = 0;
class CCharString { public:
    char m_text[32];
    CCharString() { m_text[0] = 0; ++g_live; }
    CCharString(const CCharString& other) { std::strcpy(m_text, other.m_text); ++g_live; }
    ~CCharString() { --g_live; }
    const CCharString& operator=(const char* text) { std::strcpy(m_text, text); return *this; }
};
struct CV_RandomPopulationSimScript {
    unsigned long BanditCreatureGeneratorIndex;
    unsigned long VillagerCreatureGeneratorIndex;
    unsigned long GuardCreatureGeneratorIndex;
    CCharString ChooseRandomGuardFromDefs();
};
CCharString CV_RandomPopulationSimScript::ChooseRandomGuardFromDefs()
{
    CCharString defName;
    defName = "CREATURE_BS_GUARD";
    ++GuardCreatureGeneratorIndex;
    return defName;
}

int main()
{
    CV_RandomPopulationSimScript sim;
    sim.BanditCreatureGeneratorIndex = 5; sim.VillagerCreatureGeneratorIndex = 6; sim.GuardCreatureGeneratorIndex = 7;
    bool ok;
    {
        CCharString a = sim.ChooseRandomGuardFromDefs();
        CCharString b = sim.ChooseRandomGuardFromDefs();
        ok = std::strcmp(a.m_text, "CREATURE_BS_GUARD") == 0 && std::strcmp(b.m_text, "CREATURE_BS_GUARD") == 0
            && sim.GuardCreatureGeneratorIndex == 9 && sim.BanditCreatureGeneratorIndex == 5 && sim.VillagerCreatureGeneratorIndex == 6;
    }
    ok = ok && g_live == 0;
    if (!ok) {
        std::printf("00ec3230_TEST FAIL\n");
        return 1;
    }
    std::printf("00ec3230_TEST PASS\n");
    return 0;
}