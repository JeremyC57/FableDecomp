// Grade: DIFFER(1026v1002) -- first pass: safe-iterator attach/detach, processor offer loop, best-priority copy and requeue all in shape; retail places the alt-key query event between bestEvent and the result buffers (0xa4) where VC7.1 here puts it last, and the iterator/scalars sit 4 bytes higher; loop tail order not yet compared
// CGamePlayerInterface::GetInputProcessEvent(CInputProcessEvent* out) -- scan the queued input events, offer each unprocessed one to every registered input processor and keep the highest-priority accepted result (requeueing events a processor defers); copy the winner out and requeue its event
// Leaf: ?GetInputProcessEvent@CGamePlayerInterface
// Flags: /GS
struct CInputEvent {
    unsigned char _pad00[0x24];
    long DeviceNumber;                             // +0x24
    unsigned char _pad28[0x0c];
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
    void Reset();                                  // 0x00A03BE0
    long GetType() const;                          // 0x00A03B40
    long GetDeviceType() const;                    // 0x00A03B50
    long GetKey() const;                           // 0x00A03B70
};
struct CInputEventScanInfo {
    bool Started;
    long Index;
    CInputEventScanInfo() : Started(false) { }
};
struct CInputManager {
    bool EventScanStart(CInputEventScanInfo* scan, CInputEvent* event);   // 0x009F4ED0
    bool EventScanNext(CInputEventScanInfo* scan, CInputEvent* event);    // 0x009F4F10
    void EventScanEnd(CInputEventScanInfo* scan);                         // 0x009F4AC0
    bool IsEventProcessed(CInputEvent* event);                            // 0x009F4B10
    bool IsKeyboardEventInQueue(long type, long key, CInputEvent* event); // 0x009F50A0
    void RequeueEvent(CInputEvent* event);                                // 0x009F55C0
};
extern CInputManager* g_pInputManager;             // 0x013B8388
struct CDisplay { unsigned char _pad[0xa4]; bool Minimised; };
struct CEngine {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual bool IsPaused();                       // slot 9 (+0x24)
    unsigned char _pad04[0x20];
    CDisplay* Display;                             // +0x24
};
extern CEngine* g_pEngine;                         // 0x013B86A0
struct CInputProcessSlot {
    char Key;                                      // +0x00
    unsigned char _pad01[0x21];
    bool Active;                                   // +0x22
    unsigned char _pad23[5];
    CInputProcessSlot() : Key(-1), Active(false) { }
};
struct CInputProcessEvent {
    long Type;                                     // +0x00
    long State;                                    // +0x04
    long _08;
    CInputProcessSlot Slots[4];                    // +0x0c
    long _ac;
    void Reset();                                  // 0x00A0D300
    void SetPlayerNumber(long player);             // 0x00A0D3A0
    long GetPriority() const;                      // 0x00A0D3B0
};
struct CInputProcessBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void Process(CInputEvent* event, CInputProcessEvent* result);  // +0x10
    virtual void v14(); virtual void v18();
    virtual bool ProcessWhenMinimised();           // +0x1c
    virtual bool Accepts(CInputEvent* event);      // +0x20
};
struct CPlayerManager {
    long GetPlayerNumberFromJoystickDeviceNumber(long device);   // 0x00449990
};
struct CProcessNode { CInputProcessBase* Data; CProcessNode* Next; CProcessNode* Prev; };
struct CSafeIterator;
struct CProcessList {
    CProcessNode* Head;
    long _04, _08;
    CSafeIterator* Iterators;                      // +0x0c
};
struct CSafeIterator {
    CProcessNode* Node;
    CProcessNode* Next;
    CProcessNode* Prev;
    bool Attached;
    CSafeIterator* NextIterator;
    CSafeIterator* PrevIterator;
    CProcessList* List;
    CSafeIterator() : Attached(false), NextIterator(0), PrevIterator(0), List(0) { }
    void Attach(CProcessList* list)
    {
        NextIterator = list->Iterators;
        if (NextIterator)
            NextIterator->PrevIterator = this;
        list->Iterators = this;
        List = list;
        PrevIterator = 0;
        Attached = true;
    }
    void Detach()
    {
        if (PrevIterator) {
            PrevIterator->NextIterator = NextIterator;
            if (NextIterator)
                NextIterator->PrevIterator = PrevIterator;
        }
        else {
            List->Iterators = NextIterator;
            if (NextIterator)
                NextIterator->PrevIterator = 0;
        }
        Attached = false;
        NextIterator = 0;
        PrevIterator = 0;
        List = 0;
    }
    void Set(CProcessNode* node)
    {
        Node = node;
        if (node) {
            Next = node->Next;
            Prev = node->Prev;
        }
        else {
            Next = 0;
            Prev = 0;
        }
    }
};
struct CGamePlayerInterface {
    virtual void v00();
    CProcessList Processors;                       // +0x04
    unsigned char _pad14[0x6e8];
    CPlayerManager* PlayerManager;                 // +0x6fc
    bool GetInputProcessEvent(CInputProcessEvent* out);
};
bool CGamePlayerInterface::GetInputProcessEvent(CInputProcessEvent* out)
{
    CSafeIterator it;
    CInputEvent event;
    CInputEvent bestEvent;
    CInputProcessEvent result;
    CInputProcessEvent bestResult;
    bestEvent.Reset();
    bestResult.Reset();
    bool altDown = false;
    if (g_pEngine && !g_pEngine->IsPaused()) {
        CInputEvent query;
        altDown = g_pInputManager->IsKeyboardEventInQueue(2, 0x38, &query);
    }
    CInputEventScanInfo scan;
    long bestPriority = 0;
    bool found = false;
    for (bool more = g_pInputManager->EventScanStart(&scan, &event); more; more = g_pInputManager->EventScanNext(&scan, &event)) {
        if (altDown && event.GetDeviceType() == 2 && event.GetKey() == 0xf)
            continue;
        if (event.GetType() == 0)
            continue;
        if (g_pInputManager->IsEventProcessed(&event))
            continue;
        it.Attach(&Processors);
        it.Set(Processors.Head);
        CInputProcessBase* processor;
        while (it.Node && (processor = it.Node->Data) != 0) {
            result.Reset();
            if (processor->Accepts(&event)) {
                result.SetPlayerNumber(PlayerManager->GetPlayerNumberFromJoystickDeviceNumber(event.DeviceNumber));
                if (processor->ProcessWhenMinimised() || !g_pEngine->Display->Minimised)
                    processor->Process(&event, &result);
            }
            if (result.State == 1) {
                found = true;
                if (result.GetPriority() > bestPriority) {
                    bestPriority = result.GetPriority();
                    bestResult = result;
                    bestEvent = event;
                }
            }
            else if (result.State == 2) {
                g_pInputManager->RequeueEvent(&event);
                break;
            }
            it.Set(it.Next);
        }
        it.Detach();
    }
    g_pInputManager->EventScanEnd(&scan);
    if (found) {
        *out = bestResult;
        if (bestEvent.GetType())
            g_pInputManager->RequeueEvent(&bestEvent);
        return true;
    }
    return false;
}
