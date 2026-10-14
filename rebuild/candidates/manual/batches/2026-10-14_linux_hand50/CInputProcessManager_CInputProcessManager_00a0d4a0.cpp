// CInputProcessManager::CInputProcessManager() -- CBase, own vtable, the input-process list (+4) cleared by its inline constructor, the ten queued CProcessedInput records (+0x14) built by an inlined counted loop (each runs the vector constructor iterator over its four CGameEvents), then NoQueuedProcessedInputs (+0x6f4) cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D4A0, length 76 bytes (+ int3 padding). Vtable 0x0129CA38 (CInputProcessManager).
// Callees: CBase::CBase @ 0x0099A2F0, `vector constructor iterator' @ 0x004038C0 (element size 0x28, count 4, CGameEvent::CGameEvent @ 0x004159A0).
// Layout from Ego_r.pdb: CLinkedList<CAInputProcess> {Head, Tail, NoEntries, FirstScanInfo} +4; CProcessedInput {Player +0, Type +4,
// CGameEvent[4] +8, NoGameEvents +0xa8, Priority +0xac} = 0xb0 (engine/CProcessedInput.h); NoQueuedProcessedInputs +0x6f4.
// Shape: as the landed CGameEventPackageSet constructor (0x009F1A80): the list's four zero stores sit after the vtable store (a member
// object with an inline constructor), the outer array is `dec ebx ; jne` over `add edi,0B0h`, and the count is stored after the loop
// (a body assignment; an initialiser would be stored first).
// Leaf: ??0CInputProcessManager
struct CGameEvent {
    long          Type;                             // +0x00
    signed char   Player;                           // +0x04
    unsigned char Data[32];                         // +0x05
    unsigned char EndPos;                           // +0x25
    bool          Valid;                            // +0x26
    bool          Replacement;                      // +0x27
    CGameEvent();                                   // 0x004159A0
};
struct CProcessedInput {                            // 0xb0 bytes
    long          Player;                           // +0x00
    long          Type;                             // +0x04 EProcessedEventType
    CGameEvent    GameEvents[4];                    // +0x08
    unsigned char NoGameEvents;                     // +0xa8
    long          Priority;                         // +0xac EGameEventPriority
    CProcessedInput() { }
};
struct CLinkedList_CAInputProcess {                 // 16 bytes
    void* Head;                                     // +0x00
    void* Tail;                                     // +0x04
    long  NoEntries;                                // +0x08
    void* FirstScanInfo;                            // +0x0c
    CLinkedList_CAInputProcess() : Head(0), Tail(0), NoEntries(0), FirstScanInfo(0) { }
};
struct CBase {
    virtual void v00();
    CBase();                                        // 0x0099A2F0
};
struct CInputProcessManager : CBase {               // vtable 0x0129CA38
    virtual void v00();
    CLinkedList_CAInputProcess InputProcessList;    // +0x04
    CProcessedInput QueuedProcessedInputs[10];      // +0x14
    long            NoQueuedProcessedInputs;        // +0x6f4
    CInputProcessManager();
};

CInputProcessManager::CInputProcessManager()
    : CBase(), InputProcessList()
{
    NoQueuedProcessedInputs = 0;
}
