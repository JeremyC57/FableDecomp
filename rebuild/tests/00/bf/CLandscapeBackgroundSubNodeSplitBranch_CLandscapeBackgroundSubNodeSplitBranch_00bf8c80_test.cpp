#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_calls = 0, g_args[3]; static void* g_nodes[2]; struct CMovableResource { virtual void v00() { } }; struct CTreeNode { unsigned char _body[0xe8]; CTreeNode(long a, long b, long c) { g_nodes[g_calls++] = this; g_args[0] = a; g_args[1] = b; g_args[2] = c; } };
struct CBranch : CMovableResource { CTreeNode m_nodeA; CTreeNode m_nodeB; virtual void v00() { } CBranch(long a, long b, long c) : m_nodeA(a, b, c), m_nodeB(a, b, c) { } };

int main() {
    CBranch b(1, 2, 3); if (g_calls != 2 || g_nodes[0] != &b.m_nodeA || g_nodes[1] != &b.m_nodeB || g_args[0] != 1 || g_args[2] != 3) { std::printf("00bf8c80_TEST FAIL\n"); return 1; }
    std::printf("00bf8c80_TEST PASS\n"); return 0;
}