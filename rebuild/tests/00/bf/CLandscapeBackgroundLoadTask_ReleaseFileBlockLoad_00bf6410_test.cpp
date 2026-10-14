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
    CBlockManager bm; int req; CLoadTask t; t.m_pOwner = 0; t.m_pManager = &bm; t.m_pRequest = &req; t.m_requested = true; t.ReleaseFileBlockLoad();
    if (std::strcmp(g_seq, "R") || g_released != &req || t.m_requested) { std::printf("00bf6410_TEST FAIL\n"); return 1; } t.ReleaseFileBlockLoad(); if (std::strcmp(g_seq, "R")) { std::printf("00bf6410_TEST FAIL\n"); return 1; }
    std::printf("00bf6410_TEST PASS\n");
    return 0;
}