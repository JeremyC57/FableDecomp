#include <cstdio>
#include <cstdlib>
struct CAnimSet { int x; }; struct CThingCreatureBase { int y; };
static int g_freed = 0; static int g_fill = 0; static CAnimSet g_a, g_b;
static void free_x(void* p) { ++g_freed; std::free(p); }
struct CVec { CAnimSet** m_pFirst; CAnimSet** m_pLast; CAnimSet** m_pEnd; CVec() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } ~CVec() { if (m_pFirst) free_x(m_pFirst); } unsigned long size() const { return m_pLast - m_pFirst; } };
static void GetActionAnimSetList_008f4990(CVec* v, CThingCreatureBase*, long) { if (g_fill) { v->m_pFirst = (CAnimSet**)std::malloc(8); v->m_pFirst[0] = &g_a; v->m_pFirst[1] = &g_b; v->m_pLast = v->m_pFirst + g_fill; v->m_pEnd = v->m_pFirst + 2; } }
static CAnimSet* GetSingle(CThingCreatureBase* c, long a) { CVec list; GetActionAnimSetList_008f4990(&list, c, a); if (list.size() == 1) return list.m_pFirst[0]; return 0; }
int main()
{
    CThingCreatureBase c;
    g_fill = 0; if (GetSingle(&c, 1) != 0 || g_freed != 0) { std::printf("008f4aa0_TEST FAIL\n"); return 1; }
    g_fill = 1; if (GetSingle(&c, 1) != &g_a || g_freed != 1) { std::printf("008f4aa0_TEST FAIL\n"); return 1; }
    g_fill = 2; if (GetSingle(&c, 1) != 0 || g_freed != 2) { std::printf("008f4aa0_TEST FAIL\n"); return 1; }
    std::printf("008f4aa0_TEST PASS\n"); return 0;
}