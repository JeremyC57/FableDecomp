// Behaviour test for 0x00BFD3B0 CEngineWaterBackgroundSubPatch::OnPostRelocation: rebind every non-null buffer, in retail order.
#include <cstdio>

static void* g_blocks[16]; static void* g_owners[16]; static int g_n = 0;
struct CMemory { void RebindBlockOwner(void* b, void* o) { g_blocks[g_n] = b; g_owners[g_n] = o; ++g_n; } };
struct SEngine { CMemory* m_pLandscapeMemory; };
static SEngine g_engine; static SEngine* g_pEngine = &g_engine;

struct CEngineWaterBackgroundSubPatch {
    void* IndexBuffer; void* VertexBuffer; void* BridgeIndexBuffers[4]; void* BridgeVertexBuffers[4];
    void OnPostRelocation()
    {
        CMemory* memory = g_pEngine->m_pLandscapeMemory;
        for (long i = 0; i < 4; ++i) {
            if (BridgeIndexBuffers[i]) memory->RebindBlockOwner(BridgeIndexBuffers[i], this);
            if (BridgeVertexBuffers[i]) memory->RebindBlockOwner(BridgeVertexBuffers[i], this);
        }
        if (IndexBuffer) memory->RebindBlockOwner(IndexBuffer, this);
        if (VertexBuffer) memory->RebindBlockOwner(VertexBuffer, this);
    }
};

int main()
{
    CMemory mem; g_engine.m_pLandscapeMemory = &mem;
    int b[10];
    CEngineWaterBackgroundSubPatch p;
    p.IndexBuffer = &b[0]; p.VertexBuffer = 0;
    for (int i = 0; i < 4; ++i) { p.BridgeIndexBuffers[i] = (i & 1) ? 0 : &b[1 + i]; p.BridgeVertexBuffers[i] = &b[5 + i]; }
    p.OnPostRelocation();
    void* want[7] = { &b[1], &b[5], &b[6], &b[3], &b[7], &b[8], &b[0] };
    if (g_n != 7) { std::printf("00bfd3b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i)
        if (g_blocks[i] != want[i] || g_owners[i] != &p) { std::printf("00bfd3b0_TEST FAIL\n"); return 1; }
    std::printf("00bfd3b0_TEST PASS\n");
    return 0;
}
