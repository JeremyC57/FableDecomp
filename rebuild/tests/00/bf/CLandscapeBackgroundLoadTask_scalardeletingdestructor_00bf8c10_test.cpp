#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_freed = 0; struct CLandscapeMemory { void FreeVirtual(void* p) { std::strcat(g_seq, "F"); g_freed = p; } }; struct SEngine { unsigned char _pad[0x6b0]; CLandscapeMemory* m_pMem; }; static CLandscapeMemory g_mem; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
struct CRequestOwner { virtual ~CRequestOwner() { std::strcat(g_seq, "O"); } }; static void* g_released = 0; struct CBlockManager { void ReleaseRequest(void* r) { std::strcat(g_seq, "R"); g_released = r; } };
struct CLoadTask { CRequestOwner* m_pOwner; CBlockManager* m_pManager; void* m_pRequest; unsigned char _pad[2]; bool m_requested; unsigned char _pad2;
    ~CLoadTask() { if (m_pOwner) delete m_pOwner; if (m_requested) m_pManager->ReleaseRequest(m_pRequest); } static void operator delete(void* p) { g_pEngine->m_pMem->FreeVirtual(p); } static void* operator new(size_t n) { return std::malloc(n); } };

int main() {
    g_engine.m_pMem = &g_mem; CBlockManager bm; int req; CLoadTask* t = new CLoadTask; t->m_pOwner = new CRequestOwner; t->m_pManager = &bm; t->m_pRequest = &req; t->m_requested = true; delete t;
    if (std::strcmp(g_seq, "ORF") || g_released != &req || g_freed != t) { std::printf("00bf8c10_TEST FAIL\n"); return 1; } g_seq[0] = 0; t = new CLoadTask; t->m_pOwner = 0; t->m_requested = false; delete t; if (std::strcmp(g_seq, "F")) { std::printf("00bf8c10_TEST FAIL\n"); return 1; }
    std::printf("00bf8c10_TEST PASS\n"); return 0;
}