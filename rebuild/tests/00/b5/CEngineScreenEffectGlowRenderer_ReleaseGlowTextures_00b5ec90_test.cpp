#include <cstdio>
#include <stddef.h>
struct CTexture_Calls { int x; };
static long g_released[8]; static int g_n = 0;
struct CTexturePool_Retail { void Release(long handle) { g_released[g_n++] = handle; } };
struct CEngine_Retail { unsigned char _pad[0x3190]; CTexturePool_Retail TexturePool; };
static CEngine_Retail g_engine; CEngine_Retail* g_pEngine_01436e98 = &g_engine;
struct CEngineScreenEffectGlowRenderer_Retail {
    unsigned char _pad[0x10]; CTexture_Calls* GlowTexture256; CTexture_Calls* GlowTexture64; CTexture_Calls* GlowTexture64Alt; CTexture_Calls* GlowTexture16;
    long GlowTexture256Handle; long GlowTexture64Handle; long GlowTexture64AltHandle; long GlowTexture16Handle;
    void ReleaseGlowTextures()
    {
        CTexturePool_Retail* pool = &g_pEngine_01436e98->TexturePool;
        pool->Release(GlowTexture256Handle); pool->Release(GlowTexture64Handle); pool->Release(GlowTexture64AltHandle); pool->Release(GlowTexture16Handle);
        GlowTexture256 = 0; GlowTexture64 = 0; GlowTexture64Alt = 0; GlowTexture16 = 0;
    }
};
int main()
{
    CTexture_Calls t; CEngineScreenEffectGlowRenderer_Retail r;
    r.GlowTexture256 = &t; r.GlowTexture64 = &t; r.GlowTexture64Alt = &t; r.GlowTexture16 = &t;
    r.GlowTexture256Handle = 11; r.GlowTexture64Handle = 22; r.GlowTexture64AltHandle = 33; r.GlowTexture16Handle = 44;
    r.ReleaseGlowTextures();
    if (g_n != 4 || g_released[0] != 11 || g_released[1] != 22 || g_released[2] != 33 || g_released[3] != 44 || r.GlowTexture256 || r.GlowTexture64 || r.GlowTexture64Alt || r.GlowTexture16
        || offsetof(CEngineScreenEffectGlowRenderer_Retail, GlowTexture16Handle) != 0x2c || offsetof(CEngine_Retail, TexturePool) != 0x3190) { std::printf("00b5ec90_TEST FAIL\n"); return 1; }
    std::printf("00b5ec90_TEST PASS\n"); return 0;
}