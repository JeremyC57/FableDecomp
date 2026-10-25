// CBaseList::GetI(POSITION) -- DirectShow BaseClasses wxlist.cpp: the object at a position without stepping (NULL for a NULL position)
// Leaf: ?GetI@CBaseList@@QBEPAXPAU__POSITION@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB13A0, 16 bytes (+ int3 padding), thiscall `ret 4` (`this` unused).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB13A0 = CBaseList::GetI"): p -> CNode, result m_pObject (+8); called
//   by this sweep's CCmdQueue::GetDueCommand 0x00CAF8F0 (`m_listPresentation.Get(pos)` / `m_listStream.Get(pos)` at
//   0x00CAF932 / 0x00CAF94E), 0x00CAF77C / 0x00CAF79D, 0x00CAFA52 / 0x00CAFA6B, 0x00CAFEF3, 0x00CAFFC4 and the jump thunk
//   0x00CAEF00. The landed `CEditControlCentre_PeekConnectedToTrackNode(const CTrackNodeArg*)` is the same bytes as a free
//   `__stdcall` function on the node.
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

void* CBaseList::GetI(POSITION p) const
{
    if (p == 0) {
        return 0;
    }

    CNode* pn = (CNode*)p;
    void* pObject = pn->GetData();
    return pObject;
}
