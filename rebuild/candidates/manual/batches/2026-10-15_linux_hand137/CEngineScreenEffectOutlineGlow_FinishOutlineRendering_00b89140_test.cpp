// Behaviour test for 0x00B89140 CEngineScreenEffectOutlineGlow::FinishOutlineRendering: restore sequence; release only if deactivated.
#include <cstdio>

static int g_log[16]; static int g_n = 0; static long g_released = 0;
struct CEffect;
static CEffect* g_effect = 0;
struct CRenderStateManager { void RestoreCaptureBlock() { g_log[g_n++] = 1; } };
static CRenderStateManager g_rsm; static CRenderStateManager* g_pRenderStateManager = &g_rsm;
struct CRenderTarget { int x; void Uninitialise(); };
struct CDisplayManager {
    void SetRenderTarget(CRenderTarget*) { g_log[g_n++] = 2; }
    void SetViewport(void*) { g_log[g_n++] = 3; }
};
static CDisplayManager g_dm; static CDisplayManager* g_pDisplayManager = &g_dm;
struct CTexturePool { void Release(long t) { g_released = t; g_log[g_n++] = 5; } };
struct CEngineGlobal { CTexturePool m_texturePool; };
static CEngineGlobal g_eng; static CEngineGlobal* g_pEngine = &g_eng;

struct CEffect {
    unsigned char m_active; void* OutlineTexture; long OutlineTextureHandle;
    CRenderTarget OldRenderTarget; long OldViewport_TLX; bool OutlineRenderingCaptureBlock_Open; bool m_dropOnUninit;
    unsigned char IsActive() const { return m_active; }
    void FinishOutlineRendering()
    {
        if (IsActive()) {
            OutlineRenderingCaptureBlock_Open = false;
            g_pRenderStateManager->RestoreCaptureBlock();
            CRenderTarget* target = &OldRenderTarget;
            g_pDisplayManager->SetRenderTarget(target);
            g_pDisplayManager->SetViewport(&OldViewport_TLX);
            target->Uninitialise();
            if (!IsActive()) {
                g_pEngine->m_texturePool.Release(OutlineTextureHandle);
                OutlineTexture = 0;
            }
        }
    }
};
void CRenderTarget::Uninitialise() { g_log[g_n++] = 4; if (g_effect->m_dropOnUninit) g_effect->m_active = 0; }

int main()
{
    int tex;
    CEffect e; g_effect = &e;
    e.m_active = 1; e.OutlineTexture = &tex; e.OutlineTextureHandle = 77; e.OutlineRenderingCaptureBlock_Open = true; e.m_dropOnUninit = false;
    e.FinishOutlineRendering();
    if (g_n != 4 || g_log[0] != 1 || g_log[3] != 4 || e.OutlineRenderingCaptureBlock_Open || e.OutlineTexture != &tex) { std::printf("00b89140_TEST FAIL\n"); return 1; }
    e.m_dropOnUninit = true;
    e.FinishOutlineRendering();
    if (g_n != 9 || g_log[8] != 5 || g_released != 77 || e.OutlineTexture != 0) { std::printf("00b89140_TEST FAIL\n"); return 1; }
    e.FinishOutlineRendering();
    if (g_n != 9) { std::printf("00b89140_TEST FAIL\n"); return 1; }
    std::printf("00b89140_TEST PASS\n");
    return 0;
}
