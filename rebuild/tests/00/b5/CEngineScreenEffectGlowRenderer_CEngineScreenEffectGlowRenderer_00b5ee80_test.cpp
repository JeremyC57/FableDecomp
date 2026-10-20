// Behaviour test for 0x00B5EE80 CEngineScreenEffectGlowRenderer::~CEngineScreenEffectGlowRenderer (model copy).
#include <cstdio>

static int g_freedPage = -100, g_dataDeleted = 0, g_baseDtor = 0;
struct CData { long m_refCount; ~CData() { ++g_dataDeleted; } };
struct CShader {
    CData* Data;
    CShader() : Data(0) {}
    ~CShader() { CData* d = Data; if (d) { if (--d->m_refCount == 0) delete d; } }
};
struct CComponent { ~CComponent() { ++g_baseDtor; } };
struct CGlow : CComponent {
    long DynamicTexturePageHandle;
    CShader shaders[9];
    ~CGlow() { if (DynamicTexturePageHandle >= 0) g_freedPage = DynamicTexturePageHandle; }
};

int main()
{
    CData* shared = new CData; shared->m_refCount = 2;
    CData* solo = new CData; solo->m_refCount = 1;
    {
        CGlow g; g.DynamicTexturePageHandle = 7;
        g.shaders[0].Data = shared; g.shaders[8].Data = shared; g.shaders[4].Data = solo;
    }
    if (g_freedPage != 7 || g_dataDeleted != 2 || g_baseDtor != 1) { std::printf("00b5ee80_TEST FAIL\n"); return 1; }
    { CGlow h; h.DynamicTexturePageHandle = -1; }
    if (g_freedPage != 7 || g_baseDtor != 2) { std::printf("00b5ee80_TEST FAIL\n"); return 1; }
    std::printf("00b5ee80_TEST PASS\n");
    return 0;
}