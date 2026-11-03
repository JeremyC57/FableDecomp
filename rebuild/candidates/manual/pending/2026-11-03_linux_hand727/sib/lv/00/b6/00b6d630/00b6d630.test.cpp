// Behaviour test for 0x00B6D630 CEngineWaterRenderer::FinishUsingDynamicTextures: four releases in retail order, slots cleared.
#include <cstdio>

static void* g_released[8];
static int g_count = 0;

struct CTexturePool { void Release(void* t) { g_released[g_count++] = t; } };
struct CEngineGlobal { CTexturePool m_texturePool; };
static CEngineGlobal g_engine;
static CEngineGlobal* g_pEngine = &g_engine;

class CEngineWaterRenderer { public:
    void* m_pReflectionTexture; long m_reflectionSlot;
    void* m_pRefractionTexture; long m_refractionSlot;
    void* m_pSeaBumpTexture;    long m_seaBumpSlot;
    void* m_pWaterBumpTexture;  long m_waterBumpSlot;
    void FinishUsingDynamicTextures()
    {
        CTexturePool* pool = &g_pEngine->m_texturePool;
        pool->Release(m_pReflectionTexture);
        m_reflectionSlot = 0;
        pool->Release(m_pSeaBumpTexture);
        m_seaBumpSlot = 0;
        pool->Release(m_pRefractionTexture);
        m_refractionSlot = 0;
        pool->Release(m_pWaterBumpTexture);
        m_waterBumpSlot = 0;
    }
};

int main()
{
    int a, b, c, d;
    CEngineWaterRenderer w;
    w.m_pReflectionTexture = &a; w.m_reflectionSlot = 1;
    w.m_pRefractionTexture = &b; w.m_refractionSlot = 2;
    w.m_pSeaBumpTexture = &c;    w.m_seaBumpSlot = 3;
    w.m_pWaterBumpTexture = &d;  w.m_waterBumpSlot = 4;
    w.FinishUsingDynamicTextures();
    if (g_count != 4 || g_released[0] != &a || g_released[1] != &c || g_released[2] != &b || g_released[3] != &d
        || w.m_reflectionSlot || w.m_refractionSlot || w.m_seaBumpSlot || w.m_waterBumpSlot
        || w.m_pReflectionTexture != &a) {
        std::printf("00b6d630_TEST FAIL\n"); return 1;
    }
    std::printf("00b6d630_TEST PASS\n");
    return 0;
}