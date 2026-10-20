// Behaviour test for CTCHeroStats::GetRandomSuitName (0x0057BAD4): the model runs over stub callees that
// record the random range and return a chosen index; the copied name must be the table entry.
#include <cstdio>
#include <cstring>

static const char* g_lastAssigned[16];
static int g_assignCount = 0;
static unsigned long g_range = 0;
static unsigned long g_pick = 0;
static int g_dtorCount = 0;

struct CCharString {
    const char* Text;
    CCharString() : Text(0) {}
    CCharString(const CCharString& other) : Text(other.Text) {}
    CCharString& operator=(const char* text) { Text = text; if (g_assignCount < 16) g_lastAssigned[g_assignCount] = text; ++g_assignCount; return *this; }
};

struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    explicit CCharStringVector(unsigned long count) { _M_start = new CCharString[count]; _M_finish = _M_end_of_storage = _M_start + count; }
    ~CCharStringVector() { delete[] _M_start; ++g_dtorCount; }
    CCharString* begin() { return _M_start; }
};

unsigned long GFRandom(unsigned long range, unsigned long* seed) { g_range = range; ++*seed; return g_pick; }

struct CTCHeroStats {
    unsigned long Seed;
    unsigned long* GetPRandomSeed() const { return const_cast<unsigned long*>(&Seed); }
    CCharString GetRandomSuitName() const;
};

CCharString CTCHeroStats::GetRandomSuitName() const
{
    CCharStringVector nameList(9);
    CCharString* names = nameList.begin();
    names[0] = "HERO_SUIT_NAKED";
    names[1] = "HERO_SUIT_OAK_VALE";
    names[2] = "HERO_SUIT_BANDIT";
    names[3] = "HERO_SUIT_STEALTH";
    names[4] = "HERO_SUIT_ARMOUR";
    names[5] = "HERO_SUIT_BRIGHT_ARMOUR";
    names[6] = "HERO_SUIT_DARK_ARMOUR";
    names[7] = "HERO_SUIT_WILL_ROBE";
    names[8] = "HERO_SUIT_LEATHER_ARMOUR";
    return names[GFRandom(9, GetPRandomSeed())];
}

int main()
{
    CTCHeroStats stats;
    stats.Seed = 100;
    g_pick = 6;
    CCharString r = stats.GetRandomSuitName();
    if (g_range != 9 || stats.Seed != 101 || g_assignCount != 9 || g_dtorCount != 1 ||
        std::strcmp(r.Text, "HERO_SUIT_DARK_ARMOUR") != 0) { std::printf("0057bad4_TEST FAIL\n"); return 1; }
    g_pick = 0; g_assignCount = 0;
    CCharString r2 = stats.GetRandomSuitName();
    if (std::strcmp(r2.Text, "HERO_SUIT_NAKED") != 0 || std::strcmp(g_lastAssigned[8], "HERO_SUIT_LEATHER_ARMOUR") != 0) { std::printf("0057bad4_TEST FAIL\n"); return 1; }
    std::printf("0057bad4_TEST PASS\n");
    return 0;
}
