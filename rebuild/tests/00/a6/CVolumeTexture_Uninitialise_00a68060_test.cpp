#include <cstdio>
#include <cstring>
static void* g_removed; static float g_weight; static int g_releaseTexture;
struct CRenderStatistics { void RemoveTexture(void* texture, float weight) { g_removed = texture; g_weight = weight; } };
struct CRenderSystem { CRenderStatistics* m_pStatistics; };
class CSystemManager { public: CRenderSystem* m_pRender; };
static CRenderStatistics g_stats; static CRenderSystem g_render; static CSystemManager g_system;
CSystemManager* GFGetSystemManager() { g_render.m_pStatistics = &g_stats; g_system.m_pRender = &g_render; return &g_system; }
struct CFakeTexture { long m_refs; unsigned long AddRef() { return ++m_refs; } unsigned long Release() { return --m_refs; } };
class CVolumeTexture { public:
    CFakeTexture* m_pTexture; long m_format; long m_levels;
    void ReleaseTexture() { ++g_releaseTexture; if (m_pTexture) m_pTexture->Release(); }
    void Uninitialise()
    {
        if (m_pTexture) {
            m_pTexture->AddRef();
            if (m_pTexture->Release() == 1) {
                CSystemManager* system = GFGetSystemManager();
                system->m_pRender->m_pStatistics->RemoveTexture(m_pTexture, 1.0f);
            }
            ReleaseTexture();
            m_levels = 0; m_pTexture = 0; m_format = 0;
        }
    }
};
int main()
{
    int ok = 1;
    CFakeTexture shared; shared.m_refs = 2;
    CVolumeTexture v; v.m_pTexture = &shared; v.m_format = 5; v.m_levels = 3;
    v.Uninitialise();
    if (g_removed != 0 || g_releaseTexture != 1 || v.m_pTexture || v.m_format || v.m_levels || shared.m_refs != 1) ok = 0;
    v.m_pTexture = &shared; v.Uninitialise();
    if (g_removed != &shared || g_weight != 1.0f || g_releaseTexture != 2 || v.m_pTexture) ok = 0;
    v.Uninitialise();
    if (g_releaseTexture != 2) ok = 0;
    if (!ok) { std::printf("00a68060_TEST FAIL\n"); return 1; }
    std::printf("00a68060_TEST PASS\n");
    return 0;
}