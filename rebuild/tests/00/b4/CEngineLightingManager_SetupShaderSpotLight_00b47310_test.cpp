// Behaviour test for CEngineLightingManager::SetupShaderSpotLight @ 0x00B47310.
#include <cstdio>
#include <cmath>
struct CMatrix3x4 { float m[12]; };
struct CRGBFloatColour { float R, G, B, A; };
struct CShadowRendererSceneData {
    float other[48]; CMatrix3x4 SpotLightFalloffMatrix; float rest[36];
};
struct CEngineShadowRenderer { int pad[3]; CShadowRendererSceneData SceneData; };
static CEngineShadowRenderer g_renderer; static CEngineShadowRenderer* g_pShadowRenderer = &g_renderer;
static CMatrix3x4 g_seenMatrix; static const CMatrix3x4* g_seenPtr; static float g_scale, g_offset; static const CRGBFloatColour* g_colour;
struct CShaderRenderManager {
    void SetShadowSpotLightFalloffParameters(const CMatrix3x4* f, float s, float o, const CRGBFloatColour* c)
    { g_seenPtr = f; g_seenMatrix = *f; g_scale = s; g_offset = o; g_colour = c; }
};
static CShaderRenderManager g_shader; static CShaderRenderManager* g_pShader = &g_shader;
struct CEngineLightingManagerSpotlightParams { float Radius, SpotRadius, ConeWidth; CRGBFloatColour Colour; };
struct CEngineLightingManager {
    CEngineLightingManagerSpotlightParams SpotLightParams;
    void SetupShaderSpotLight()
    {
        CShadowRendererSceneData scene(g_pShadowRenderer->SceneData);
        float ratio = SpotLightParams.SpotRadius / SpotLightParams.Radius;
        float scale = 1.0f / (1.0f / (ratio * ratio) - 1.0f);
        g_pShader->SetShadowSpotLightFalloffParameters(&scene.SpotLightFalloffMatrix, scale, -scale, &SpotLightParams.Colour);
    }
};
int main()
{
    for (int i = 0; i < 12; ++i) g_renderer.SceneData.SpotLightFalloffMatrix.m[i] = (float)i;
    CEngineLightingManager m; m.SpotLightParams.Radius = 2.0f; m.SpotLightParams.SpotRadius = 1.0f;
    m.SetupShaderSpotLight();
    // ratio 0.5 -> 1/0.25 = 4 -> 4 - 1 = 3 -> scale 1/3
    int ok = std::fabs(g_scale - 1.0f / 3.0f) < 1e-6f && g_offset == -g_scale && g_colour == &m.SpotLightParams.Colour
          && g_seenPtr != &g_renderer.SceneData.SpotLightFalloffMatrix;   // a copy is passed
    for (int i = 0; i < 12; ++i) ok = ok && g_seenMatrix.m[i] == (float)i;
    if (!ok) { std::printf("00b47310_TEST FAIL\n"); return 1; }
    std::printf("00b47310_TEST PASS\n");
    return 0;
}