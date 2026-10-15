// Behaviour test for CV_AssassinAttacksScript::GetRegionIdForRegion (0x00E38290).
#include <cstdio>

static int g_destroyed = 0;
struct CCharString {
    const char* m_text;
    CCharString(const char* t) : m_text(t) { }
    CCharString(const CCharString& o) : m_text(o.m_text) { }
    ~CCharString() { ++g_destroyed; }
    bool operator!=(const CCharString& other) const
    {
        const char* a = m_text; const char* b = other.m_text;
        while (*a && *a == *b) { ++a; ++b; }
        return *a != *b;
    }
};

struct RegionData {
    CCharString name;
    long count, max;
    bool assassinActive;
    RegionData() : name(""), count(0), max(0), assassinActive(false) { }
};

struct CV_AssassinAttacksScript {
    RegionData RegionInfo[5];
    long GetRegionIdForRegion(CCharString region);
};

long CV_AssassinAttacksScript::GetRegionIdForRegion(CCharString region)
{
    long i;
    for (i = 0; i < 5; ++i) {
        if (!(RegionInfo[i].name != region))
            break;
    }
    return i;
}

int main()
{
    CV_AssassinAttacksScript s;
    const char* names[5] = { "Bowerstone", "Oakvale", "Knothole", "Twinblade", "Witchwood" };
    for (int i = 0; i < 5; ++i) s.RegionInfo[i].name.m_text = names[i];
    if (s.GetRegionIdForRegion(CCharString("Knothole")) != 2) { std::printf("00e38290_TEST FAIL\n"); return 1; }
    if (s.GetRegionIdForRegion(CCharString("Bowerstone")) != 0) { std::printf("00e38290_TEST FAIL\n"); return 1; }
    if (s.GetRegionIdForRegion(CCharString("Witchwood")) != 4) { std::printf("00e38290_TEST FAIL\n"); return 1; }
    if (s.GetRegionIdForRegion(CCharString("Snowspire")) != 5) { std::printf("00e38290_TEST FAIL\n"); return 1; }
    std::printf("00e38290_TEST PASS\n");
    return 0;
}