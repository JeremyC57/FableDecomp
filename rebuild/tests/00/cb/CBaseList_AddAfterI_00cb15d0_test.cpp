#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
static int g_head, g_tail;
class CBaseList {
public:
    struct CNode { CNode* m_pPrev; CNode* m_pNext; void* m_pObject; };
    CNode* m_pFirst; CNode* m_pLast; int m_Count; int m_iCacheSize; int m_iUsed; CNode* m_pHead;
    CNode* RemoveFromCache() { CNode* p = m_pHead; if (p) { m_pHead = p->m_pNext; m_iUsed--; } return p; }
    CNode* AddHeadI(void*) { ++g_head; return 0; }
    CNode* AddTailI(void*) { ++g_tail; return 0; }
    CNode* AddAfterI(CNode* pos, void* pObj)
    {
        if (pos == 0) return AddHeadI(pObj);
        CNode* pAfter = pos;
        if (pAfter == m_pLast) return AddTailI(pObj);
        CNode* pNode = RemoveFromCache();
        if (pNode == 0) pNode = new CNode;
        if (pNode == 0) return 0;
        pNode->m_pObject = pObj;
        CNode* pBefore = pAfter->m_pNext;
        pNode->m_pPrev = pAfter;
        pNode->m_pNext = pBefore;
        pBefore->m_pPrev = pNode;
        pAfter->m_pNext = pNode;
        ++m_Count;
        return pNode;
    }
};
int main()
{
    int ok = 1;
    CBaseList::CNode a, b, cached; int x = 1, y = 2;
    a.m_pPrev = 0; a.m_pNext = &b; b.m_pPrev = &a; b.m_pNext = 0; cached.m_pNext = 0;
    CBaseList l; l.m_pFirst = &a; l.m_pLast = &b; l.m_Count = 2; l.m_iCacheSize = 4; l.m_iUsed = 1; l.m_pHead = &cached;
    ok = l.AddAfterI(0, &x) == 0 && g_head == 1;
    ok = ok && l.AddAfterI(&b, &x) == 0 && g_tail == 1;
    CBaseList::CNode* n = l.AddAfterI(&a, &x);
    ok = ok && n == &cached && l.m_iUsed == 0 && l.m_pHead == 0 && a.m_pNext == n && n->m_pPrev == &a && n->m_pNext == &b && b.m_pPrev == n && n->m_pObject == &x && l.m_Count == 3;
    CBaseList::CNode* m = l.AddAfterI(n, &y);
    ok = ok && m != 0 && m != &cached && n->m_pNext == m && m->m_pNext == &b && b.m_pPrev == m && l.m_Count == 4;
    delete m;
    if (!ok) { std::printf("00cb15d0_TEST FAIL\n"); return 1; }
    std::printf("00cb15d0_TEST PASS\n");
    return 0;
}