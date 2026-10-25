// CBaseList::AddTail(CBaseList* pList) -- DirectShow wxlist.cpp: append every object of another list with AddTailI; FALSE as soon as one add fails, else TRUE
// Leaf: ?AddTail@CBaseList@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1540, 53 bytes. __fastcall, `ret 4`.
// Library: DirectShow base classes (strmbase wxlist.cpp). The manifest label "List_CopyAllNodes" is descriptive.
// Callee: CBaseList::AddTailI @ 0x00CB1460 (__fastcall, kept out of line -- declared only here).
// Shape: `pos = pList->GetHeadPositionI(); while (pos) { if (NULL == AddTailI(pList->GetNextI(pos))) return FALSE; }`
// with GetNextI inlined: its NULL test merges into the loop test and pos lives in esi.
// Model: the DX9 SDK wxlist.h CBaseList (retail build: no CBaseObject base, no vtable). m_pFirst +0, m_pLast +4,
// m_Count +8, then the CNodeCache m_Cache {m_iCacheSize +0x0c, m_iUsed +0x10, m_pHead +0x14}; CNode {m_pPrev,
// m_pNext, m_pObject} is 0x0c bytes and its empty constructor makes `new CNode` a plain operator new(0x0c).
struct __POSITION { int unused; };
typedef __POSITION* POSITION;

void __cdecl operator delete(void* block);         // 0x00BFE9BC
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
    int AddTail(CBaseList* pList);
};

int CBaseList::AddTail(CBaseList* pList)
{
    POSITION pos = pList->GetHeadPositionI();

    while (pos) {
        if (0 == AddTailI(pList->GetNextI(pos))) {
            return 0;
        }
    }
    return 1;
}