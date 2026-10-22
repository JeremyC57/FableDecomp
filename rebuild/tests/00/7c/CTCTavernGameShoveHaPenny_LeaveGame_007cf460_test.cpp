#include <cstdio>
// Behaviour test for CTCTavernGameShoveHaPenny::vfunc_30 (0x007CF460).
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
struct CTCTavernGameCoinBase {
    unsigned char _pad[0x108];
    CWorld* GetWorld() { return &g_world; }
    void LeaveGame() { ++g_base; }
};
struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    void vfunc_30();
};
void CTCTavernGameShoveHaPenny::vfunc_30()
{
    CTCTavernGameCoinBase::LeaveGame();
    CGameCameraManager* camera;
    GetWorld()->GetCameraSource()->GetCameraManager(&camera);
    camera->ReleaseScriptedControl();
    camera->ResetCamera();
}
int main()
{
    static CTCTavernGameShoveHaPenny g;
    CCameraManagerSource src;
    g_world.m_pCameraSource = &src;
    g.vfunc_30();
    if (g_base != 1 || g_release != 1 || g_reset != 1) { std::printf("007cf460_TEST FAIL\n"); return 1; }
    std::printf("007cf460_TEST PASS\n");
    return 0;
}