// CMouseDX::SetExclusiveMode(bool exclusive) -- drop the current DirectInput mouse device and the maintained events, then recreate it: exclusive (foreground, buffered, acquired) or non-exclusive (background, buffered, acquired, Windows-mouse state reset); false on any DirectInput failure
// Leaf: ?SetExclusiveMode@CMouseDX
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5710, length 464 bytes (453 + int3 padding), virtual, `ret 4`.
// Callees: list<CMaintainedEvent>::clear 0x009E4710 (manifest "CGuiControlTreePane::SortTreeRecursively" is propagated; the
//   body walks and frees the list nodes), GFGetSystemManager 0x009A4EC0 (`__fastcall`; window handle at +0x94),
//   CMouseDX::ResetNonExclusiveMouse 0x00AB5030; IDirectInput8W::CreateDevice (slot 3) on the input manager's +0x80;
//   IDirectInputDevice8W Release (2), SetProperty (6), Acquire (7), Unacquire (8), SetDataFormat (11), SetCooperativeLevel (13).
// Data: GUID_SysMouse 0x012ACAA4, c_dfDIMouse2 0x012ADDFC, c_dfDIMouse 0x012ADBF4 (DIPROP_BUFFERSIZE is the constant 1).
// Layout: the landed CMouseDX::CMouseDX 0x00AB5D00 model (retail = PDB - 8 after CMouse): MaintainedEventsList +0x340c,
//   DMouse +0x341c, InputManagerDX +0x3420, Acquired +0x3424, ExclusiveMode +0x343c.
// Shape: the two modes are written out in full as nested success tests with one shared `return false` at the end (VC7.1
//   hoists the shared CreateDevice argument pushes above the branch; early `return false`s place an exit mid-body); the
//   window handle comes through an inline CSystemManager accessor (a named local swaps eax/ecx); the buffer-size
//   DIPROPDWORD is a frame local filled in declaration order; c_dfDIMouse2 falls back to c_dfDIMouse.
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct DIDATAFORMAT { unsigned long dwSize, dwObjSize, dwFlags, dwDataSize, dwNumObjs; void* rgodf; };
struct DIPROPHEADER { unsigned long dwSize, dwHeaderSize, dwObj, dwHow; };
struct DIPROPDWORD { DIPROPHEADER diph; unsigned long dwData; };
extern "C" const GUID GUID_SysMouse;                // 0x012ACAA4
extern "C" const DIDATAFORMAT c_dfDIMouse2;         // 0x012ADDFC
extern "C" const DIDATAFORMAT c_dfDIMouse;          // 0x012ADBF4
#define DIPROP_BUFFERSIZE (*(const GUID*)1)

struct IDirectInputDevice8W {
    virtual void __stdcall v00();
    virtual void __stdcall v01();
    virtual unsigned long __stdcall Release();                                          // slot 2 (+0x08)
    virtual void __stdcall v03();
    virtual void __stdcall v04();
    virtual void __stdcall v05();
    virtual long __stdcall SetProperty(const GUID& property, const DIPROPHEADER* header);   // slot 6 (+0x18)
    virtual long __stdcall Acquire();                                                  // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();                                                // slot 8 (+0x20)
    virtual void __stdcall v09();
    virtual void __stdcall v10();
    virtual long __stdcall SetDataFormat(const DIDATAFORMAT* format);                  // slot 11 (+0x2c)
    virtual void __stdcall v12();
    virtual long __stdcall SetCooperativeLevel(void* window, unsigned long flags);      // slot 13 (+0x34)
};
struct IDirectInput8W {
    virtual void __stdcall v00();
    virtual void __stdcall v01();
    virtual void __stdcall v02();
    virtual long __stdcall CreateDevice(const GUID& guid, IDirectInputDevice8W** device, void* outer);   // slot 3 (+0x0c)
};
struct CInputManagerDX {
    unsigned char   _pad00[0x80];
    IDirectInput8W* DInput;                         // +0x80
};
struct CSystemManager {
    unsigned char _pad00[0x94];
    void*         WindowHandle;                     // +0x94
    void* GetWindowHandle() const { return WindowHandle; }
};
CSystemManager* __fastcall GFGetSystemManager();    // 0x009A4EC0

struct CMaintainedEventList {                       // STLport list<CMaintainedEvent> (sentinel pointer)
    void* _M_node;
    void clear();                                   // 0x009E4710
};

struct CMouseDX {
    virtual void v000();
    unsigned char         _pad0004[0x340c - 0x04];  // CMouse: EventStore, NoStoredEvents, NoEventScansRunning
    CMaintainedEventList  MaintainedEventsList;     // +0x340c
    unsigned char         _pad3410[0x341c - 0x3410];// InputManager, Pos
    IDirectInputDevice8W* DMouse;                   // +0x341c
    CInputManagerDX*      InputManagerDX;           // +0x3420
    bool                  Acquired;                 // +0x3424
    unsigned char         _pad3425[0x343c - 0x3425];// MouseStep, NonExclusiveOld*
    bool                  ExclusiveMode;            // +0x343c
    void ResetNonExclusiveMouse();                  // 0x00AB5030
    virtual bool SetExclusiveMode(bool exclusive);
};

bool CMouseDX::SetExclusiveMode(bool exclusive)
{
    if (DMouse) {
        DMouse->Unacquire();
        DMouse->Release();
        DMouse = 0;
    }
    MaintainedEventsList.clear();
    if (exclusive) {
        ExclusiveMode = true;
        if (InputManagerDX->DInput->CreateDevice(GUID_SysMouse, &DMouse, 0) == 0) {
            if (DMouse->SetCooperativeLevel(GFGetSystemManager()->GetWindowHandle(), 5) == 0) {
                DIPROPDWORD bufferSize;
                bufferSize.diph.dwSize = sizeof(DIPROPDWORD);
                bufferSize.diph.dwHeaderSize = sizeof(DIPROPHEADER);
                bufferSize.diph.dwObj = 0;
                bufferSize.diph.dwHow = 0;
                bufferSize.dwData = 0x100;
                if (DMouse->SetProperty(DIPROP_BUFFERSIZE, &bufferSize.diph) == 0
                    && (DMouse->SetDataFormat(&c_dfDIMouse2) == 0 || DMouse->SetDataFormat(&c_dfDIMouse) == 0)
                    && DMouse->Acquire() == 0) {
                    Acquired = true;
                    return true;
                }
            }
        }
    } else {
        if (InputManagerDX->DInput->CreateDevice(GUID_SysMouse, &DMouse, 0) == 0) {
            if (DMouse->SetCooperativeLevel(GFGetSystemManager()->GetWindowHandle(), 10) == 0) {
                DIPROPDWORD bufferSize;
                bufferSize.diph.dwSize = sizeof(DIPROPDWORD);
                bufferSize.diph.dwHeaderSize = sizeof(DIPROPHEADER);
                bufferSize.diph.dwObj = 0;
                bufferSize.diph.dwHow = 0;
                bufferSize.dwData = 0x100;
                if (DMouse->SetProperty(DIPROP_BUFFERSIZE, &bufferSize.diph) == 0
                    && (DMouse->SetDataFormat(&c_dfDIMouse2) == 0 || DMouse->SetDataFormat(&c_dfDIMouse) == 0)
                    && DMouse->Acquire() == 0) {
                    Acquired = true;
                    ResetNonExclusiveMouse();
                    ExclusiveMode = false;
                    return true;
                }
            }
        }
    }
    return false;
}
