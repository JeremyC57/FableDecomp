// Behaviour test for CV_RandomPopulationSimScript::ChooseRandomVillagerFromDefs (0x00EC3130): scripted rand values
// pick the category (value % 10) and the entry (value % n); the generator index is bumped every call.
#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* Text;
    CCharString() : Text("") {}
    CCharString(const CCharString& o) : Text(o.Text) {}
    const CCharString& operator=(const char* t) { Text = t; return *this; }
};
static int g_seq[4]; static int g_pos = 0;
static int fake_rand() { return g_seq[g_pos++]; }
struct CV_RandomPopulationSimScript {
    unsigned long VillagerCreatureGeneratorIndex;
    CCharString ChooseRandomVillagerFromDefs()
    {
        CCharString defName;
        switch (fake_rand() % 10) {
        case 0:
        case 1: {
            const char* names[4] = { "CREATURE_TRADER_STOCKED_01", "CREATURE_TRADER_STOCKED_02",
                                     "CREATURE_TRADER_STOCKED_03", "CREATURE_TRADER_STOCKED_04" };
            defName = names[fake_rand() % 4];
            break;
        }
        case 2: {
            const char* names[3] = { "CREATURE_TRADER_STOCKED_TATTOOIST_01", "CREATURE_TRADER_STOCKED_TATTOOIST_02",
                                     "CREATURE_TRADER_STOCKED_TATTOOIST_03" };
            defName = names[fake_rand() % 3];
            break;
        }
        case 3: {
            const char* names[2] = { "CREATURE_TRADER_STOCKED_BARBER_01", "CREATURE_TRADER_STOCKED_BARBER_02" };
            defName = names[fake_rand() % 2];
            break;
        }
        default:
            defName = "CREATURE_TRADER_01";
            break;
        }
        ++VillagerCreatureGeneratorIndex;
        return defName;
    }
};
static bool check(int a, int b, const char* want, CV_RandomPopulationSimScript& s)
{
    g_seq[0] = a; g_seq[1] = b; g_pos = 0;
    CCharString r = s.ChooseRandomVillagerFromDefs();
    return std::strcmp(r.Text, want) == 0;
}
int main()
{
    CV_RandomPopulationSimScript s; s.VillagerCreatureGeneratorIndex = 5;
    bool ok = check(10, 7, "CREATURE_TRADER_STOCKED_04", s) && check(21, 4, "CREATURE_TRADER_STOCKED_01", s)
        && check(32, 5, "CREATURE_TRADER_STOCKED_TATTOOIST_03", s) && check(13, 3, "CREATURE_TRADER_STOCKED_BARBER_02", s)
        && check(9, 0, "CREATURE_TRADER_01", s) && check(4, 0, "CREATURE_TRADER_01", s);
    ok = ok && s.VillagerCreatureGeneratorIndex == 11;
    if (!ok) { std::printf("00ec3130_TEST FAIL\n"); return 1; }
    std::printf("00ec3130_TEST PASS\n");
    return 0;
}