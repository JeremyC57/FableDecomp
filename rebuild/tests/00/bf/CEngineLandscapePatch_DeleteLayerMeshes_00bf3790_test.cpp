#include <cstdio>
static int g_meshDeleted = 0, g_freed = 0; static void* g_freedBlock = 0;
struct CLayerMesh { virtual ~CLayerMesh() { ++g_meshDeleted; } };
struct CMM { void FreeVirtual(void* b) { ++g_freed; g_freedBlock = b; } };
struct CEngine { unsigned char _pad[0x6b0]; CMM* m_pLandscapeMemoryManager; };
static CEngine g_engine; static CEngine* g_pEngine = &g_engine;
struct CPatch { unsigned char _pad[8]; CLayerMesh* m_pLayerMesh; void* m_pLayerBlock; unsigned char _pad10[0x2c]; unsigned char m_layerCount : 3; unsigned char m_other : 5;
    void DeleteLayerMeshes() { if (m_pLayerMesh) { delete m_pLayerMesh; m_pLayerMesh = 0; m_layerCount = 0; } if (m_pLayerBlock) { g_pEngine->m_pLandscapeMemoryManager->FreeVirtual(m_pLayerBlock); m_pLayerBlock = 0; } } };
int main()
{
    CMM mm; g_engine.m_pLandscapeMemoryManager = &mm; int block; CPatch p; p.m_pLayerMesh = 0; p.m_pLayerBlock = 0; p.m_layerCount = 5; p.m_other = 9;
    p.DeleteLayerMeshes(); if (g_meshDeleted != 0 || g_freed != 0 || p.m_layerCount != 5) { std::printf("00bf3790_TEST FAIL\n"); return 1; }
    p.m_pLayerMesh = new CLayerMesh; p.m_pLayerBlock = &block; p.DeleteLayerMeshes();
    if (g_meshDeleted != 1 || p.m_pLayerMesh != 0 || p.m_layerCount != 0 || p.m_other != 9 || g_freed != 1 || g_freedBlock != &block || p.m_pLayerBlock != 0) { std::printf("00bf3790_TEST FAIL\n"); return 1; }
    std::printf("00bf3790_TEST PASS\n"); return 0;
}