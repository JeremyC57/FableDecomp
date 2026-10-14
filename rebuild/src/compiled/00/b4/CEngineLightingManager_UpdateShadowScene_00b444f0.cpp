// CEngineLightingManager::UpdateShadowScene(void* scene) -- hand the scene to the shadow renderer with the current shadowed-spotlight parameters: shadow mode 2 as a shadowed pass (1), mode 1 as a plain pass (0), other modes do nothing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B444F0, length 80 bytes.
// Callees: CEngineShadowRenderer::UpdateShadowScene @ 0x00B4F390 (on the global renderer @ 0x01436E60; arguments: the scene, the pass flag, the parameter block at
// +0x4698 by address). Shape: the mode at +0x4694 is read once; `cmp 2 ; jne` first, then `cmp 1 ; jne`, each arm forming `this+0x4698` with `add ecx` and
// loading the renderer into ecx last; both arms `ret 4`. The PDB layout (Ego) puts ShadowMode at +0x46a0; the TLC object is 12 bytes shorter here, so the
// fields are modelled by address.
struct CEngineShadowRenderer {
    void UpdateShadowScene(void* scene, long shadowed, void* params);   // 0x00B4F390
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;   // 0x01436E60

struct CEngineLightingManager {
    unsigned char _pad[0x4694];
    long          m_shadowMode;                    // +0x4694 (ELightingManagerShadowMode)
    unsigned char m_spotLightParams[0x38];         // +0x4698 (CEngineLightingManagerSpotlightParams)
    void UpdateShadowScene(void* scene);
};

void CEngineLightingManager::UpdateShadowScene(void* scene)
{
    if (m_shadowMode == 2)
        g_pShadowRenderer_01436e60->UpdateShadowScene(scene, 1, m_spotLightParams);
    else if (m_shadowMode == 1)
        g_pShadowRenderer_01436e60->UpdateShadowScene(scene, 0, m_spotLightParams);
}