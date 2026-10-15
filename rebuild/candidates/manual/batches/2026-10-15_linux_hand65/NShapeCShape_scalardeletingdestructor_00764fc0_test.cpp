#include <cstdio>
#include <cstdlib>
// NShape::CShape scalar deleting destructor: the edge then point vector blocks are freed, then the object is deleted
static long g_frees; static void* g_freed[4];
static void FreeHook(void* p) { g_freed[g_frees++] = p; std::free(p); }
struct CRetailVector_pod {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CRetailVector_pod() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CRetailVector_pod() { if (m_pFirst) FreeHook(m_pFirst); }
};
struct CShape {
    long m_id; CRetailVector_pod m_points; CRetailVector_pod m_edges;
    CShape(long id) : m_id(id) { }
    virtual ~CShape() { }
};
int main()
{
    CShape* s = new CShape(3);
    void* points = std::malloc(24); void* edges = std::malloc(12);
    s->m_points.m_pFirst = points; s->m_edges.m_pFirst = edges;
    CShape* base = s;
    delete base;                                   // slot 0: the scalar deleting destructor with flag 1
    CShape* empty = new CShape(4);
    delete empty;                                  // null vectors: nothing freed
    if (g_frees != 2 || g_freed[0] != edges || g_freed[1] != points) { std::printf("00764fc0_TEST FAIL\n"); return 1; }
    std::printf("00764fc0_TEST PASS\n");
    return 0;
}
