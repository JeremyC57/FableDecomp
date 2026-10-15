// CEngineLandscapeRenderer::SetupCommonStates() -- refreshes the global lighting and fog states, then loads an identity world transform into the shader render manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B67480, length 140 bytes (144 with padding).
// Callees: CEngineLightingManager::UpdateGlobalLightingStates @ 0x00B46C80 and CEngineLightingManager::EnableFog
// @ 0x00B46890 (both on the lighting manager singleton @ 0x01436E9C, re-read for each call),
// CShaderRenderManager::SetWorldTransform(const CMatrix3x4*) @ 0x009881F0 (on the shader render manager @ 0x01436E14).
// Shape: `this` is never read (`ret`, no arguments). The identity matrix is a 48-byte frame local (`sub esp,30h`)
// filled by an inline SetIdentity that clears all twelve words from the top down and then writes the three 1.0f
// diagonal words (m[8], m[4], m[0]); VC drops the three dead zero stores and keeps the order: nine descending zero
// immediates, then the diagonal, all after the address push. A twelve-float constructor stores interleaved
// ascending, an ascending clear gives ascending stores.
struct CMatrix3x4 {
    float m[12];
    void SetIdentity()
    {
        m[11] = 0.0f; m[10] = 0.0f; m[9] = 0.0f; m[8] = 0.0f; m[7] = 0.0f; m[6] = 0.0f;
        m[5] = 0.0f; m[4] = 0.0f; m[3] = 0.0f; m[2] = 0.0f; m[1] = 0.0f; m[0] = 0.0f;
        m[8] = 1.0f; m[4] = 1.0f; m[0] = 1.0f;
    }
};

struct CEngineLightingManager {
    void UpdateGlobalLightingStates();             // 0x00B46C80
    void EnableFog();                              // 0x00B46890
};
extern CEngineLightingManager* g_pEngineLightingManager_01436e9c;   // 0x01436E9C

struct CShaderRenderManager {
    void SetWorldTransform(const CMatrix3x4* matrix);   // 0x009881F0
};
extern CShaderRenderManager* g_pShaderRenderManager_01436e14;     // 0x01436E14

struct CEngineLandscapeRenderer {
    void SetupCommonStates();
};

void CEngineLandscapeRenderer::SetupCommonStates()
{
    g_pEngineLightingManager_01436e9c->UpdateGlobalLightingStates();
    g_pEngineLightingManager_01436e9c->EnableFog();
    CMatrix3x4 identity;
    identity.SetIdentity();
    g_pShaderRenderManager_01436e14->SetWorldTransform(&identity);
}
