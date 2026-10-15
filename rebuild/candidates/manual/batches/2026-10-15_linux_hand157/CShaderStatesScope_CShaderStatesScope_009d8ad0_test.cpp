// Behaviour test for 0x009D8AD0 CShaderStatesScope constructor: saved states and the enable/disable calls.
#include <cstdio>
struct Mgr {
    bool VertexShadersEnabled, PixelShadersEnabled; int calls;
    void EnableVertexShaders() { VertexShadersEnabled = true; ++calls; }
    void DisableVertexShaders() { VertexShadersEnabled = false; ++calls; }
    void EnablePixelShaders() { PixelShadersEnabled = true; ++calls; }
    void DisablePixelShaders() { PixelShadersEnabled = false; ++calls; }
    void SetVertexShadersEnabled(bool e, bool* old) { if (old) *old = VertexShadersEnabled; if (e != VertexShadersEnabled) { if (e) EnableVertexShaders(); else DisableVertexShaders(); } }
    void SetPixelShadersEnabled(bool e, bool* old) { if (old) *old = PixelShadersEnabled; if (e != PixelShadersEnabled) { if (e) EnablePixelShaders(); else DisablePixelShaders(); } }
};
Mgr g_ShaderRenderManager;
struct Scope {
    bool OldVertexShaders, OldPixelShaders;
    Scope(bool vs, bool ps) { g_ShaderRenderManager.SetVertexShadersEnabled(vs, &OldVertexShaders); g_ShaderRenderManager.SetPixelShadersEnabled(ps, &OldPixelShaders); }
};
int main()
{
    g_ShaderRenderManager.VertexShadersEnabled = true; g_ShaderRenderManager.PixelShadersEnabled = false; g_ShaderRenderManager.calls = 0;
    Scope s(false, false);
    if (!s.OldVertexShaders || s.OldPixelShaders || g_ShaderRenderManager.VertexShadersEnabled || g_ShaderRenderManager.calls != 1) { std::printf("009d8ad0_TEST FAIL\n"); return 1; }
    Scope t(true, true);
    if (t.OldVertexShaders || t.OldPixelShaders || !g_ShaderRenderManager.PixelShadersEnabled || g_ShaderRenderManager.calls != 3) { std::printf("009d8ad0_TEST FAIL\n"); return 1; }
    std::printf("009d8ad0_TEST PASS\n");
    return 0;
}
