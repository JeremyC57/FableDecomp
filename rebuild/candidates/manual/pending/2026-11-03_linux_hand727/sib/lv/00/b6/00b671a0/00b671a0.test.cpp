// test: CEngineLandscapeRenderer::SetupBackgroundStates -- request the faded-shadow factor (granted only in shadow mode 1, dirty flag only on change), then update global lighting states
#include <stdio.h>
class CEngineLightingManager { public:
    int ShadowMode; bool FadedShadowDiffuseFactorEnabled; unsigned long StateUpdateFlags; int updates;
    int PeekGlobalShadowMode() const { return ShadowMode; }
    void SetEnableFadedShadowFactor(bool enable) {
        if (PeekGlobalShadowMode() != 1) enable = false;
        if (enable != FadedShadowDiffuseFactorEnabled) { FadedShadowDiffuseFactorEnabled = enable; StateUpdateFlags |= 1; }
    }
    void UpdateGlobalLightingStates() { ++updates; }
};
static CEngineLightingManager* g_pLightingManager;
class CEngineLandscapeRenderer { public:
    void SetupBackgroundStates() { g_pLightingManager->SetEnableFadedShadowFactor(true); g_pLightingManager->UpdateGlobalLightingStates(); }
};
int main() {
    CEngineLightingManager m = { 1, false, 0, 0 };
    CEngineLandscapeRenderer r;
    g_pLightingManager = &m;
    r.SetupBackgroundStates();
    if (!m.FadedShadowDiffuseFactorEnabled || m.StateUpdateFlags != 1 || m.updates != 1) { printf("00b671a0_TEST FAIL\n"); return 1; }
    m.StateUpdateFlags = 0;
    r.SetupBackgroundStates();                               // unchanged: no dirty flag
    if (m.StateUpdateFlags != 0 || m.updates != 2) { printf("00b671a0_TEST FAIL\n"); return 1; }
    m.ShadowMode = 2;
    r.SetupBackgroundStates();                               // other shadow mode: forced off, dirty
    if (m.FadedShadowDiffuseFactorEnabled || m.StateUpdateFlags != 1 || m.updates != 3) { printf("00b671a0_TEST FAIL\n"); return 1; }
    printf("00b671a0_TEST PASS\n");
    return 0;
}