#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SScalarTag { SScalarTag() { } };
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVector_pod() : m_pFirst(0) { } ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };
struct CCountedPointer_00762480 { void* m_pObject; void* m_pCount; };
static int g_range = 0; static void* g_rangeFirst = 0;
static void DestroyRange_00764ee0(CCountedPointer_00762480* f, CCountedPointer_00762480*, const SScalarTag&) { ++g_range; g_rangeFirst = f; }
struct CRetailVector_counted { CCountedPointer_00762480* m_pFirst; CCountedPointer_00762480* m_pLast; CCountedPointer_00762480* m_pEnd; CRetailVector_counted() : m_pFirst(0), m_pLast(0) { }
    ~CRetailVector_counted() { DestroyRange_00764ee0(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); } };
namespace NShape { struct CShape { virtual void v00() { } long m_id; CRetailVector_pod m_points; CRetailVector_pod m_edges; ~CShape() { } };
struct CBasicShape : CShape { CRetailVector_counted Subdivisions; ~CBasicShape() { } }; }
int main() {
    int x1, x2; CCountedPointer_00762480 cp[2]; { NShape::CBasicShape s; s.m_points.m_pFirst = &x1; s.m_edges.m_pFirst = &x2; s.Subdivisions.m_pFirst = cp; s.Subdivisions.m_pLast = cp + 2; }
    if (g_range != 1 || g_rangeFirst != cp || g_freed != 3 || g_freedPtr[0] != cp || g_freedPtr[1] != &x2 || g_freedPtr[2] != &x1) { std::printf("00762480_TEST FAIL\n"); return 1; }
    std::printf("00762480_TEST PASS\n"); return 0;
}