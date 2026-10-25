// Behaviour test for 00cb13a0: CBaseList::GetI -- object at the position, NULL for NULL
#include <cstdio>

static int g_deleted = 0;
struct CNode { CNode* m_pPrev; CNode* m_pNext; void* m_pObject; };
static void FreeNode(CNode* p) { ++g_deleted; delete p; }
struct CNodeCache {
    int m_iCacheSize; int m_iUsed; CNode* m_pHead;
    void AddToCache(CNode* pNode) { if (m_iUsed < m_iCacheSize) { pNode->m_pNext = m_pHead; m_pHead = pNode; m_iUsed++; } else FreeNode(pNode); }
    void Destroy() { CNode* pNode = m_pHead; while (pNode) { CNode* pCurrent = pNode; pNode = pNode->m_pNext; FreeNode(pCurrent); } }
};
typedef CNode* POSITION;
struct CBaseList {
    CNode* m_pFirst; CNode* m_pLast; long m_Count; CNodeCache m_Cache;
    void* GetNextI(POSITION& rp) const { if (rp == 0) return 0; CNode* pn = rp; rp = pn->m_pNext; return pn->m_pObject; }
    void* GetI(POSITION p) const { if (p == 0) return 0; return p->m_pObject; }
    void* RemoveI(POSITION pos)
    {
        if (pos == 0) return 0;
        CNode* pCurrent = pos;
        if (pCurrent->m_pPrev == 0) m_pFirst = pCurrent->m_pNext; else pCurrent->m_pPrev->m_pNext = pCurrent->m_pNext;
        if (pCurrent->m_pNext == 0) m_pLast = pCurrent->m_pPrev; else pCurrent->m_pNext->m_pPrev = pCurrent->m_pPrev;
        void* pObject = pCurrent->m_pObject;
        m_Cache.AddToCache(pCurrent);
        --m_Count;
        return pObject;
    }
};
static CNode* MakeNode(CNode* prev, void* obj) { CNode* n = new CNode; n->m_pPrev = prev; n->m_pNext = 0; n->m_pObject = obj; if (prev) prev->m_pNext = n; return n; }

int main()
{
    int a; CNode* n1 = MakeNode(0, &a);
    CBaseList l; l.m_pFirst = n1; l.m_pLast = n1; l.m_Count = 1;
    int ok = l.GetI(n1) == &a && l.GetI(0) == 0 && l.m_pFirst == n1;
    std::printf(ok ? "00cb13a0_TEST PASS\n" : "00cb13a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
