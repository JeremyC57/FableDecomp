#include <cstdio>
#include <cstring>
static int g_ctor = 0, g_dtor = 0; static char g_quest[64], g_region[64];
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { ++g_ctor; }
    ~CCharString() { ++g_dtor; PStringData = 0; }
};
struct CGameScriptInterface_Calls {
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region) { std::strcpy(g_quest, quest.PStringData); std::strcpy(g_region, region.PStringData); }
};
struct CChestHidingAssassin {
    unsigned char _pad_0x00[0x40]; CGameScriptInterface_Calls* m_pInterface; unsigned char _pad_0x44[4]; bool m_opened; bool m_assassinOut; bool m_screamed;
    void Init() { m_opened = false; m_assassinOut = false; m_screamed = false; m_pInterface->AddQuestRegion("Q_HeroSoulsThunder", "KnotholeGlade"); }
};
int main()
{
    CGameScriptInterface_Calls iface; CChestHidingAssassin a; a.m_pInterface = &iface; a.m_opened = a.m_assassinOut = a.m_screamed = true;
    a.Init();
    if (a.m_opened || a.m_assassinOut || a.m_screamed || g_ctor != 2 || g_dtor != 2 || std::strcmp(g_quest, "Q_HeroSoulsThunder") != 0 || std::strcmp(g_region, "KnotholeGlade") != 0) { std::printf("00d7ad10_TEST FAIL\n"); return 1; }
    std::printf("00d7ad10_TEST PASS\n"); return 0;
}
