#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CRenderStateManager; static CRenderStateManager* g_pRSM = (CRenderStateManager*)1; struct CBase { virtual void v00() { } ~CBase() { std::strcat(g_seq, "B"); } }; struct CRenderStateManager : CBase { void* m_4; virtual ~CRenderStateManager() { g_pRSM = 0; m_4 = 0; } };

int main() {
    CRenderStateManager* p = new CRenderStateManager; delete p; if (std::strcmp(g_seq, "B") || g_pRSM != 0) { std::printf("00a07980_TEST FAIL\n"); return 1; }
    std::printf("00a07980_TEST PASS\n"); return 0;
}