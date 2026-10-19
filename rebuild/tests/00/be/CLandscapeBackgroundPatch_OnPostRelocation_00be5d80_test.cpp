#include <cstdio>
struct CMovableResource { virtual ~CMovableResource() {} };
static void* g_blocks[32]; static void* g_handlers[32]; static int g_n = 0; static int g_strips = 0; static long g_zs = -100;
struct CEngineLandscapeMemoryManager {
    void SetMovableResourceHandler(void* block, CMovableResource* handler) { g_blocks[g_n] = block; g_handlers[g_n++] = handler; }
};
struct CLandscapeBackgroundPatch;
struct CEngineLandscapeRenderer {
    CEngineLandscapeMemoryManager* MemoryManager;
    void OnRegistedZSortingPatchRelocation(long handle, CLandscapeBackgroundPatch*) { g_zs = handle; }
};
static CEngineLandscapeMemoryManager g_mm; static CEngineLandscapeRenderer g_r = { &g_mm };
static CEngineLandscapeRenderer* g_pLandscapeRenderer = &g_r;
struct CPatchTesselationEdgeStrip { int id; void SetMovableResourceHandler(CMovableResource*) { ++g_strips; } };
struct CLandscapeBackgroundPatch : CMovableResource {
    CPatchTesselationEdgeStrip EdgeStrips[4];
    void* Texture; void* IndexBuffer; void* VertexBuffer; void* BridgeIndexBuffers[4]; void* BridgeVertexBuffers[4];
    void* WaterSubPatch; long ZSorterHandle; bool SharedIndexBuffer;
    void OnPostRelocation();
};
void CLandscapeBackgroundPatch::OnPostRelocation()
{
    CEngineLandscapeMemoryManager* memory = g_pLandscapeRenderer->MemoryManager;
    if (WaterSubPatch)
        memory->SetMovableResourceHandler(WaterSubPatch, this);
    for (long i = 0; i < 4; ++i) {
        if (BridgeIndexBuffers[i])
            memory->SetMovableResourceHandler(BridgeIndexBuffers[i], this);
        if (BridgeVertexBuffers[i])
            memory->SetMovableResourceHandler(BridgeVertexBuffers[i], this);
        EdgeStrips[i].SetMovableResourceHandler(this);
    }
    if (Texture)
        memory->SetMovableResourceHandler(Texture, this);
    if (IndexBuffer && !SharedIndexBuffer)
        memory->SetMovableResourceHandler(IndexBuffer, this);
    if (VertexBuffer)
        memory->SetMovableResourceHandler(VertexBuffer, this);
    if (ZSorterHandle >= 0)
        g_pLandscapeRenderer->OnRegistedZSortingPatchRelocation(ZSorterHandle, this);
}
static int fail() { std::printf("00be5d80_TEST FAIL\n"); return 1; }
int main()
{
    int r[16];
    CLandscapeBackgroundPatch p;
    p.WaterSubPatch = &r[0]; p.Texture = &r[1]; p.IndexBuffer = &r[2]; p.VertexBuffer = 0; p.SharedIndexBuffer = true; p.ZSorterHandle = -1;
    for (int i = 0; i < 4; ++i) { p.BridgeIndexBuffers[i] = (i & 1) ? &r[4 + i] : 0; p.BridgeVertexBuffers[i] = &r[8 + i]; }
    p.OnPostRelocation();
    // water, idx1, vtx0..3 (6 so far), texture; index skipped (shared), vertex null
    if (g_n != 8 || g_strips != 4 || g_zs != -100) return fail();
    if (g_blocks[0] != &r[0] || g_blocks[1] != &r[8] || g_blocks[2] != &r[5] || g_blocks[3] != &r[9] || g_blocks[7] != &r[1]) return fail();
    for (int i = 0; i < g_n; ++i) if (g_handlers[i] != &p) return fail();
    g_n = 0; g_strips = 0; p.SharedIndexBuffer = false; p.VertexBuffer = &r[3]; p.ZSorterHandle = 7; p.WaterSubPatch = 0;
    p.OnPostRelocation();
    if (g_n != 9 || g_blocks[7] != &r[2] || g_blocks[8] != &r[3] || g_zs != 7) return fail();
    std::printf("00be5d80_TEST PASS\n");
    return 0;
}