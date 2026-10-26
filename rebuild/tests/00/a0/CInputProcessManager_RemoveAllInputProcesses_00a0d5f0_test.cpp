// Behaviour test for CInputProcessManager::RemoveAllInputProcesses (0x00A0D5F0): every process is detached even
// though each detach unlinks its own node during the scan; the scan is unregistered and an outer scan survives.
#include <cstdio>
class CBaseClass { public: virtual ~CBaseClass() {} };
class CLinkedListPosition { public: CBaseClass* Data; CLinkedListPosition* Next; CLinkedListPosition* Previous; };

struct CInputProcessList;
class CAInputProcess : public CBaseClass { public:
    CInputProcessList* m_list; CLinkedListPosition m_pos; int m_removed;
    void RemoveFromInterface();                    // 0x00A0D2D0 (test: unlink like RemoveInputProcess)
};

class CLinkedListScanInfo;
struct CInputProcessList {                         // CLinkedList<CAInputProcess>
    CLinkedListPosition* Head;
    CLinkedListPosition* Tail;
    long NoEntries;
    mutable CLinkedListScanInfo* FirstScanInfo;
    CAInputProcess* StartScanAtHead(CLinkedListScanInfo* s) const;
    CAInputProcess* ScanForward(CLinkedListScanInfo* s) const;
    void RemoveScanInfo(CLinkedListScanInfo* s) const;
};
class CLinkedListScanInfo { public:
    CLinkedListPosition* Current;
    CLinkedListPosition* Next;
    CLinkedListPosition* Previous;
    bool Started, Forwards;
    CLinkedListScanInfo* NextScan;
    CLinkedListScanInfo* PreviousScan;
    const CInputProcessList* List;
    CLinkedListScanInfo() { }
};
inline CAInputProcess* CInputProcessList::StartScanAtHead(CLinkedListScanInfo* s) const
{
    s->NextScan = FirstScanInfo;
    if (FirstScanInfo)
        FirstScanInfo->PreviousScan = s;
    FirstScanInfo = s;
    s->List = this;
    s->PreviousScan = 0;
    s->Started = true;
    CLinkedListPosition* p = Head;
    if (!p)
        return 0;
    s->Current = p;
    s->Next = p->Next;
    s->Previous = 0;
    return static_cast<CAInputProcess*>(p->Data);
}
inline CAInputProcess* CInputProcessList::ScanForward(CLinkedListScanInfo* s) const
{
    CLinkedListPosition* p = s->Next;
    s->Current = p;
    if (!p)
        return 0;
    s->Next = p->Next;
    s->Previous = p->Previous;
    return static_cast<CAInputProcess*>(p->Data);
}
inline void CInputProcessList::RemoveScanInfo(CLinkedListScanInfo* s) const
{
    if (!s->PreviousScan) {
        CLinkedListScanInfo* next = s->NextScan;
        FirstScanInfo = next;
        if (next)
            next->PreviousScan = 0;
        return;
    }
    s->PreviousScan->NextScan = s->NextScan;
    if (s->NextScan)
        s->NextScan->PreviousScan = s->PreviousScan;
}

class CInputProcessManager : public CBaseClass { public:         // vtable 0x0129CA38
    CInputProcessList InputProcessList;            // +0x04
    unsigned char QueuedProcessedInputs[10][0xb0]; // +0x14 CProcessedInput[10]
    long NoQueuedProcessedInputs;                  // +0x6f4
    void RemoveAllInputProcesses();
};

void CInputProcessManager::RemoveAllInputProcesses()
{
    CLinkedListScanInfo scan;
    for (CAInputProcess* p = InputProcessList.StartScanAtHead(&scan); p; p = InputProcessList.ScanForward(&scan))
        p->RemoveFromInterface();
    InputProcessList.RemoveScanInfo(&scan);
}

void CAInputProcess::RemoveFromInterface()
{
    ++m_removed;
    CLinkedListPosition* n = &m_pos;
    for (CLinkedListScanInfo* s = m_list->FirstScanInfo; s; s = s->NextScan)
        if (s->Next == n) s->Next = n->Next;
    if (m_list->Head == n) m_list->Head = n->Next;
    if (m_list->Tail == n) m_list->Tail = n->Previous;
    if (n->Previous) n->Previous->Next = n->Next;
    if (n->Next) n->Next->Previous = n->Previous;
    n->Next = n->Previous = 0; n->Data = 0;
    --m_list->NoEntries;
}
int main()
{
    CInputProcessManager m;
    m.InputProcessList.Head = m.InputProcessList.Tail = 0; m.InputProcessList.NoEntries = 0;
    CLinkedListScanInfo outer; outer.NextScan = 0; outer.PreviousScan = 0; outer.Next = 0;
    m.InputProcessList.FirstScanInfo = &outer;
    CAInputProcess p[3];
    for (int i = 0; i < 3; ++i) {
        p[i].m_list = &m.InputProcessList; p[i].m_removed = 0; p[i].m_pos.Data = &p[i];
        p[i].m_pos.Previous = i ? &p[i - 1].m_pos : 0; p[i].m_pos.Next = i < 2 ? &p[i + 1].m_pos : 0;
    }
    m.InputProcessList.Head = &p[0].m_pos; m.InputProcessList.Tail = &p[2].m_pos; m.InputProcessList.NoEntries = 3;
    m.RemoveAllInputProcesses();
    int bad = 0;
    for (int i = 0; i < 3; ++i) if (p[i].m_removed != 1) bad = 1;
    if (m.InputProcessList.Head || m.InputProcessList.NoEntries) bad = 2;
    if (m.InputProcessList.FirstScanInfo != &outer || outer.PreviousScan != 0 || outer.NextScan != 0) bad = 3;
    m.InputProcessList.FirstScanInfo = 0;
    m.RemoveAllInputProcesses();   // empty list
    if (m.InputProcessList.FirstScanInfo != 0) bad = 4;
    if (bad) { std::printf("00a0d5f0_TEST FAIL\n"); return 1; }
    std::printf("00a0d5f0_TEST PASS\n");
    return 0;
}