#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0; static void* g_freed[4]; static void FreeBlock(void* p) { if (g_frees < 4) g_freed[g_frees] = p; ++g_frees; std::free(p); }
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) FreeBlock(m_pFirst); } };
struct SPair { CRetailVector_pod m_a; CRetailVector_pod m_b; ~SPair() { } };

int main()
{
    SPair* p = new SPair; void* a = std::malloc(4); void* b = std::malloc(4); p->m_a.m_pFirst = a; p->m_b.m_pFirst = b; delete p;
    if (g_frees != 2 || g_freed[0] != b || g_freed[1] != a) { std::printf("0090ba50_TEST FAIL\n"); return 1; } g_frees = 0; p = new SPair; p->m_a.m_pFirst = 0; p->m_b.m_pFirst = 0; delete p; if (g_frees) { std::printf("0090ba50_TEST FAIL\n"); return 1; }
    std::printf("0090ba50_TEST PASS\n");
    return 0;
}