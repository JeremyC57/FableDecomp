// CBaseList::AddAfterI(POSITION pos, void* pObj) -- DirectShow wxlist.cpp: insert an object after pos (AddHeadI for NULL, AddTailI after the last node), splicing a cached/new node in
// Leaf: ?AddAfterI@CBaseList@@QAEPAU__POSITION@@PAU2@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB15D0, 124 bytes, thiscall, `ret 8`.
// Identity (proven by source): DirectShow base classes (strmbase wxlist.cpp) CBaseList::AddAfterI -- NULL pos goes to AddHeadI
//   0x00CB14D0, pos == m_pLast (+4) to AddTailI 0x00CB1460, else a node from m_Cache.RemoveFromCache() or `new CNode` (0x0C bytes)
//   is chained between pAfter and pAfter->Next(). Its mirror AddBeforeI is the landed 0x00CB16A0 (model reused verbatim).
// Callees: CBaseList::AddHeadI 0x00CB14D0, CBaseList::AddTailI 0x00CB1460, scalar operator new 0x00BFEA1A.
// Shape: as the landed AddBeforeI, pBefore = pAfter->Next() is read after SetData (the DX9 SDK text reads it before the
//   allocation, which keeps it in ebx across the call: 3 bytes longer); then SetPrev(pAfter), SetNext(pBefore),
//   pBefore->SetPrev, pAfter->SetNext, ++m_Count.
struct __POSITION { int unused; };
typedef __POSITION* POSITION;

void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CBaseList {
public:
    class CNode {
        CNode* m_pPrev;                             // +0x00
        CNode* m_pNext;                             // +0x04
        void*  m_pObject;                           // +0x08
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
        CNode* RemoveFromCache()
        {
            CNode* pNode = m_pHead;
            if (pNode != 0) {
                m_pHead = pNode->Next();
                m_iUsed--;
            }
            return pNode;
        }
    private:
        int    m_iCacheSize;                        // +0x0c
        int    m_iUsed;                             // +0x10
        CNode* m_pHead;                             // +0x14
    };

protected:
    CNode* m_pFirst;                                // +0x00
    CNode* m_pLast;                                 // +0x04
    long   m_Count;                                 // +0x08
private:
    CNodeCache m_Cache;                             // +0x0c
public:
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
        void* pObject = pn->GetData();
        return pObject;
    }
    void* GetNextI(POSITION& rp) const
    {
        if (rp == 0)
            return 0;
        void* pObject;
        CNode* pn = (CNode*)rp;
        rp = (POSITION)pn->Next();
        pObject = pn->GetData();
        return pObject;
    }

    POSITION AddTailI(void* pObject);               // 0x00CB1460
    POSITION AddHeadI(void* pObject);               // 0x00CB14D0
    POSITION AddAfterI(POSITION pos, void* pObj);
};

POSITION CBaseList::AddAfterI(POSITION pos, void* pObj)
{
    if (pos == 0)
        return AddHeadI(pObj);

    CNode* pAfter = (CNode*)pos;
    if (pAfter == m_pLast)
        return AddTailI(pObj);

    CNode* pNode = (CNode*)m_Cache.RemoveFromCache();
    if (pNode == 0) {
        pNode = new CNode;
    }

    if (pNode == 0) {
        return 0;
    }

    pNode->SetData(pObj);

    CNode* pBefore = pAfter->Next();

    pNode->SetPrev(pAfter);
    pNode->SetNext(pBefore);
    pBefore->SetPrev(pNode);
    pAfter->SetNext(pNode);

    ++m_Count;

    return (POSITION)pNode;
}