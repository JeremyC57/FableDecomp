#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } }; struct CANav : CBaseNC { virtual void v00() { } float m_x, m_y; bool QuadTreeLevel, NavLayer, LeafNode, Blocked; CANav(float x, float y, bool c, bool d) : CBaseNC(), m_x(x), m_y(y), QuadTreeLevel(c), NavLayer(d), LeafNode(true), Blocked(true) { } };
struct CNode : CANav { virtual void v00() { } CNode(bool c, bool d) : CANav(-1.0f, -1.0f, c, d) { } };

int main() {
    CNode* n = new CNode(true, false); if (g_base != 1 || n->m_x != -1.0f || n->m_y != -1.0f || !n->QuadTreeLevel || n->NavLayer || !n->LeafNode || !n->Blocked) { std::printf("00a768c0_TEST FAIL\n"); return 1; } delete n;
    std::printf("00a768c0_TEST PASS\n"); return 0;
}