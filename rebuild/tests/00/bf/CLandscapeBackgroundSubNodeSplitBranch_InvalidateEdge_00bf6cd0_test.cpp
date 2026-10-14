#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_calls = 0, g_args[4]; static void* g_nodes[2]; struct CTreeNode { unsigned char _body[0xe8]; void InvalidateEdge(long a, long b, long c, long d) { g_nodes[g_calls++] = this; g_args[0] = a; g_args[1] = b; g_args[2] = c; g_args[3] = d; } };
struct CBranch { virtual void v00() { } CTreeNode m_nodeA; CTreeNode m_nodeB; void InvalidateEdge(long a, long b, long c, long d) { m_nodeA.InvalidateEdge(a, b, c, d); m_nodeB.InvalidateEdge(a, b, c, d); } };

int main() {
    CBranch b; b.InvalidateEdge(1, 2, 3, 4); if (g_calls != 2 || g_nodes[0] != &b.m_nodeA || g_nodes[1] != &b.m_nodeB || g_args[0] != 1 || g_args[3] != 4) { std::printf("00bf6cd0_TEST FAIL\n"); return 1; }
    std::printf("00bf6cd0_TEST PASS\n"); return 0;
}