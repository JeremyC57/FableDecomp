#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct SRec { long m_0, m_4; void* m_pObject; SCountBlock* m_pCount; SRec() { }
    SRec(const SRec& o) : m_0(o.m_0), m_4(o.m_4), m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; } };

int main()
{
    SCountBlock b; b.m_references = 1; int obj; SRec a; a.m_0 = 3; a.m_4 = 4; a.m_pObject = &obj; a.m_pCount = &b;
    SRec c(a); if (c.m_0 != 3 || c.m_4 != 4 || c.m_pObject != &obj || c.m_pCount != &b || b.m_references != 2) { std::printf("00cb9c20_TEST FAIL\n"); return 1; }
    a.m_pCount = 0; SRec d(a); if (d.m_pCount != 0 || b.m_references != 2) { std::printf("00cb9c20_TEST FAIL\n"); return 1; }
    std::printf("00cb9c20_TEST PASS\n");
    return 0;
}
