#include <cstdio>
#include <cstring>
static int g_live;
class CCharString { public: const char* m_pData; CCharString(const char* t, long) : m_pData(t) { ++g_live; } ~CCharString() { --g_live; } };
class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& n) { return std::strcmp(n.m_pData, "MARKER_DICE_GAME") == 0 ? 17 : -1; } };
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; } };
struct CAIStateGroup_BanditPlayDice { virtual long GetMarkerType() { CCharString markerName("MARKER_DICE_GAME", -1); return CGameDefinitionManager::Get().GetDefGlobalIndexFromName(markerName); } };
int main()
{
    CAIStateGroup_BanditPlayDice g; long r = g.GetMarkerType();
    if (r != 17 || g_live != 0) { std::printf("00928550_TEST FAIL\n"); return 1; }
    std::printf("00928550_TEST PASS\n"); return 0;
}