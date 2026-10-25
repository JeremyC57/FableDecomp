// CBaseList::GetNextI(POSITION&) -- DirectShow BaseClasses wxlist.cpp: return the object at the position and step the position to the next node (NULL, position unchanged, at the end)
// Leaf: ?GetNextI@CBaseList@@QBEPAXAAPAU__POSITION@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB1380, 24 bytes (+ int3 padding), thiscall `ret 4` (`this` unused).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB1380 CCharString_GetLength = CBaseList::GetNextI"): rp -> CNode, rp =
//   node->m_pNext (+4), result node->m_pObject (+8); callers are CGenericList<T>::GetNext users in the strmbase TUs: the
//   landed CCmdQueue::~CCmdQueue 0x00CAF580 (0x00CAF5BE / 0x00CAF5F7, its comment names CB1380 GetNextI), 0x00CAFF11,
//   0x00CAFFD4, 0x00CB0F6A and the jump thunks 0x00CAEEF0 / 0x00CB0CB0. The landed `long __stdcall GetLength(CCharString*)`
//   is the same bytes as a free function on the position reference.
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

void* CBaseList::GetNextI(POSITION& rp) const
{
    /* have we reached the end of the list */

    if (rp == 0) {
        return 0;
    }

    /* Lock the object before continuing */

    void* pObject;

    /* Copy the original position then step on */

    CNode* pn = (CNode*)rp;
    rp = (POSITION)pn->Next();

    /* Get the object at the original position from the list */

    pObject = pn->GetData();
    return pObject;
}
