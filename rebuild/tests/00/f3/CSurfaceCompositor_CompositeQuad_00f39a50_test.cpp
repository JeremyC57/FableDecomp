#include <cstdio>
struct CTexture { int id; };
static CTexture* g_tex = 0; static unsigned long g_count = 0; static const void* g_verts = 0; static long g_stride = 0, g_vcount = 0, g_tris = 0;
static const void* g_indices = 0; static long g_blend = -1; static int g_order = 0, g_blendAt = 0, g_drawAt = 0;
struct CRenderManagerCore {
    void DrawAsIndexedTris(CTexture** textures, unsigned long count, const void* vertices, long stride, long vcount, const void* indices, long tris)
    { g_tex = textures[0]; g_count = count; g_verts = vertices; g_stride = stride; g_vcount = vcount; g_indices = indices; g_tris = tris; g_drawAt = ++g_order; }
};
struct CDisplayManager { CRenderManagerCore* PRenderManager; };
struct CSystemManager { CDisplayManager* PDisplayManager; };
static CSystemManager g_system;
static CSystemManager* GFGetSystemManager() { return &g_system; }
static const unsigned short g_QuadIndices[6] = { 0, 1, 2, 0, 2, 3 };
struct SCompositeVertex { float x, y, z, rhw, u, v; };
struct CSurfaceCompositor {
    long m_quadsComposited;
    void SetBlendMode(long blend) { g_blend = blend; g_blendAt = ++g_order; }
    void CompositeQuad(CTexture* texture, const SCompositeVertex* vertices, long blend)
    {
        CRenderManagerCore* core = GFGetSystemManager()->PDisplayManager->PRenderManager;
        CTexture* textures[1];
        textures[0] = texture;
        SetBlendMode(blend);
        core->DrawAsIndexedTris(textures, 1, vertices, 24, 4, g_QuadIndices, 2);
        ++m_quadsComposited;
    }
};
int main()
{
    CRenderManagerCore core; CDisplayManager dm; dm.PRenderManager = &core; g_system.PDisplayManager = &dm;
    CTexture t; t.id = 3; SCompositeVertex v[4];
    CSurfaceCompositor c; c.m_quadsComposited = 5;
    c.CompositeQuad(&t, v, 2);
    if (g_tex != &t || g_count != 1 || g_verts != v || g_stride != 24 || g_vcount != 4 || g_indices != g_QuadIndices || g_tris != 2
        || g_blend != 2 || g_blendAt != 1 || g_drawAt != 2 || c.m_quadsComposited != 6) { std::printf("00f39a50_TEST FAIL\n"); return 1; }
    std::printf("00f39a50_TEST PASS\n");
    return 0;
}