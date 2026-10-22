// CKeyboardDX::UpdateKeyEvents() -- drain DirectInput's buffered keyboard data (up to 256 records, re-acquiring once when the input was lost) into the event store: each record becomes a key-pressed event, or a key-released event carrying the start time of the matching held key from the maintained-event list; then refresh the maintained events
// Leaf: UpdateKeyEvents
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB6240, 516 bytes (+ int3 padding); 8-aligned frame (`and esp,-8`, __chkstk 0x1450).
// Callees: CInputEvent ctor 0x00A04410 / dtor 0x00A03B30, GetType 0x00A03B40, GetKey 0x00A03B70, SetAsKeyboardKeyPressed(EInputKey,
//   double) 0x00A03BF0, SetAsKeyboardKeyReleased(EInputKey, double, double) 0x00A03C50 (FableWin.pdb names); CKeyboard
//   EmptyEventStore 0x00A66760, AddEventToStore 0x00A66770, UpdateMaintainedEvents 0x00A66880, AddMaintainedEventsToEventStore
//   0x00A668E0, ProcessMaintainedEventsFromEvent 0x00A669E0; UpdateAcquired (out-of-line copy 0x00AB6100, inlined here:
//   GFGetSystemManager 0x009A4EC0, IsApplicationActive 0x009A57B0, HasInputFocus 0x009A4FB0, virtual slot 3 SetHasInputFocus);
//   IDirectInputDevice8 COM slots 10 GetDeviceData and 7 Acquire. Constants 4294967296.0f 0x0122DCB4, 0.001f 0x01266300.
// Layout: retail CKeyboardDX as the landed CKeyboardDX::UpdateKeyStates 0x00AB5FF0 (DKeyboard +0x3418, KeyMap +0x351c); the
//   CKeyboard base keeps MaintainedEventsList (STLport list, node pointer) at +0x340c as in Ego_r.pdb; CMaintainedEvent
//   holds its CInputEvent at +4 (Ego_r.pdb), whose StartTime is at +0x30.
struct IDirectInputDevice8_Retail {                // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetCapabilities(void* caps);
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);
    virtual long __stdcall GetProperty(const void* guid, void* header);
    virtual long __stdcall SetProperty(const void* guid, const void* header);
    virtual long __stdcall Acquire();              // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();            // slot 8 (+0x20)
    virtual long __stdcall GetDeviceState(unsigned long size, void* data);   // slot 9 (+0x24)
    virtual long __stdcall GetDeviceData(unsigned long size, void* data, unsigned long* count, unsigned long flags);   // slot 10 (+0x28)
};
struct DIDEVICEOBJECTDATA_Retail {                 // 20 bytes
    unsigned long dwOfs;
    unsigned long dwData;
    unsigned long dwTimeStamp;
    unsigned long dwSequence;
    unsigned long uAppData;
};
enum EInputKey { INPUT_KEY_NONE };
struct CInputEvent {                               // 0x34 bytes
    unsigned char Data[0x30];
    float         StartTime;                       // +0x30
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
    long GetType() const;                          // 0x00A03B40
    EInputKey GetKey() const;                      // 0x00A03B70
    void SetAsKeyboardKeyPressed(EInputKey key, double time);                     // 0x00A03BF0
    void SetAsKeyboardKeyReleased(EInputKey key, double startTime, double time);  // 0x00A03C50
};
struct CMaintainedEvent {
    long        _pad0;
    CInputEvent Event;                             // +0x04
};
struct CMaintainedEventNode {                      // STLport _List_node<CMaintainedEvent>
    CMaintainedEventNode* Next;
    CMaintainedEventNode* Prev;
    CMaintainedEvent      Data;                    // +0x08
};
struct CSystemManager {
    bool IsApplicationActive();                    // 0x009A57B0
    bool HasInputFocus();                          // 0x009A4FB0
};
CSystemManager* __cdecl GFGetSystemManager();      // 0x009A4EC0
struct CKeyboardDX {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void SetHasInputFocus(bool focus);     // slot 3 (+0x0c)
    unsigned char               _pad4[0x340c - 4];
    CMaintainedEventNode*       MaintainedEventsList;   // +0x340c (list node)
    unsigned char               _pad3410[0x3418 - 0x3410];
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    unsigned short              KeyMap[0x7a];      // +0x351c
    bool                        Acquired;          // +0x3610
    void EmptyEventStore();                        // 0x00A66760
    bool AddEventToStore(const CInputEvent* event);   // 0x00A66770
    void UpdateMaintainedEvents();                 // 0x00A66880
    void AddMaintainedEventsToEventStore();        // 0x00A668E0
    void ProcessMaintainedEventsFromEvent(const CInputEvent* event);   // 0x00A669E0
    void UpdateAcquired()
    {
        if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus())
            SetHasInputFocus(true);
        else
            SetHasInputFocus(false);
    }
    EInputKey ConvertDXKeyToInputKey(long dxKey) const
    {
        for (long i = 0; i < 0x7a; ++i)
            if (KeyMap[i] == dxKey)
                return (EInputKey)i;
        return (EInputKey)0;
    }
    bool UpdateKeyEvents();
};

bool CKeyboardDX::UpdateKeyEvents()
{
    CInputEvent event;
    EmptyEventStore();
    UpdateAcquired();
    DIDEVICEOBJECTDATA_Retail data[0x100];
    unsigned long count = 0x100;
    long result = DKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA_Retail), data, &count, 0);
    if (result == (long)0x8007001E) {
        DKeyboard->Acquire();
        result = DKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA_Retail), data, &count, 0);
    }
    if (result >= 0) {
        for (unsigned long i = 0; i < count; ++i) {
            EInputKey key = ConvertDXKeyToInputKey(data[i].dwOfs);
            if (data[i].dwData & 0x80) {
                event.SetAsKeyboardKeyPressed(key, (float)data[i].dwTimeStamp * 0.001f);
            } else {
                double startTime = (float)data[i].dwTimeStamp * 0.001f;
                for (CMaintainedEventNode* node = MaintainedEventsList->Next; node != MaintainedEventsList; node = node->Next) {
                    if (node->Data.Event.GetType() == 2 && node->Data.Event.GetKey() == key) {
                        startTime = node->Data.Event.StartTime;
                        break;
                    }
                }
                event.SetAsKeyboardKeyReleased(key, startTime, (float)data[i].dwTimeStamp * 0.001f);
            }
            AddEventToStore(&event);
            ProcessMaintainedEventsFromEvent(&event);
        }
    }
    UpdateMaintainedEvents();
    AddMaintainedEventsToEventStore();
    return true;
}
