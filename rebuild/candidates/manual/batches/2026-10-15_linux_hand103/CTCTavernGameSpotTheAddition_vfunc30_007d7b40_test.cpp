#include <cstdio>
// Behaviour test for CTCTavernGameSpotTheAddition::vfunc_30 (0x007D7B40).
static int g_release = 0, g_reset = 0, g_base = 0;
struct CGameCameraManager {
    void ReleaseScriptedControl() { ++g_release; }
    void ResetCamera() { ++g_reset; }
};
static CGameCameraManager g_camera;
struct CCameraManagerSource {
    virtual bool GetCameraManager(CGameCameraManager** camera) { *camera = &g_camera; return true; }
    virtual ~CCameraManagerSource() {}
};
struct CWorld {
    CCameraManagerSource* m_pCameraSource;
    CCameraManagerSource* GetCameraSource() { return m_pCameraSource; }
};
static CWorld g_world;
struct CTCTavernGame {
    unsigned char _pad[0x108];
    CWorld* GetWorld() { return &g_world; }
    void LeaveGame() { ++g_base; }
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    void vfunc_30();
};
void CTCTavernGameSpotTheAddition::vfunc_30()
{
    CTCTavernGame::LeaveGame();
    CGameCameraManager* camera;
    GetWorld()->GetCameraSource()->GetCameraManager(&camera);
    camera->ReleaseScriptedControl();
    camera->ResetCamera();
}
int main()
{
    static CTCTavernGameSpotTheAddition g;
    CCameraManagerSource src;
    g_world.m_pCameraSource = &src;
    g.vfunc_30();
    if (g_base != 1 || g_release != 1 || g_reset != 1) { std::printf("007d7b40_TEST FAIL\n"); return 1; }
    std::printf("007d7b40_TEST PASS\n");
    return 0;
}
