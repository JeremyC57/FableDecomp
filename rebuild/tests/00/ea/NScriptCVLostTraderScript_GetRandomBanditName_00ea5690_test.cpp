// Behaviour test for 0x00EA5690 CV_LostTraderScript::GetRandomBanditName: rand() % 10 selects the def name.
#include <cstdio>
#include <cstring>
static int g_roll = 0;
static int test_rand() { return g_roll; }
struct CCharString {
    char text[48];
    CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
};
static inline CCharString ShouldNotHappenName() { CCharString name("Should not happen", -1); return name; }
struct CV_LostTraderScript {
    CCharString GetRandomBanditName()
    {
        CCharString bandit = ShouldNotHappenName();
        switch (test_rand() % 10)
        {
        case 0: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_A", -1);
        case 1: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_B", -1);
        case 2: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_C", -1);
        case 3: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_D", -1);
        case 4: return CCharString("CREATURE_BANDIT_FIRE_ARCHER_LEVEL2", -1);
        case 5: return CCharString("CREATURE_BANDIT_FIRE_ARCHER_LEVEL3", -1);
        case 6: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_A", -1);
        case 7: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_B", -1);
        case 8: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_C", -1);
        case 9: return CCharString("CREATURE_BANDIT_ARCHER_LEVEL3", -1);
        }
        return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_D", -1);
    }
};
int main()
{
    CV_LostTraderScript s;
    g_roll = 13; CCharString a = s.GetRandomBanditName();
    g_roll = 9;  CCharString b = s.GetRandomBanditName();
    g_roll = -3; CCharString c = s.GetRandomBanditName();   // negative remainder -> default arm
    g_roll = 24; CCharString d = s.GetRandomBanditName();
    if (std::strcmp(a.text, "CREATURE_BANDIT_GRUNT_LEVEL2_D") || std::strcmp(b.text, "CREATURE_BANDIT_ARCHER_LEVEL3")
        || std::strcmp(c.text, "CREATURE_BANDIT_GRUNT_LEVEL3_D") || std::strcmp(d.text, "CREATURE_BANDIT_FIRE_ARCHER_LEVEL2")) {
        std::printf("00ea5690_TEST FAIL\n"); return 1;
    }
    std::printf("00ea5690_TEST PASS\n");
    return 0;
}