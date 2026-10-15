#include <cstdio>
#include <cstring>
struct CCharString {
    char m_text[32];
    bool operator!=(const CCharString& other) const { return std::strcmp(m_text, other.m_text) != 0; }
    CCharString& operator=(const CCharString& other) { std::strcpy(m_text, other.m_text); return *this; }
};
static int g_refreshes = 0;
static long g_lastFlag = -1;
struct CPlayerGui { void Refresh(long flag) { ++g_refreshes; g_lastFlag = flag; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
struct CTCQuestCard {
    CCharString QuestName, ObjectiveName, RegionName;
    void SetRegionName(const CCharString& name)
    {
        if (name != RegionName) {
            RegionName = name;
            g_pPlayerGui->Refresh(0);
        }
    }
};
int main()
{
    CTCQuestCard q;
    std::strcpy(q.RegionName.m_text, "Oakvale");
    CCharString same; std::strcpy(same.m_text, "Oakvale");
    q.SetRegionName(same);
    if (g_refreshes != 0) { std::printf("00702560_TEST FAIL\n"); return 1; }
    CCharString other; std::strcpy(other.m_text, "Bowerstone");
    q.SetRegionName(other);
    if (g_refreshes != 1 || g_lastFlag != 0 || std::strcmp(q.RegionName.m_text, "Bowerstone") != 0) { std::printf("00702560_TEST FAIL\n"); return 1; }
    std::printf("00702560_TEST PASS\n");
    return 0;
}