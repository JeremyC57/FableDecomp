// Behaviour test for CScopedShadersEnable::~CScopedShadersEnable (0x009D8B40): restores the saved enables.
#include <cstdio>

struct CShaderRenderManager
{
    bool VertexShadersEnabled;
    bool PixelShadersEnabled;
    int  m_calls;
    void EnableVertexShaders()  { VertexShadersEnabled = true;  ++m_calls; }
    void DisableVertexShaders() { VertexShadersEnabled = false; ++m_calls; }
    void EnablePixelShaders()   { PixelShadersEnabled = true;   ++m_calls; }
    void DisablePixelShaders()  { PixelShadersEnabled = false;  ++m_calls; }
    void SetVertexShadersEnable(bool enable, bool* old)
    {
        if (old) *old = VertexShadersEnabled;
        if (enable != VertexShadersEnabled) { if (enable) EnableVertexShaders(); else DisableVertexShaders(); }
    }
    void SetPixelShadersEnable(bool enable, bool* old)
    {
        if (old) *old = PixelShadersEnabled;
        if (enable != PixelShadersEnabled) { if (enable) EnablePixelShaders(); else DisablePixelShaders(); }
    }
};

CShaderRenderManager g_ShaderRenderManager;

class CScopedShadersEnable
{
public:
    CScopedShadersEnable(bool vertexEnable, bool pixelEnable)
    {
        g_ShaderRenderManager.SetVertexShadersEnable(vertexEnable, &OldVertexEnable);
        g_ShaderRenderManager.SetPixelShadersEnable(pixelEnable, &OldPixelEnable);
    }
    ~CScopedShadersEnable()
    {
        g_ShaderRenderManager.SetVertexShadersEnable(OldVertexEnable, 0);
        g_ShaderRenderManager.SetPixelShadersEnable(OldPixelEnable, 0);
    }
    bool OldVertexEnable;
    bool OldPixelEnable;
};

int main()
{
    g_ShaderRenderManager.VertexShadersEnabled = true;
    g_ShaderRenderManager.PixelShadersEnabled = false;
    g_ShaderRenderManager.m_calls = 0;
    {
        CScopedShadersEnable scope(false, false);
        if (g_ShaderRenderManager.VertexShadersEnabled || g_ShaderRenderManager.PixelShadersEnabled) { std::printf("009d8b40_TEST FAIL\n"); return 1; }
        if (g_ShaderRenderManager.m_calls != 1) { std::printf("009d8b40_TEST FAIL\n"); return 1; }
    }
    if (!g_ShaderRenderManager.VertexShadersEnabled || g_ShaderRenderManager.PixelShadersEnabled) { std::printf("009d8b40_TEST FAIL\n"); return 1; }
    if (g_ShaderRenderManager.m_calls != 2) { std::printf("009d8b40_TEST FAIL\n"); return 1; }   // only the changed state is switched back
    std::printf("009d8b40_TEST PASS\n");
    return 0;
}