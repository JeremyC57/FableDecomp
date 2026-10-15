// Behaviour test for CEngineLandscapeRenderer::SetupCommonStates @ 0x00B67480.
#include <cstdio>
struct CMatrix3x4 {
    float m[12];
    void SetIdentity()
    {
        m[11] = 0.0f; m[10] = 0.0f; m[9] = 0.0f; m[8] = 0.0f; m[7] = 0.0f; m[6] = 0.0f;
        m[5] = 0.0f; m[4] = 0.0f; m[3] = 0.0f; m[2] = 0.0f; m[1] = 0.0f; m[0] = 0.0f;
        m[8] = 1.0f; m[4] = 1.0f; m[0] = 1.0f;
    }
};
static int g_calls[3]; static int g_seq = 0; static CMatrix3x4 g_seen;
struct CEngineLightingManager {
    void UpdateGlobalLightingStates() { g_calls[0] = ++g_seq; }
    void EnableFog() { g_calls[1] = ++g_seq; }
};
static CEngineLightingManager g_light; static CEngineLightingManager* g_pLighting = &g_light;
struct CShaderRenderManager { void SetWorldTransform(const CMatrix3x4* m) { g_calls[2] = ++g_seq; g_seen = *m; } };
static CShaderRenderManager g_shader; static CShaderRenderManager* g_pShader = &g_shader;
struct CEngineLandscapeRenderer {
    void SetupCommonStates()
    {
        g_pLighting->UpdateGlobalLightingStates();
        g_pLighting->EnableFog();
        CMatrix3x4 identity;
        identity.SetIdentity();
        g_pShader->SetWorldTransform(&identity);
    }
};
int main()
{
    CEngineLandscapeRenderer r; r.SetupCommonStates();
    int ok = g_calls[0] == 1 && g_calls[1] == 2 && g_calls[2] == 3;
    for (int i = 0; i < 12; ++i) ok = ok && g_seen.m[i] == ((i == 0 || i == 4 || i == 8) ? 1.0f : 0.0f);
    if (!ok) { std::printf("00b67480_TEST FAIL\n"); return 1; }
    std::printf("00b67480_TEST PASS\n");
    return 0;
}
