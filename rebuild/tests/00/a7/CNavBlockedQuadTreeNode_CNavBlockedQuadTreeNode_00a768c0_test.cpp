#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } }; struct CANav : CBaseNC { virtual void v00() { } float m_x, m_y; bool m_c, m_d, m_e, m_f; CANav(float x, float y, bool c, bool d) : CBaseNC(), m_x(x), m_y(y), m_c(c), m_d(d), m_e(true), m_f(true) { } };
struct CNode : CANav { virtual void v00() { } CNode(bool c, bool d) : CANav(-1.0f, -1.0f, c, d) { } };

int main() {
    CNode* n = new CNode(true, false); if (g_base != 1 || n->m_x != -1.0f || n->m_y != -1.0f || !n->m_c || n->m_d || !n->m_e || !n->m_f) { std::printf("00a768c0_TEST FAIL\n"); return 1; } delete n;
    std::printf("00a768c0_TEST PASS\n"); return 0;
}