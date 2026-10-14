#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SScalarTag { SScalarTag() { } };
struct SEntry_00675b10 { unsigned char _body[8]; };
static int g_range = 0; static void* g_rangeFirst = 0;
static void DestroyRange_00675850(SEntry_00675b10* f, SEntry_00675b10*, const SScalarTag&) { ++g_range; g_rangeFirst = f; }
struct CRetailVector_00675b10 { SEntry_00675b10* m_pFirst; SEntry_00675b10* m_pLast; SEntry_00675b10* m_pEnd; CRetailVector_00675b10() : m_pFirst(0), m_pLast(0) { } ~CRetailVector_00675b10() { DestroyRange_00675850(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); } };
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVector_pod() : m_pFirst(0) { } ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };
static int g_member = 0;
struct CMember_006753b0 { unsigned char _body[0xc]; ~CMember_006753b0() { ++g_member; } };
struct CVectorTriple_00675b10 { CRetailVector_00675b10 m_objects; CMember_006753b0 m_member; CRetailVector_pod m_a; unsigned char _pad24[4]; CRetailVector_pod m_b; ~CVectorTriple_00675b10() { } };
int main() {
    SEntry_00675b10 es[2]; int x1, x2; { CVectorTriple_00675b10 t; t.m_objects.m_pFirst = es; t.m_objects.m_pLast = es + 2; t.m_a.m_pFirst = &x1; t.m_b.m_pFirst = &x2; }
    if (g_range != 1 || g_member != 1 || g_freed != 3 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1 || g_freedPtr[2] != es) { std::printf("00675b10_TEST FAIL\n"); return 1; }
    std::printf("00675b10_TEST PASS\n"); return 0;
}
