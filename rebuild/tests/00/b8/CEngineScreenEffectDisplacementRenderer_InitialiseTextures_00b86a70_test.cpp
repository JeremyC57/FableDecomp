// Behaviour test for 0x00B86A70 CEngineScreenEffectDisplacementRenderer::InitialiseTextures: free old page, allocate page + 256x256 texture.
#include <cstdio>

static int g_freed = 0; static long g_nextPage = 40; static long g_texResult = 3; static long g_sizeX = 0, g_fmt = 0;
class CPixelFormat { public: long FormatIndex; CPixelFormat(unsigned int f) : FormatIndex((long)f) {} };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) {} };
struct CMgr {
    void FreeMemoryPage(long) { ++g_freed; }
    long AllocateMemoryPage(long, long) { return g_nextPage++; }
    long AllocateTexture(long page, const C2DCoordI* size, long, const CPixelFormat* format, long, long)
    { g_sizeX = size->X; g_fmt = format->FormatIndex; return page >= 0 ? g_texResult : -1; }
};
struct CEngineGlobal { CMgr m_scratchTextures; };
static CEngineGlobal g_e; static CEngineGlobal* g_pEngine = &g_e;
struct CRenderer {
    int* BackBufferCopyTexture; int* DisplacementTexture; long DisplacementTextureHandle; long DynamicTexturePageHandle;
    void FreeTexturePage() { if (DynamicTexturePageHandle >= 0) g_pEngine->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle); }
    bool InitialiseTextures()
    {
        FreeTexturePage();
        CMgr* textures = &g_pEngine->m_scratchTextures;
        DynamicTexturePageHandle = textures->AllocateMemoryPage(0, 0);
        CPixelFormat format(0x15);
        C2DCoordI size(256, 256);
        BackBufferCopyTexture = 0;
        DisplacementTextureHandle = textures->AllocateTexture(DynamicTexturePageHandle, &size, 1, &format, 1, 0);
        DisplacementTexture = 0;
        return DisplacementTextureHandle >= 0;
    }
};

int main()
{
    int t;
    CRenderer r; r.BackBufferCopyTexture = &t; r.DisplacementTexture = &t; r.DisplacementTextureHandle = -1; r.DynamicTexturePageHandle = -1;
    if (!r.InitialiseTextures() || g_freed != 0 || r.DynamicTexturePageHandle != 40 || r.DisplacementTextureHandle != 3
        || r.BackBufferCopyTexture || r.DisplacementTexture || g_sizeX != 256 || g_fmt != 0x15) { std::printf("00b86a70_TEST FAIL\n"); return 1; }
    g_texResult = -5;
    if (r.InitialiseTextures() || g_freed != 1 || r.DynamicTexturePageHandle != 41) { std::printf("00b86a70_TEST FAIL\n"); return 1; }
    std::printf("00b86a70_TEST PASS\n");
    return 0;
}