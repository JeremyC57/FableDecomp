#include <cstdio>
#include <cstring>
static int g_live;
struct CCharString { const char* m_pData; CCharString(const char* t, long) : m_pData(t) { ++g_live; } ~CCharString() { --g_live; } };
struct CDefinitionManager { long GetDefGlobalIndexFromName(const CCharString& n) { return std::strcmp(n.m_pData, "MARKER_PLOT") == 0 ? 23 : -1; } };
struct CGameDefinitionManager : CDefinitionManager { static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; } };
struct CAIStateGroup_BanditPlot { virtual long GetMarkerType() { CCharString markerName("MARKER_PLOT", -1); return CGameDefinitionManager::Get().GetDefGlobalIndexFromName(markerName); } };
int main()
{
    CAIStateGroup_BanditPlot g; long r = g.GetMarkerType();
    if (r != 23 || g_live != 0) { std::printf("00928590_TEST FAIL\n"); return 1; }
    std::printf("00928590_TEST PASS\n"); return 0;
}
