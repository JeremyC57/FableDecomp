#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVector_pod() : m_pFirst(0) { } ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };
namespace NShape { struct CShape { virtual void v00() { } long m_id; CRetailVector_pod m_points; CRetailVector_pod m_edges; ~CShape() { } }; }
int main() {
    int x1, x2; { NShape::CShape s; s.m_points.m_pFirst = &x1; s.m_edges.m_pFirst = &x2; } if (g_freed != 2 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1) { std::printf("00764f70_TEST FAIL\n"); return 1; }
    std::printf("00764f70_TEST PASS\n"); return 0;
}