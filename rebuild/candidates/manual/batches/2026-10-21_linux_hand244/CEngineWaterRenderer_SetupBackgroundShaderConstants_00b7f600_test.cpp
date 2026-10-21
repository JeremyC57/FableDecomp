#include <cstdio>
// behaviour test: CEngineWaterRenderer::SetupBackgroundShaderConstants uploads constant 3, resizes the bone list,
// sets the four background matrices in order and runs PrepareForRender(0x604)
struct CMatrix3x4 { float M[12]; };
static int g_log[16]; static int g_logCount = 0;
static float g_c[4]; static unsigned long g_constIndex = 99, g_resizeN = 0;
static const CMatrix3x4* g_bones[4];
static unsigned long g_prepareFlags = 0;
struct CShaderRenderManager {
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w)
    { g_constIndex = index; g_c[0] = x; g_c[1] = y; g_c[2] = z; g_c[3] = w; g_log[g_logCount++] = 1; }
    void ResizeBones(unsigned long n) { g_resizeN = n; g_log[g_logCount++] = 2; }
    void SetBoneTransform(unsigned long index, const CMatrix3x4* m) { g_bones[index] = m; g_log[g_logCount++] = 3; }
    void PrepareForRender(unsigned long flags) { g_prepareFlags = flags; g_log[g_logCount++] = 4; }
};
static CShaderRenderManager g_manager;
static CShaderRenderManager* g_pShaderRenderManager = &g_manager;
struct CEngineWaterSettings {
    bool EnableBumpMap;
    float BumpMatFactor;
    CMatrix3x4 ShoreLookupMaskMatrix, ShoreLookupMaskMatrix2, ShoreLookupMaskMatrixForBounds, OtherBackgroundShaderVars;
};
struct CEngineWaterRenderer {
    CEngineWaterSettings Settings;
    void SetupBackgroundShaderConstants();
};
void CEngineWaterRenderer::SetupBackgroundShaderConstants()
{
    float factor = 0.0f;
    if (Settings.EnableBumpMap)
        factor = Settings.BumpMatFactor * 0.5f;
    g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, factor, factor, factor);
    g_pShaderRenderManager->ResizeBones(4);
    g_pShaderRenderManager->SetBoneTransform(0, &Settings.OtherBackgroundShaderVars);
    g_pShaderRenderManager->SetBoneTransform(1, &Settings.ShoreLookupMaskMatrix);
    g_pShaderRenderManager->SetBoneTransform(2, &Settings.ShoreLookupMaskMatrixForBounds);
    g_pShaderRenderManager->SetBoneTransform(3, &Settings.ShoreLookupMaskMatrix2);
    g_pShaderRenderManager->PrepareForRender(0x604);
}
int main()
{
    CEngineWaterRenderer r;
    r.Settings.EnableBumpMap = true; r.Settings.BumpMatFactor = 3.0f;
    r.SetupBackgroundShaderConstants();
    bool ok = g_constIndex == 3 && g_c[0] == 1.5f && g_c[3] == 1.5f && g_resizeN == 4
        && g_bones[0] == &r.Settings.OtherBackgroundShaderVars && g_bones[1] == &r.Settings.ShoreLookupMaskMatrix
        && g_bones[2] == &r.Settings.ShoreLookupMaskMatrixForBounds && g_bones[3] == &r.Settings.ShoreLookupMaskMatrix2
        && g_prepareFlags == 0x604 && g_logCount == 7 && g_log[0] == 1 && g_log[1] == 2 && g_log[6] == 4;
    r.Settings.EnableBumpMap = false; g_logCount = 0;
    r.SetupBackgroundShaderConstants();
    ok = ok && g_c[1] == 0.0f;
    if (!ok) { std::printf("00b7f600_TEST FAIL\n"); return 1; }
    std::printf("00b7f600_TEST PASS\n");
    return 0;
}
