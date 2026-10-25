// Behaviour test for 0x00CB1340 CBaseList::RemoveAll: every node deleted, count and both ends reset, cache untouched.
#include <cstdio>

struct __POSITION { int unused; };
typedef __POSITION* POSITION;

static int g_news = 0, g_deletes = 0;

class CBaseList {
public:
    class CNode {
        CNode* m_pPrev;
        CNode* m_pNext;
        void*  m_pObject;
    public:
        CNode() {}
        CNode* Prev() const { return m_pPrev; }
        CNode* Next() const { return m_pNext; }
        void SetPrev(CNode* p) { m_pPrev = p; }
        void SetNext(CNode* p) { m_pNext = p; }
        void* GetData() const { return m_pObject; }
        void SetData(void* p) { m_pObject = p; }
    };
    class CNodeCache {
    public:
        CNodeCache() : m_iCacheSize(4), m_iUsed(0), m_pHead(0) {}
        void AddToCache(CNode* pNode)
        {
            pNode->SetNext(m_pHead);
            m_pHead = pNode;
            m_iUsed++;
        }
        CNode* RemoveFromCache()
        {
            CNode* pNode = m_pHead;
            if (pNode != 0) {
                m_pHead = pNode->Next();
                m_iUsed--;
            }
            return pNode;
        }
        int Used() const { return m_iUsed; }
    private:
        int    m_iCacheSize;
        int    m_iUsed;
        CNode* m_pHead;
    };
    CNode* m_pFirst;
    CNode* m_pLast;
    long   m_Count;
    CNodeCache m_Cache;

    CBaseList() : m_pFirst(0), m_pLast(0), m_Count(0) {}
    static CNode* NewNode() { ++g_news; return new CNode; }
    static void DeleteNode(CNode* p) { ++g_deletes; delete p; }
    POSITION GetHeadPositionI() const { return (POSITION)m_pFirst; }
    POSITION Next(POSITION pos) const
    {
        if (pos == 0)
            return (POSITION)m_pFirst;
        CNode* pn = (CNode*)pos;
        return (POSITION)pn->Next();
    }
    void* GetI(POSITION p) const
    {
        if (p == 0)
            return 0;
        CNode* pn = (CNode*)p;
        return pn->GetData();
    }
    void* GetNextI(POSITION& rp) const
    {
        if (rp == 0)
            return 0;
        CNode* pn = (CNode*)rp;
        rp = (POSITION)pn->Next();
        return pn->GetData();
    }

    void RemoveAll()
    {
        CNode* pn = m_pFirst;
        while (pn) {
            CNode* op = pn;
            pn = pn->Next();
            DeleteNode(op);
        }
        m_Count = 0;
        m_pFirst = m_pLast = 0;
    }
    POSITION FindI(void* pObj) const
    {
        POSITION pn;
        for (pn = GetHeadPositionI(); pn != 0; pn = Next(pn))
            if (GetI(pn) == pObj)
                return pn;
        return 0;
    }
    POSITION AddTailI(void* pObject)
    {
        CNode* pNode = m_Cache.RemoveFromCache();
        if (pNode == 0)
            pNode = NewNode();
        if (pNode == 0)
            return 0;
        pNode->SetData(pObject);
        pNode->SetNext(0);
        pNode->SetPrev(m_pLast);
        if (m_pLast == 0)
            m_pFirst = pNode;
        else
            m_pLast->SetNext(pNode);
        m_pLast = pNode;
        ++m_Count;
        return (POSITION)pNode;
    }
    POSITION AddHeadI(void* pObject)
    {
        CNode* pNode = m_Cache.RemoveFromCache();
        if (pNode == 0)
            pNode = NewNode();
        if (pNode == 0)
            return 0;
        pNode->SetData(pObject);
        pNode->SetPrev(0);
        pNode->SetNext(m_pFirst);
        if (m_pFirst == 0)
            m_pLast = pNode;
        else
            m_pFirst->SetPrev(pNode);
        m_pFirst = pNode;
        ++m_Count;
        return (POSITION)pNode;
    }
    int AddTail(CBaseList* pList)
    {
        POSITION pos = pList->GetHeadPositionI();
        while (pos) {
            if (0 == AddTailI(pList->GetNextI(pos)))
                return 0;
        }
        return 1;
    }
    POSITION AddBeforeI(POSITION pos, void* pObj)
    {
        if (pos == 0)
            return AddTailI(pObj);
        CNode* pAfter = (CNode*)pos;
        if (pAfter == m_pFirst)
            return AddHeadI(pObj);
        CNode* pNode = m_Cache.RemoveFromCache();
        if (pNode == 0)
            pNode = NewNode();
        if (pNode == 0)
            return 0;
        pNode->SetData(pObj);
        CNode* pBefore = pAfter->Prev();
        pNode->SetPrev(pBefore);
        pNode->SetNext(pAfter);
        pAfter->SetPrev(pNode);
        pBefore->SetNext(pNode);
        ++m_Count;
        return (POSITION)pNode;
    }
};

static int g_obj[8];
// the list's objects in order must be exactly `idx` (indices into g_obj), both directions consistent
static bool Check(const CBaseList& l, const int* idx, int n)
{
    if (l.m_Count != n) return false;
    CBaseList::CNode* p = l.m_pFirst;
    CBaseList::CNode* prev = 0;
    for (int i = 0; i < n; ++i) {
        if (p == 0 || p->GetData() != &g_obj[idx[i]] || p->Prev() != prev) return false;
        prev = p;
        p = p->Next();
    }
    return p == 0 && l.m_pLast == prev;
}

int main()
{
    bool ok = true;

    CBaseList l;
    CBaseList::CNode* cached = new CBaseList::CNode;
    l.m_Cache.AddToCache(cached);
    l.AddTailI(&g_obj[0]);              // reuses the cached node
    l.AddTailI(&g_obj[1]);
    l.AddTailI(&g_obj[2]);
    int del0 = g_deletes;
    l.RemoveAll();
    ok = ok && g_deletes - del0 == 3 && l.m_Count == 0 && l.m_pFirst == 0 && l.m_pLast == 0 && l.m_Cache.Used() == 0;
    l.RemoveAll();                      // empty list: nothing deleted
    ok = ok && g_deletes - del0 == 3;
    if (!ok) { std::printf("00cb1340_TEST FAIL\n"); return 1; }
    std::printf("00cb1340_TEST PASS\n");
    return 0;
}