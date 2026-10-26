// CInputProcessManager::RemoveAllInputProcesses() -- walk the input-process list with a registered (mutation-safe) scan and detach every process from the interface, then unregister the scan
// Leaf: RemoveAllInputProcesses
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D530, 179 bytes; called from ~CInputProcessManager (0x00A0D810) and its deleting
//   destructor (0x00A0D950, vtable 0x0129CA38 slot 0).
// Callees: CAInputProcess::RemoveFromInterface() @ 0x00A0D2D0 (landed; it calls RemoveInputProcess, which unlinks the
//   node and moves every registered scan off it -- hence the scan registration here).
// Layout (Ego_r.pdb): CInputProcessManager {vptr, InputProcessList (CLinkedList<CAInputProcess>: Head, Tail, NoEntries,
//   FirstScanInfo) +4, QueuedProcessedInputs[10] +0x14, NoQueuedProcessedInputs}; CLinkedListPosition {Data, Next,
//   Previous}; CLinkedListScanInfo (28 bytes) {Current, Next, Previous, Started, Forwards, NextScan, PreviousScan, List}.
//   There is no engine header for the manager or the list; the model is the landed PreparePrimitivesForEngine
//   (0x00A0D6B0) one.
// Shape: StartScanAtHead / ScanForward / RemoveScanInfo inline list members (the scan sits in the frame, Forwards and
//   the 0xb0-byte inputs untouched), the loop runs while the position's Data is non-null.
class CBaseClass { public: virtual ~CBaseClass(); };
class CLinkedListPosition { public: CBaseClass* Data; CLinkedListPosition* Next; CLinkedListPosition* Previous; };

class CAInputProcess : public CBaseClass { public:
    void RemoveFromInterface();                    // 0x00A0D2D0
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