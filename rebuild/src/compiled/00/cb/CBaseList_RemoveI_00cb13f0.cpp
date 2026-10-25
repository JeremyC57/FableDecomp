// CBaseList::RemoveI(POSITION) -- DirectShow BaseClasses wxlist.cpp: unlink the node at a position from the doubly linked list, hand the node to the node cache (CNodeCache::AddToCache: kept while m_iUsed < m_iCacheSize, else deleted), decrement the count and return the node's object
// Leaf: ?RemoveI@CBaseList@@QAEPAXPAU__POSITION@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB13F0, 107 bytes (+ int3 padding), thiscall `ret 4`; operator delete 0x00BFE9BC.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB13F0 = CBaseList::RemoveI"): callers are RemoveHeadI 0x00CB1900 (`push
//   m_pFirst ; call`) and RemoveTailI 0x00CB1910 (`push m_pLast ; call`), 0x00CAFFFA and the jump thunk 0x00CAEF10; the cache
//   fields are m_iCacheSize +0x0c / m_iUsed +0x10 / m_pHead +0x14. The landed CPersistTraits::RemoveNodeAndRecycle (owner
//   "could not be confirmed") is the same body.
//   The CBaseList layout and CNode are the ones agent450's landed wxlist.cpp rows (RemoveAll 0x00CB1340, FindI 0x00CB13C0,
//   AddTailI 0x00CB1460, ...) matched with; chunk 450's batch README names this row.
// Label: the landed name is a propagated manifest label / by-address placeholder; the real name is the strmbase member.
// ---- strmbase model (DirectShow BaseClasses wxlist.h, DX9 SDK era, release build: no CBaseObject base, no vtable) ----
// Layout (as the landed CBaseList rows 0x00CB1340..0x00CB16A0): m_pFirst +0, m_pLast +4, m_Count +8, then the CNodeCache
// m_Cache {m_iCacheSize +0x0c, m_iUsed +0x10, m_pHead +0x14}; CNode {m_pPrev, m_pNext, m_pObject} is 0x0c bytes.
struct __POSITION { int unused; };
typedef __POSITION* POSITION;

void __cdecl operator delete(void* block);         // 0x00BFE9BC

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
        CNodeCache(int iCacheSize) : m_iCacheSize(iCacheSize), m_pHead(0), m_iUsed(0) {}
        ~CNodeCache();
        void AddToCache(CNode* pNode)
        {
            if (m_iUsed < m_iCacheSize) {
                pNode->SetNext(m_pHead);
                m_pHead = pNode;
                m_iUsed++;
            } else {
                delete pNode;
            }
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
    void* GetNextI(POSITION& rp) const;             // 0x00CB1380
    void* GetI(POSITION p) const;                   // 0x00CB13A0
    void* RemoveI(POSITION pos);                    // 0x00CB13F0
    void RemoveAll();                               // 0x00CB1340
};

void* CBaseList::RemoveI(POSITION pos)
{
    /* Lock the critical section before continuing */

    if (pos == 0) return 0;


    CNode* pCurrent = (CNode*)pos;

    /* Update the previous node */

    CNode* pNode = pCurrent->Prev();
    if (pNode == 0) {
        m_pFirst = pCurrent->Next();
    } else {
        pNode->SetNext(pCurrent->Next());
    }

    /* Update the following node */

    pNode = pCurrent->Next();
    if (pNode == 0) {
        m_pLast = pCurrent->Prev();
    } else {
        pNode->SetPrev(pCurrent->Prev());
    }

    /* Get the object this node was looking after */

    void* pObject = pCurrent->GetData();

    /* Try and add the node object to the cache -
       a NULL return code from the cache means we ran out of room.
       The cache size is fixed by a constructor argument when the
       list is created and defaults to DEFAULTCACHE.
    */

    m_Cache.AddToCache(pCurrent);

    /* If the list is empty then reset the list count */

    --m_Count;
    return pObject;
}
