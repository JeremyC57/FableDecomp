#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SScalarTag { SScalarTag() { } };
struct SEntryA_00a851e0 { unsigned char _body[8]; }; struct SEntryB_00a851e0 { unsigned char _body[8]; };
static int g_rangeA = 0, g_rangeB = 0;
static void DestroyRangeA_00a83420(SEntryA_00a851e0*, SEntryA_00a851e0*, const SScalarTag&) { ++g_rangeA; }
static void DestroyRangeB_00a827d0(SEntryB_00a851e0*, SEntryB_00a851e0*, const SScalarTag&) { ++g_rangeB; }
struct CRetailVectorA_00a851e0 { SEntryA_00a851e0* m_pFirst; SEntryA_00a851e0* m_pLast; SEntryA_00a851e0* m_pEnd; CRetailVectorA_00a851e0() : m_pFirst(0) { } ~CRetailVectorA_00a851e0() { DestroyRangeA_00a83420(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); } };
struct CRetailVectorB_00a851e0 { SEntryB_00a851e0* m_pFirst; SEntryB_00a851e0* m_pLast; SEntryB_00a851e0* m_pEnd; CRetailVectorB_00a851e0() : m_pFirst(0) { } ~CRetailVectorB_00a851e0() { DestroyRangeB_00a827d0(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); } };
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVector_pod() : m_pFirst(0) { } ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };
struct CVectorTriple_00a851e0 { CRetailVector_pod m_pods; CRetailVectorB_00a851e0 m_b; CRetailVectorA_00a851e0 m_a; ~CVectorTriple_00a851e0() { } };
int main() {
    SEntryA_00a851e0 ea[1]; SEntryB_00a851e0 eb[1]; int x; { CVectorTriple_00a851e0 t; t.m_pods.m_pFirst = &x; t.m_b.m_pFirst = eb; t.m_b.m_pLast = eb + 1; t.m_a.m_pFirst = ea; t.m_a.m_pLast = ea + 1; }
    if (g_rangeA != 1 || g_rangeB != 1 || g_freed != 3 || g_freedPtr[0] != ea || g_freedPtr[1] != eb || g_freedPtr[2] != &x) { std::printf("00a851e0_TEST FAIL\n"); return 1; }
    std::printf("00a851e0_TEST PASS\n"); return 0;
}