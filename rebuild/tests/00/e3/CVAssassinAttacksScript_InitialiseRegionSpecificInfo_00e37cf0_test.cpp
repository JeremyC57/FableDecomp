// Behaviour test for CV_AssassinAttacksScript::InitialiseRegionSpecificInfo (0x00E37CF0).
#include <cstdio>

static int g_termAfter = -1;   // IsActiveThreadTerminating() true from this call number on (-1: never)
static int g_calls = 0;

struct RegionData { void* name; long count, max; bool assassinActive; };

template <class T>
struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() { return m_pFirst; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CV_AssassinAttacksScript {
    bool Activated;
    RegionData RegionInfo[5];
    CRetailVector<long> RegionKillCounts;
    bool IsActiveThreadTerminating() { ++g_calls; return g_termAfter >= 0 && g_calls > g_termAfter; }
    void InitialiseRegionSpecificInfo();
};

void CV_AssassinAttacksScript::InitialiseRegionSpecificInfo()
{
    for (long i = 0; i < 5; ++i) {
        if (Activated) {
            if (IsActiveThreadTerminating())
                return;
            RegionInfo[i].count = RegionKillCounts[i];
        } else {
            if (IsActiveThreadTerminating())
                return;
            RegionKillCounts[i] = 0;
        }
    }
}

int main()
{
    long kills[5] = { 3, 1, 4, 1, 5 };
    CV_AssassinAttacksScript s;
    s.RegionKillCounts.m_pFirst = kills; s.RegionKillCounts.m_pLast = s.RegionKillCounts.m_pEnd = kills + 5;
    for (int i = 0; i < 5; ++i) s.RegionInfo[i].count = -1;
    s.Activated = true;
    s.InitialiseRegionSpecificInfo();
    for (int i = 0; i < 5; ++i) if (s.RegionInfo[i].count != kills[i]) { std::printf("00e37cf0_TEST FAIL\n"); return 1; }
    s.Activated = false; g_calls = 0; g_termAfter = 2;
    s.InitialiseRegionSpecificInfo();
    if (kills[0] != 0 || kills[1] != 0 || kills[2] != 4 || g_calls != 3) { std::printf("00e37cf0_TEST FAIL\n"); return 1; }
    std::printf("00e37cf0_TEST PASS\n");
    return 0;
}