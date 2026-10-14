#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CTCBase : CBase { void* m_pOwner; bool m_active; bool m_dirty; CTCBase(void* o) : CBase(), m_pOwner(o), m_active(false), m_dirty(false) { } };
int main() {
    int owner; CTCBase b(&owner); if (b.m_pOwner != &owner || b.m_active || b.m_dirty || g_base != 1) { std::printf("00686800_TEST FAIL\n"); return 1; }
    std::printf("00686800_TEST PASS\n"); return 0;
}
