#include <cstdio>
// Behaviour test for CTCTavernGameCoinGolf::vfunc_30 (0x007D4A90).
static int g_auto = 0, g_reset = 0, g_base = 0, g_fade = 0, g_fadeArg = -1;
struct CGameCameraManager {
    void SetAutomaticCameraControl() { ++g_auto; }
    void ResetCamera() { ++g_reset; }
};
static CGameCameraManager g_camera;
struct CCameraManagerSource {
    bool m_has;
    virtual bool GetCameraManager(CGameCameraManager** camera) { if (m_has) *camera = &g_camera; return m_has; }
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
struct CTCTavernGameCoinGolf : CTCTavernGameCoinBase {
    void FadeOutTableOccupants(bool fadeOut) { ++g_fade; g_fadeArg = fadeOut ? 1 : 0; }
    void vfunc_30();
};
void CTCTavernGameCoinGolf::vfunc_30()
{
    CTCTavernGameCoinBase::LeaveGame();
    FadeOutTableOccupants(true);
    CGameCameraManager* camera = 0;
    if (GetWorld()->GetCameraSource()->GetCameraManager(&camera)) {
        camera->SetAutomaticCameraControl();
        camera->ResetCamera();
    }
}
int main()
{
    static CTCTavernGameCoinGolf g;
    CCameraManagerSource src; src.m_has = false;
    g_world.m_pCameraSource = &src;
    g.vfunc_30();
    if (g_base != 1 || g_fade != 1 || g_fadeArg != 1 || g_auto != 0 || g_reset != 0) { std::printf("007d4a90_TEST FAIL\n"); return 1; }
    src.m_has = true;
    g.vfunc_30();
    if (g_base != 2 || g_auto != 1 || g_reset != 1) { std::printf("007d4a90_TEST FAIL\n"); return 1; }
    std::printf("007d4a90_TEST PASS\n");
    return 0;
}
