// Behaviour test for 0x00a0d780 (CInputProcessManager::RemoveInputProcess): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CInputProcessBase;
struct SProcessNode { CInputProcessBase* m_pOwner; SProcessNode* m_pNext; SProcessNode* m_pPrev; };
struct CInputProcessBase { int id; SProcessNode m_node; };
struct SProcessCursor { SProcessNode* m_pForward; SProcessNode* m_pBackward; SProcessCursor* m_pNextCursor; };
struct CInputProcessManager {
    SProcessNode* m_pFirst; SProcessNode* m_pLast; long m_count; SProcessCursor* m_pCursors;
    void AddInputProcess(CInputProcessBase* p)
    {
        SProcessNode* node = &p->m_node; node->m_pNext = 0; node->m_pPrev = m_pLast; node->m_pOwner = p;
        if (m_pLast) m_pLast->m_pNext = node; m_pLast = node; if (!m_pFirst) m_pFirst = node; ++m_count;
    }
    void RemoveInputProcess(CInputProcessBase* process)
    {
        SProcessNode* node = &process->m_node;
        for (SProcessCursor* c = m_pCursors; c; c = c->m_pNextCursor) {
            if (c->m_pForward == node && c->m_pForward) c->m_pForward = node->m_pNext;
            if (c->m_pBackward == node && c->m_pBackward) c->m_pBackward = node->m_pPrev;
        }
        if (m_pFirst == node) m_pFirst = node->m_pNext;
        if (m_pLast == node) m_pLast = node->m_pPrev;
        if (node->m_pPrev) node->m_pPrev->m_pNext = node->m_pNext;
        if (node->m_pNext) node->m_pNext->m_pPrev = node->m_pPrev;
        node->m_pNext = 0; node->m_pPrev = 0; node->m_pOwner = 0;
        --m_count;
    }
};
static void Run()
{
    CInputProcessBase a, b, c; a.id = 1; b.id = 2; c.id = 3;
    CInputProcessManager m; m.m_pFirst = m.m_pLast = 0; m.m_count = 0;
    SProcessCursor cur; cur.m_pForward = &b.m_node; cur.m_pBackward = &b.m_node; cur.m_pNextCursor = 0; m.m_pCursors = &cur;
    m.AddInputProcess(&a); m.AddInputProcess(&b); m.AddInputProcess(&c);
    m.RemoveInputProcess(&b);
    CHECK(m.m_count == 2 && m.m_pFirst == &a.m_node && m.m_pLast == &c.m_node);
    CHECK(a.m_node.m_pNext == &c.m_node && c.m_node.m_pPrev == &a.m_node);
    CHECK(cur.m_pForward == &c.m_node && cur.m_pBackward == &a.m_node);
    CHECK(b.m_node.m_pNext == 0 && b.m_node.m_pPrev == 0 && b.m_node.m_pOwner == 0);
    m.RemoveInputProcess(&a); CHECK(m.m_pFirst == &c.m_node && c.m_node.m_pPrev == 0 && cur.m_pBackward == 0);
    m.RemoveInputProcess(&c); CHECK(m.m_pFirst == 0 && m.m_pLast == 0 && m.m_count == 0 && cur.m_pForward == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a0d780_TEST FAIL\n"); return 1; }
    std::printf("00a0d780_TEST PASS\n");
    return 0;
}
