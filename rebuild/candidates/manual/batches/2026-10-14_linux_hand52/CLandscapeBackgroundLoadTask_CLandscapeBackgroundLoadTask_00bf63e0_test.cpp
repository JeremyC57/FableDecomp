#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
struct CRequestOwner { virtual ~CRequestOwner() { std::strcat(g_seq, "O"); } }; static void* g_released = 0; struct CBlockManager { void ReleaseRequest(void* r) { std::strcat(g_seq, "R"); g_released = r; } };
struct CLoadTask { CRequestOwner* m_pOwner; CBlockManager* m_pManager; void* m_pRequest; unsigned char _pad[2]; bool m_requested; unsigned char _pad2;
    ~CLoadTask() { if (m_pOwner) delete m_pOwner; if (m_requested) m_pManager->ReleaseRequest(m_pRequest); }
    void ReleaseFileBlockLoad() { if (m_requested) { m_pManager->ReleaseRequest(m_pRequest); m_requested = false; } } };

int main()
{
    CBlockManager bm; int req; CLoadTask* t = new CLoadTask; t->m_pOwner = new CRequestOwner; t->m_pManager = &bm; t->m_pRequest = &req; t->m_requested = true; delete t;
    if (std::strcmp(g_seq, "OR") || g_released != &req) { std::printf("00bf63e0_TEST FAIL\n"); return 1; } g_seq[0] = 0; t = new CLoadTask; t->m_pOwner = 0; t->m_requested = false; delete t; if (std::strcmp(g_seq, "")) { std::printf("00bf63e0_TEST FAIL\n"); return 1; }
    std::printf("00bf63e0_TEST PASS\n");
    return 0;
}
