#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_freed = 0; struct CLandscapeMemory { void FreeVirtual(void* p) { std::strcat(g_seq, "F"); g_freed = p; } }; struct SEngine { unsigned char _pad[0x6b0]; CLandscapeMemory* m_pMem; }; static CLandscapeMemory g_mem; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
struct CRequestOwner { virtual ~CRequestOwner() { std::strcat(g_seq, "O"); } }; static void* g_released = 0; struct CBlockManager { void ReleaseRequest(void* r) { std::strcat(g_seq, "R"); g_released = r; } };
struct CLoadTask { CRequestOwner* m_pOwner; CBlockManager* m_pManager; void* m_pRequest; unsigned char _pad[2]; bool m_requested; unsigned char _pad2;
    ~CLoadTask() { if (m_pOwner) delete m_pOwner; if (m_requested) m_pManager->ReleaseRequest(m_pRequest); } static void operator delete(void* p) { g_pEngine->m_pMem->FreeVirtual(p); } static void* operator new(size_t n) { return std::malloc(n); } };
struct CPatch { virtual ~CPatch() { std::strcat(g_seq, "P"); } }; struct CMovableResource { virtual void v00() { } ~CMovableResource() { std::strcat(g_seq, "M"); } };
struct CTreeNode : CMovableResource { unsigned char _pad04[0x94]; CLoadTask* m_pLoadTask; long m_9c; CPatch* m_pPatch; virtual void v00() { } ~CTreeNode() { delete m_pPatch; delete m_pLoadTask; } };

int main() {
    g_engine.m_pMem = &g_mem; CBlockManager bm; int req; CLoadTask* t = new CLoadTask; t->m_pOwner = 0; t->m_pManager = &bm; t->m_pRequest = &req; t->m_requested = true; { CTreeNode n; n.m_pLoadTask = t; n.m_pPatch = new CPatch; } if (std::strcmp(g_seq, "PRFM") || g_freed != t) { std::printf("00bf73a0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; { CTreeNode n; n.m_pLoadTask = 0; n.m_pPatch = 0; } if (std::strcmp(g_seq, "M")) { std::printf("00bf73a0_TEST FAIL\n"); return 1; }
    std::printf("00bf73a0_TEST PASS\n"); return 0;
}
