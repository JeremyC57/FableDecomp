// CInputManager::Update() -- per-frame input: while loading a recorded package, apply it when its time stamp is due and refresh the checksum; otherwise update the mouse, keyboard and joysticks, clear both event vectors, scan every device's events into the event store, turn the inkey buffer into keyboard character / repeat events (GFGetTime stamps) and save the event package when recording
// Grade: DIFFER(813v1072) 254/368 -- control flow, calls and vtable slots match; residue: (1) retail inlines CInputEvent's copy constructor at the joystick and inkey push_backs (two ~0x83-byte member-wise copies with the union's overlapping stores) but calls it out of line at the mouse and keyboard ones -- an inline model inlines all four (1210 bytes), a template scan helper either stays out of line or inlines all; (2) this lives in esi in retail (edi here) and the zero register is set up later
// Leaf: ?Update@CInputManager@@UAEXXZ
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F57A0, 1072 bytes, plain `ret`, `and esp,-8` frame.
// Name: Ego_r.pdb ?Update@CInputManager@@UAEXXZ + the manifest row.
// Callees: CInputEvent() 0x00A04410, CInputEvent(const CInputEvent&) 0x009E4850, ~CInputEvent 0x00A03B30,
//   SetAsKeyboardCharacter 0x00A03CB0 / ...Repeat 0x00A03CE0 (wchar_t, double), CInputManager::SetEventsFromEventPackage 0x009F54D0,
//   CalculateInternalChecksum 0x009F5170, SaveEventPackage 0x009F53C0, vector<CInputEvent>::erase 0x009F67E0 /
//   _M_insert_overflow 0x009F6AC0, CMouse::EventScanStart/Next/End 0x00A66B70/0x00A66BC0/0x00A66B00, CKeyboard's
//   0x00A667C0/0x00A66810/0x00A66850, CInkeyBuffer::GetChar 0x00A5FAC0 on the global at 0x013CB2F8 (g_InkeyBuffer: name
//   unproven), GFGetTime 0x009E1BC0; mouse Update slot 5, keyboard slot 1, joystick Update slot 10 / scans slots 0-2.
// Layout: Ego_r.pdb CInputManager with retail 12-byte vectors: ProcessedEvents +0x04, EventStore +0x10, NoStoredEvents
//   +0x1C, PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2C, Loading +0x38, Saving +0x39, LoadedEventPackages +0x4C (0x30-byte
//   packages, TimeStamp +0x0C), CurrentFilePackageIndex +0x58, TimeStamp +0x5C, NoUpdates +0x7C.
// Levers tried (all inert or worse): per-loop index variables, else-block instead of the early return, scan-info
//   initialised in the body, inline copy constructor (all four sites inline), template scan helper (out of line /
//   force-inlined).
typedef unsigned short wchar_t;
double __fastcall GFGetTime();                          // 0x009E1BC0 ?GFGetTime@@YINXZ
class CInputEvent { public:
    unsigned char Data[0x20];                           // UInputEventData
    long DeviceType;                                    // +0x20
    long DeviceNumber;                                  // +0x24
    long Type;                                          // +0x28
    float CurrentTime;                                  // +0x2c
    float StartTime;                                    // +0x30
    CInputEvent();                                      // 0x00A04410
    CInputEvent(const CInputEvent& other);              // 0x009E4850
    ~CInputEvent();                                     // 0x00A03B30
    void SetAsKeyboardCharacter(wchar_t c, double time);        // 0x00A03CB0
    void SetAsKeyboardCharacterRepeat(wchar_t c, double time);  // 0x00A03CE0
};
class CInputEventScanInfo { public:
    bool Started;
    long Index;
    CInputEventScanInfo() : Started(false) {}
};
namespace std {
struct __false_type { };
template <class _Tp> class vector { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    typedef unsigned int size_type;
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type n) { return _M_start[n]; }
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    _Tp* erase(_Tp* first, _Tp* last);
    void clear() { erase(begin(), end()); }
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) _Tp(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, size_type fill, bool atend);
};
}
inline void* operator new(unsigned int, void* p) { return p; }
class CMouse { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void Update();                              // slot 5
    bool EventScanStart(CInputEventScanInfo& info, CInputEvent& event) const;   // 0x00A66B70
    bool EventScanNext(CInputEventScanInfo& info, CInputEvent& event) const;    // 0x00A66BC0
    void EventScanEnd(CInputEventScanInfo& info) const;                         // 0x00A66B00
};
class CKeyboard { public:
    virtual void v0();
    virtual void Update();                              // slot 1
    bool EventScanStart(CInputEventScanInfo& info, CInputEvent& event) const;   // 0x00A667C0
    bool EventScanNext(CInputEventScanInfo& info, CInputEvent& event) const;    // 0x00A66810
    void EventScanEnd(CInputEventScanInfo& info) const;                         // 0x00A66850
};
class CJoystick { public:
    virtual bool EventScanStart(CInputEventScanInfo& info, CInputEvent& event) const;   // slot 0
    virtual bool EventScanNext(CInputEventScanInfo& info, CInputEvent& event) const;    // slot 1
    virtual void EventScanEnd(CInputEventScanInfo& info) const;                         // slot 2
    virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void Update();                              // slot 10
};
template <class T> class CCountedPointer { public:
    T* Data;
    void* Count;
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};
class CInputEventPackage { public:
    unsigned char Events[0xc];                          // vector<CInputEvent>
    long TimeStamp;                                     // +0x0c
    unsigned char _rest[0x20];
};
class CInkeyBuffer { public:
    bool GetChar(wchar_t* c, bool* pressed);            // 0x00A5FAC0
};
extern CInkeyBuffer g_InkeyBuffer;                      // 0x013CB2F8 (name unproven)
class CInputManager { public:
    virtual void Update();
    std::vector<CInputEvent> ProcessedEvents;           // +0x04
    std::vector<CInputEvent> EventStore;                // +0x10
    long NoStoredEvents;                                // +0x1c
    long NoEventScansRunning;                           // +0x20
    CMouse* PMouse;                                     // +0x24 scoped_ptr
    CKeyboard* PKeyboard;                               // +0x28 scoped_ptr
    std::vector<CCountedPointer<CJoystick> > PJoysticks;   // +0x2c
    bool Loading;                                       // +0x38
    bool Saving;                                        // +0x39
    unsigned char _pad3a[0x48 - 0x3a];
    unsigned long Checksum;                             // +0x48
    std::vector<CInputEventPackage> LoadedEventPackages;   // +0x4c
    long CurrentFilePackageIndex;                       // +0x58
    long TimeStamp;                                     // +0x5c
    unsigned char _pad60[0x7c - 0x60];
    unsigned long NoUpdates;                            // +0x7c
    unsigned long CalculateInternalChecksum() const;    // 0x009F5170
    void SaveEventPackage();                            // 0x009F53C0
    void SetEventsFromEventPackage(const CInputEventPackage& package);   // 0x009F54D0
};
void CInputManager::Update()
{
    CInputEventScanInfo scanInfo;
    CInputEvent event;
    ++NoUpdates;
    if (Loading) {
        CInputEventPackage& package = LoadedEventPackages[CurrentFilePackageIndex];
        if (package.TimeStamp == TimeStamp) {
            SetEventsFromEventPackage(package);
            Checksum = CalculateInternalChecksum();
            ++CurrentFilePackageIndex;
        }
        return;
    }
    if (PMouse)
        PMouse->Update();
    if (PKeyboard)
        PKeyboard->Update();
    long joysticks = PJoysticks.size();
    long i;
    for (i = 0; i < joysticks; ++i)
        PJoysticks[i]->Update();
    ProcessedEvents.clear();
    EventStore.clear();
    NoStoredEvents = 0;
    if (PMouse) {
        if (PMouse->EventScanStart(scanInfo, event)) {
            do {
                EventStore.push_back(event);
                ++NoStoredEvents;
            } while (PMouse->EventScanNext(scanInfo, event));
        }
        PMouse->EventScanEnd(scanInfo);
    }
    if (PKeyboard) {
        if (PKeyboard->EventScanStart(scanInfo, event)) {
            do {
                EventStore.push_back(event);
                ++NoStoredEvents;
            } while (PKeyboard->EventScanNext(scanInfo, event));
        }
        PKeyboard->EventScanEnd(scanInfo);
    }
    for (i = 0; i < joysticks; ++i) {
        if (PJoysticks[i].Get()) {
            if (PJoysticks[i]->EventScanStart(scanInfo, event)) {
                do {
                    EventStore.push_back(event);
                    ++NoStoredEvents;
                } while (PJoysticks[i]->EventScanNext(scanInfo, event));
            }
            PJoysticks[i]->EventScanEnd(scanInfo);
        }
    }
    wchar_t c;
    bool pressed;
    while (g_InkeyBuffer.GetChar(&c, &pressed) == true) {
        if (pressed == true)
            event.SetAsKeyboardCharacter(c, GFGetTime());
        else
            event.SetAsKeyboardCharacterRepeat(c, GFGetTime());
        EventStore.push_back(event);
        ++NoStoredEvents;
    }
    if (Saving)
        SaveEventPackage();
}
