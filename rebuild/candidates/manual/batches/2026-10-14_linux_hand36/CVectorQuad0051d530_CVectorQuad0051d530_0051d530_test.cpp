#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SScalarTag { SScalarTag() { } };
struct SEntry_0051d530 { unsigned char _body[8]; };
static int g_range = 0; static void* g_rangeFirst = 0;
static void DestroyRange_0051cd80(SEntry_0051d530* f, SEntry_0051d530*, const SScalarTag&) { ++g_range; g_rangeFirst = f; }
struct CRetailVector_0051d530 { SEntry_0051d530* m_pFirst; SEntry_0051d530* m_pLast; SEntry_0051d530* m_pEnd; CRetailVector_0051d530() : m_pFirst(0), m_pLast(0) { } ~CRetailVector_0051d530() { DestroyRange_0051cd80(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); } };
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVector_pod() : m_pFirst(0) { } ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };
struct CVectorQuad_0051d530 { CRetailVector_0051d530 m_objects; CRetailVector_pod m_a; CRetailVector_pod m_b; CRetailVector_pod m_c; ~CVectorQuad_0051d530() { } };
int main() {
    SEntry_0051d530 es[2]; int x1, x2, x3; { CVectorQuad_0051d530 q; q.m_objects.m_pFirst = es; q.m_objects.m_pLast = es + 2; q.m_a.m_pFirst = &x1; q.m_b.m_pFirst = &x2; q.m_c.m_pFirst = &x3; }
    if (g_range != 1 || g_rangeFirst != es || g_freed != 4 || g_freedPtr[0] != &x3 || g_freedPtr[1] != &x2 || g_freedPtr[2] != &x1 || g_freedPtr[3] != es) { std::printf("0051d530_TEST FAIL\n"); return 1; }
    std::printf("0051d530_TEST PASS\n"); return 0;
}
