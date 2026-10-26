#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_agents = 0; struct CAgent { CAgent() { ++g_agents; } ~CAgent() { --g_agents; std::strcat(g_seq, "A"); } }; struct CRel { virtual void Release(long f) { std::strcat(g_seq, "R"); std::strcat(g_seq, f == 1 ? "1" : "?"); } };
class CTCBase { public: virtual ~CTCBase() { std::strcat(g_seq, "B"); } }; class CTCObstacle : public CTCBase { public: CAgent* m_pAgent; CRel* m_pTarget; ~CTCObstacle() { CAgent* a = m_pAgent; if (a) { delete a; if (m_pTarget) m_pTarget->Release(1); m_pAgent = 0; m_pTarget = 0; } } };

int main() {
    CTCObstacle* o = new CTCObstacle; CAgent* ag = new CAgent; CRel r; o->m_pAgent = ag; o->m_pTarget = &r; delete o; if (std::strcmp(g_seq, "AR1B") || g_agents != 0) { std::printf("004d95eb_TEST FAIL\n"); return 1; } g_seq[0] = 0; o = new CTCObstacle; o->m_pAgent = 0; o->m_pTarget = &r; delete o; if (std::strcmp(g_seq, "B")) { std::printf("004d95eb_TEST FAIL\n"); return 1; }
    std::printf("004d95eb_TEST PASS\n"); return 0;
}