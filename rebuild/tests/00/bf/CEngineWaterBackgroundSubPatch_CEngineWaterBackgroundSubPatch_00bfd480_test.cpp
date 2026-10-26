// Behaviour test for 0x00BFD480 ~CEngineWaterBackgroundSubPatch: owned buffers returned (index -> edge list, vertex -> free).
#include <cstdio>

static void* g_edge[16]; static int g_ne = 0; static void* g_free[16]; static int g_nf = 0;
struct CMemory {
    void AppendFreeEdgeNode(void* b) { g_edge[g_ne++] = b; }
    void FreeBlock(void* b) { g_free[g_nf++] = b; }
};
struct SEngine { CMemory* m_pLandscapeMemory; };
static SEngine g_engine; static SEngine* g_pEngine = &g_engine;

class CMovableResource { public: virtual void v00() {} };
class CEngineWaterBackgroundSubPatch : public CMovableResource { public:
    void* IndexBuffer; void* VertexBuffer; void* BridgeIndexBuffers[4]; void* BridgeVertexBuffers[4]; bool DeletableBuffers;
    ~CEngineWaterBackgroundSubPatch()
    {
        if (DeletableBuffers) {
            if (IndexBuffer) g_pEngine->m_pLandscapeMemory->AppendFreeEdgeNode(IndexBuffer);
            if (VertexBuffer) g_pEngine->m_pLandscapeMemory->FreeBlock(VertexBuffer);
            for (long i = 0; i < 4; ++i) {
                if (BridgeIndexBuffers[i]) g_pEngine->m_pLandscapeMemory->AppendFreeEdgeNode(BridgeIndexBuffers[i]);
                if (BridgeVertexBuffers[i]) g_pEngine->m_pLandscapeMemory->FreeBlock(BridgeVertexBuffers[i]);
            }
        }
    }
};

int main()
{
    CMemory mem; g_engine.m_pLandscapeMemory = &mem;
    int b[10];
    {
        CEngineWaterBackgroundSubPatch p;
        p.DeletableBuffers = false; p.IndexBuffer = &b[0]; p.VertexBuffer = &b[1];
        for (int i = 0; i < 4; ++i) { p.BridgeIndexBuffers[i] = &b[2 + i]; p.BridgeVertexBuffers[i] = &b[6 + i]; }
    }
    if (g_ne || g_nf) { std::printf("00bfd480_TEST FAIL\n"); return 1; }
    {
        CEngineWaterBackgroundSubPatch p;
        p.DeletableBuffers = true; p.IndexBuffer = &b[0]; p.VertexBuffer = 0;
        for (int i = 0; i < 4; ++i) { p.BridgeIndexBuffers[i] = &b[2 + i]; p.BridgeVertexBuffers[i] = (i == 2) ? 0 : &b[6 + i]; }
    }
    if (g_ne != 5 || g_nf != 3 || g_edge[0] != &b[0] || g_edge[4] != &b[5] || g_free[0] != &b[6] || g_free[2] != &b[9]) { std::printf("00bfd480_TEST FAIL\n"); return 1; }
    std::printf("00bfd480_TEST PASS\n");
    return 0;
}