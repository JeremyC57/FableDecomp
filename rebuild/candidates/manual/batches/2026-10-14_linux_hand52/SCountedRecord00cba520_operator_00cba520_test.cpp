#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void* g_destroyed = 0; static void* g_deleted = 0;
static void Destroy(void* o) { g_destroyed = o; }
static void FreeBlock(SCountBlock* b) { g_deleted = b; delete b; }
struct SRec { long m_0, m_4; void* m_pObject; SCountBlock* m_pCount;
    SRec& operator=(const SRec& o) { m_0 = o.m_0; m_4 = o.m_4; void* object = o.m_pObject; SCountBlock* count = o.m_pCount;
        if (m_pCount != count) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); FreeBlock(m_pCount); } }
            m_pObject = object; m_pCount = count; if (count) ++count->m_references; } return *this; } };

int main()
{
    SCountBlock* old = new SCountBlock; old->m_references = 1; old->m_pfnDestroy = Destroy; int objOld, objNew; old->m_pObject = &objOld;
    SCountBlock nb; nb.m_references = 1; nb.m_pfnDestroy = Destroy; nb.m_pObject = &objNew;
    SRec a; a.m_0 = 1; a.m_4 = 2; a.m_pObject = &objOld; a.m_pCount = old;
    SRec b; b.m_0 = 7; b.m_4 = 8; b.m_pObject = &objNew; b.m_pCount = &nb;
    SRec& r = (a = b);
    if (&r != &a || a.m_0 != 7 || a.m_4 != 8 || a.m_pObject != &objNew || a.m_pCount != &nb || nb.m_references != 2 || g_destroyed != &objOld || g_deleted != old) { std::printf("00cba520_TEST FAIL\n"); return 1; }
    g_destroyed = 0; a = b; if (g_destroyed != 0 || nb.m_references != 2) { std::printf("00cba520_TEST FAIL\n"); return 1; }
    SRec c; c.m_0 = 0; c.m_4 = 0; c.m_pObject = 0; c.m_pCount = 0; c = b; if (nb.m_references != 3 || c.m_pObject != &objNew) { std::printf("00cba520_TEST FAIL\n"); return 1; }
    std::printf("00cba520_TEST PASS\n");
    return 0;
}
