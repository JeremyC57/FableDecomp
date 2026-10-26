#include <cstdio>
#include <cstring>
#include <new>

static bool g_hasMode; static long g_modeAsked; static int g_setUp, g_added; static bool g_inMelee;
class CPlayer { public: bool HasMode(long mode) { g_modeAsked = mode; return g_hasMode; } };
class CPlayerManager { public: CPlayer m_player; CPlayer* GetMainPlayer() { return &m_player; } };
class CEngine { public: unsigned char _pad[0x1c]; CPlayerManager* m_pPlayerManager; };
static CEngine* g_pEngine;
class CGameCameraManager { public:
    bool ObservedThingInBuilding;
    void SetUpMainCameraManagerSetDef() { ++g_setUp; }
    bool IsObservedThingInMelee() { return g_inMelee; }
    void AddMeleeCameraMode() { ++g_added; }
    void RemoveBuildingCameraMode() {
        if (!g_pEngine->m_pPlayerManager->GetMainPlayer()->HasMode(0x2f)) {
            SetUpMainCameraManagerSetDef();
            if (IsObservedThingInMelee()) AddMeleeCameraMode();
            ObservedThingInBuilding = false;
        }
    }
};
int main()
{
    CEngine engine; CPlayerManager pm; engine.m_pPlayerManager = &pm; g_pEngine = &engine;
    CGameCameraManager mgr; mgr.ObservedThingInBuilding = true;
    g_hasMode = true; g_inMelee = true; mgr.RemoveBuildingCameraMode();
    if (g_modeAsked != 0x2f || g_setUp != 0 || g_added != 0 || !mgr.ObservedThingInBuilding) { std::printf("0069b610_TEST FAIL\n"); return 1; }
    g_hasMode = false; g_inMelee = false; mgr.RemoveBuildingCameraMode();
    if (g_setUp != 1 || g_added != 0 || mgr.ObservedThingInBuilding) { std::printf("0069b610_TEST FAIL\n"); return 1; }
    mgr.ObservedThingInBuilding = true; g_inMelee = true; mgr.RemoveBuildingCameraMode();
    if (g_setUp != 2 || g_added != 1 || mgr.ObservedThingInBuilding) { std::printf("0069b610_TEST FAIL\n"); return 1; }
    std::printf("0069b610_TEST PASS\n");
    return 0;
}