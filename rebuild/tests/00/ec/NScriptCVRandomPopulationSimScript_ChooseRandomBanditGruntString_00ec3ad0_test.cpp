// Behaviour test for 0x00EC3AD0 CV_RandomPopulationSimScript::ChooseRandomBanditGruntString: level digits + variant.
#include <cstdio>
#include <cstring>
static int g_roll = 0;
static int test_rand() { return g_roll; }
struct CCharString {
    char text[64];
    CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    CCharString& operator=(const char* t) { std::strcpy(text, t); return *this; }
};
static CCharString GFIntToCharString(long v) { CCharString r(""); std::sprintf(r.text, "%ld", v); return r; }
static CCharString operator+(const char* l, const CCharString& r) { CCharString s(l); std::strcat(s.text, r.text); return s; }
static CCharString operator+(const CCharString& l, const CCharString& r) { CCharString s(l); std::strcat(s.text, r.text); return s; }
static inline CCharString EmptyName() { CCharString name("", -1); return name; }
struct CV_RandomPopulationSimScript {
    CCharString ChooseRandomBanditGruntString(long level)
    {
        CCharString grunt = EmptyName();
        CCharString variant("_A", -1);
        switch (test_rand() % 4)
        {
        case 0: variant = "_A"; break;
        case 1: variant = "_B"; break;
        case 2: variant = "_C"; break;
        case 3: variant = "_D"; break;
        }
        return "CREATURE_BANDIT_GRUNT_LEVEL" + GFIntToCharString(level) + variant;
    }
};
int main()
{
    CV_RandomPopulationSimScript s;
    g_roll = 7;  CCharString a = s.ChooseRandomBanditGruntString(2);
    g_roll = 4;  CCharString b = s.ChooseRandomBanditGruntString(3);
    g_roll = -2; CCharString c = s.ChooseRandomBanditGruntString(1);   // negative remainder keeps the initial "_A"
    g_roll = 5;  CCharString d = s.ChooseRandomBanditGruntString(4);
    if (std::strcmp(a.text, "CREATURE_BANDIT_GRUNT_LEVEL2_D") || std::strcmp(b.text, "CREATURE_BANDIT_GRUNT_LEVEL3_A")
        || std::strcmp(c.text, "CREATURE_BANDIT_GRUNT_LEVEL1_A") || std::strcmp(d.text, "CREATURE_BANDIT_GRUNT_LEVEL4_B")) {
        std::printf("00ec3ad0_TEST FAIL\n"); return 1;
    }
    std::printf("00ec3ad0_TEST PASS\n");
    return 0;
}