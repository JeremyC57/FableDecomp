#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_block = 0; static void* g_owner = 0; struct CMovableResource; struct CLandscapeMemory { void RebindBlockOwner(void* b, CMovableResource* o) { std::strcat(g_seq, "R"); g_block = b; g_owner = o; } void AppendFreeEdgeNode(void* b) { std::strcat(g_seq, "E"); g_block = b; } void FreeBlock(void* b) { std::strcat(g_seq, "F"); g_block = b; } };
struct SEngine { unsigned char _pad[0x6b0]; CLandscapeMemory* m_pMem; }; static CLandscapeMemory g_mem; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
struct CMovableResource { virtual void v00() { } ~CMovableResource() { } };
struct COwned { virtual ~COwned() { std::strcat(g_seq, "D"); } };
struct CLayerMesh : CMovableResource { unsigned char _pad04[0x30]; void* m_pVertexBlock; void* m_pIndexBlock; COwned* m_pOwned; unsigned char _pad40[8]; bool m_shared;
    CLayerMesh() : m_pVertexBlock(0), m_pIndexBlock(0), m_pOwned(0), m_shared(false) { }
    void OnPostRelocation() { if (m_pVertexBlock && !m_shared) g_pEngine->m_pMem->RebindBlockOwner(m_pVertexBlock, this); if (m_pIndexBlock) g_pEngine->m_pMem->RebindBlockOwner(m_pIndexBlock, this); }
    ~CLayerMesh() { if (m_pVertexBlock && !m_shared) g_pEngine->m_pMem->AppendFreeEdgeNode(m_pVertexBlock); if (m_pIndexBlock) { g_pEngine->m_pMem->FreeBlock(m_pIndexBlock); m_pIndexBlock = 0; } if (m_pOwned) delete m_pOwned; } };

int main() {
    g_engine.m_pMem = &g_mem; int v, i; { CLayerMesh m; m.m_pVertexBlock = &v; m.m_pIndexBlock = &i; m.OnPostRelocation(); if (std::strcmp(g_seq, "RR") || g_block != &i || g_owner != &m) { std::printf("00bfdd70_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; m.m_shared = true; m.OnPostRelocation(); if (std::strcmp(g_seq, "R") || g_block != &i) { std::printf("00bfdd70_TEST FAIL\n"); return 1; } g_seq[0] = 0; m.m_pIndexBlock = 0; m.OnPostRelocation(); if (std::strcmp(g_seq, "")) { std::printf("00bfdd70_TEST FAIL\n"); return 1; } }
    std::printf("00bfdd70_TEST PASS\n"); return 0;
}
