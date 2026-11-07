// CMouseDX::Update() -- drain the DirectInput mouse events into the input store (summing the relative movement), post one (optionally smoothed) mouse-movement event, run the maintained events, recentre the Windows cursor when asked, and move the engine cursor to the mouse position
// Leaf: ?Update@CMouseDX@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB58E0, 1056 bytes (code + the 8-entry switch table at 0x00AB5CE0), `ret`. CMouseDX vtable
//   0x0129EA14 slot 5; Ego_r.pdb / FableWin.pdb ?Update@CMouseDX@@UAEXXZ.
// Callees: CInputEvent() 0x00A04410 / ~CInputEvent 0x00A03B30, CMouse::EmptyEventStore 0x00A66B10 (bound before as
//   "ClearStoredEvents", no such PDB name), GetExclusiveModeDeviceMouseEvent 0x00AB4910 / GetNonExclusiveModeDeviceMouseEvent
//   0x00AB4BB0, UpdateCurrentStatusFromEvent 0x00AB5190, ConvertMouseEventToInputEvent 0x00AB5420 (all CMouseDX, protected),
//   CMouse::AddEventToStore 0x00A66B20, ProcessMaintainedEventsFromEvent 0x00A66FD0, UpdateMaintainedEvents 0x00A66C20,
//   AddMaintainedEventsToEventStore 0x00A66ED0 (catalogued as CKeyboard's; it sits in the CMouse block), GetEventFromStore
//   0x00A66B60, CInputEvent::SetAsMouseMovement(const C2DVector&, const C3DVector&, double) 0x00A03FB0, CInputEvent::GetType
//   0x00A03B40, the virtual PeekPos (slot 2) / PeekScreenPos (slot 3), GFGetSystemManager 0x009A4EC0 (__fastcall),
//   CSurface::GetDimensions 0x009F2F90 (the display manager's back buffer, +0x60 then +0x2c), the cursor primitive's slot 11
//   (+0x2c, `__stdcall` COM interface at display manager +0x58), user32 ClientToScreen / GetClientRect / OffsetRect /
//   SetCursorPos (IAT 0x01440304 / 0x01440338 / 0x014403A4 / 0x014403A0), __ftol2 0x00BFEA70.
// Constants: 0.001f 0x01266300 (the movement threshold), 0.0f 0x0122DEDC, 0.5f 0x0122F59C.
// Layout: the landed CMouseDX::CMouseDX 0x00AB5D00 model (retail = PDB - 8 after CMouse's 4-byte STLport list): Pos +0x3414,
//   NonExclusiveOldMouseX/Y +0x3434, ExclusiveMode +0x343c, InputEnabled +0x343d, ForceRecentre +0x343e, Filtering +0x484b,
//   LastUpdateMovement +0x484c; CMouseEvent (Ego_r, 0x18): Time, Type, Movement, PosX, PosY; CInputEvent 0x34 bytes.
//   EMouseEventType enumerator names are descriptive (the switch adds types 7/12 to X, 8/13 to Y, 9/14 to Z).
// Shape: the device event, the read loop and the movement post sit in an inner block so the recentre RECT reuses the event's
//   slot (frame 0x6c); the two C3DVectors stay at function scope (VC7.1 still packs the extents / screen vector onto them).
//   ExclusiveMode / InputEnabled / the fetch result compare `== true` (retail `cmp ...,1`); fabs(movement.X) is shared by the
//   threshold test and the filter test (`fcom`, popped on the other paths). The recentre block takes the system manager into a
//   named local before GetDimensions (else the hidden result is pushed before the GFGetSystemManager call) and builds the
//   cursor position in a POINT (x before y, as retail; plain or named-long arguments compute y first). The store scan loop
//   only calls GetType (its result is unused in retail).
struct HWND__;
struct POINT { long x; long y; };
struct RECT { long left; long top; long right; long bottom; };
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(HWND__* window, POINT* point);   // IAT 0x01440304
extern "C" __declspec(dllimport) int __stdcall GetClientRect(HWND__* window, RECT* rect);      // IAT 0x01440338
extern "C" __declspec(dllimport) int __stdcall OffsetRect(RECT* rect, int dx, int dy);         // IAT 0x014403A4
extern "C" __declspec(dllimport) int __stdcall SetCursorPos(int x, int y);                     // IAT 0x014403A0
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

#include "engine/C3DVector.h"
class C2DVector { public:
    float X, Y;
};
class C2DExtentsI { public:
    long X, Y;
};
class CSurface { public:
    C2DExtentsI GetDimensions() const;              // 0x009F2F90
};
struct CEngineCursorPrimitive {                     // COM-style engine interface (`__stdcall`, this pushed)
    virtual void __stdcall v00(); virtual void __stdcall v01(); virtual void __stdcall v02(); virtual void __stdcall v03();
    virtual void __stdcall v04(); virtual void __stdcall v05(); virtual void __stdcall v06(); virtual void __stdcall v07();
    virtual void __stdcall v08(); virtual void __stdcall v09(); virtual void __stdcall v10();
    virtual void __stdcall SetCursorPosition(int x, int y, int flags);   // slot 11 (+0x2c)
    virtual void __stdcall SetCursorEnabled(int enabled);                // slot 12 (+0x30)
};
class CDisplayManager { public:
    unsigned char           _pad00[0x2c];
    CSurface                BackBuffer;             // +0x2c
    unsigned char           _pad2d[0x58 - 0x2d];
    CEngineCursorPrimitive* PCursorPrimitive;       // +0x58
};
class CSystemManager { public:
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                // +0x60
    unsigned char    _pad64[0x94 - 0x64];
    HWND__*          ApplicationWinHandle;          // +0x94
};
CSystemManager* __fastcall GFGetSystemManager();    // 0x009A4EC0

enum EInputEventType { };
class CInputEvent { public:                         // 0x34 bytes
    unsigned char Data[0x20];
    long          DeviceType;
    long          DeviceNumber;
    long          Type;
    float         CurrentTime;
    float         StartTime;
    CInputEvent();                                  // 0x00A04410
    ~CInputEvent();                                 // 0x00A03B30
    EInputEventType GetType() const;                // 0x00A03B40
    void SetAsMouseMovement(const C2DVector& pos, const C3DVector& movement, double time);   // 0x00A03FB0
};

enum EMouseEventType {                              // descriptive enumerator names (values from the switch)
    MOUSE_EVENT_MOVE_X = 7, MOUSE_EVENT_MOVE_Y = 8, MOUSE_EVENT_MOVE_Z = 9,
    MOUSE_EVENT_MOVE_X2 = 12, MOUSE_EVENT_MOVE_Y2 = 13, MOUSE_EVENT_MOVE_Z2 = 14
};
class CMouseEvent { public:                         // Ego_r.pdb, 0x18 bytes
    double          Time;                           // +0x00
    EMouseEventType Type;                           // +0x08
    float           Movement;                       // +0x0c
    float           PosX;                           // +0x10
    float           PosY;                           // +0x14
};

class CMouse { public:                              // vtable 0x0129DBC4, retail 0x3414 bytes
    virtual ~CMouse();
    virtual void GetPos(C2DVector* pos) const = 0;                 // slot 1
    virtual const C2DVector& PeekPos() const = 0;                  // slot 2
    virtual C2DVector PeekScreenPos() const = 0;                   // slot 3
    virtual void SetPos(const C2DVector* pos) = 0;                 // slot 4
    virtual void Update() = 0;                                     // slot 5
    CInputEvent   EventStore[256];                  // +0x04
    long          NoStoredEvents;                   // +0x3404
    long          NoEventScansRunning;              // +0x3408
    void*         MaintainedEventsList;             // +0x340c (STLport list sentinel)
    void*         InputManager;                     // +0x3410
protected:
    void EmptyEventStore();                         // 0x00A66B10
    bool AddEventToStore(const CInputEvent* event); // 0x00A66B20
    const CInputEvent* GetEventFromStore(unsigned long index) const;   // 0x00A66B60
    void UpdateMaintainedEvents();                  // 0x00A66C20
    void AddMaintainedEventsToEventStore();         // 0x00A66ED0
    void ProcessMaintainedEventsFromEvent(const CInputEvent* event);   // 0x00A66FD0
};

class CMouseDX : public CMouse { public:            // vtable 0x0129EA14
    C2DVector             Pos;                            // +0x3414
    void*                 DMouse;                         // +0x341c
    void*                 InputManagerDX;                 // +0x3420
    bool                  Acquired;                       // +0x3424
    float                 MouseStep;                      // +0x3428
    bool                  NonExclusiveOldButtonPressed[5];// +0x342c
    float                 NonExclusiveOldMouseX;          // +0x3434
    float                 NonExclusiveOldMouseY;          // +0x3438
    bool                  ExclusiveMode;                  // +0x343c
    bool                  InputEnabled;                   // +0x343d
    bool                  ForceRecentre;                  // +0x343e
    bool                  DrawingCursor;                  // +0x343f
    unsigned char         ExclusiveModeData[256 * 20];    // +0x3440
    long                  ExclusiveModeEventsCount;       // +0x4840
    long                  NoExclusiveModeEvents;          // +0x4844
    bool                  LMBOn;                          // +0x4848
    bool                  MMBOn;                          // +0x4849
    bool                  RMBOn;                          // +0x484a
    bool                  Filtering;                      // +0x484b
    C3DVector             LastUpdateMovement;             // +0x484c
    void*                 CursorGraphic[2];               // +0x4858
    virtual ~CMouseDX();
    virtual void GetPos(C2DVector* pos) const;
    virtual const C2DVector& PeekPos() const;
    virtual C2DVector PeekScreenPos() const;
    virtual void SetPos(const C2DVector* pos);
    virtual void Update();
protected:
    bool GetExclusiveModeDeviceMouseEvent(CMouseEvent* event);      // 0x00AB4910
    bool GetNonExclusiveModeDeviceMouseEvent(CMouseEvent* event);   // 0x00AB4BB0
    void UpdateCurrentStatusFromEvent(const CMouseEvent* event);    // 0x00AB5190
    bool ConvertMouseEventToInputEvent(const CMouseEvent* mouseEvent, CInputEvent* inputEvent);   // 0x00AB5420
};

void CMouseDX::Update()
{
    CInputEvent event;
    C3DVector movement(0.0f, 0.0f, 0.0f);
    C3DVector filtered(0.0f, 0.0f, 0.0f);
    EmptyEventStore();
    {
    CMouseEvent mouseEvent;
    for (;;) {
        bool got;
        if (ExclusiveMode == true)
            got = GetExclusiveModeDeviceMouseEvent(&mouseEvent);
        else
            got = GetNonExclusiveModeDeviceMouseEvent(&mouseEvent);
        if (got != true)
            break;
        if (InputEnabled == true) {
            UpdateCurrentStatusFromEvent(&mouseEvent);
            switch (mouseEvent.Type) {
            case MOUSE_EVENT_MOVE_X:
            case MOUSE_EVENT_MOVE_X2:
                movement.X += mouseEvent.Movement;
                break;
            case MOUSE_EVENT_MOVE_Y:
            case MOUSE_EVENT_MOVE_Y2:
                movement.Y += mouseEvent.Movement;
                break;
            case MOUSE_EVENT_MOVE_Z:
            case MOUSE_EVENT_MOVE_Z2:
                movement.Z += mouseEvent.Movement;
                break;
            }
            if (ConvertMouseEventToInputEvent(&mouseEvent, &event)) {
                AddEventToStore(&event);
                ProcessMaintainedEventsFromEvent(&event);
            }
        }
    }
    if (fabs(movement.X) > 0.001f || fabs(movement.Y) > 0.001f || fabs(movement.Z) > 0.001f) {
        if (Filtering) {
            if (fabs(movement.X) > 0.0f && fabs(LastUpdateMovement.X) > 0.0f)
                filtered.X = (movement.X + LastUpdateMovement.X) * 0.5f;
            else
                filtered.X = movement.X;
            if (fabs(movement.Y) > 0.0f && fabs(LastUpdateMovement.Y) > 0.0f)
                filtered.Y = (movement.Y + LastUpdateMovement.Y) * 0.5f;
            else
                filtered.Y = movement.Y;
            if (fabs(movement.Z) > 0.0f && fabs(LastUpdateMovement.Z) > 0.0f)
                filtered.Z = (movement.Z + LastUpdateMovement.Z) * 0.5f;
            else
                filtered.Z = movement.Z;
        } else {
            filtered = movement;
        }
        LastUpdateMovement = movement;
        event.SetAsMouseMovement(PeekPos(), filtered, mouseEvent.Time);
        AddEventToStore(&event);
    }
    }
    if (InputEnabled) {
        UpdateMaintainedEvents();
        AddMaintainedEventsToEventStore();
    }
    if (ForceRecentre) {
        HWND__* window = GFGetSystemManager()->ApplicationWinHandle;
        POINT origin;
        origin.x = 0;
        origin.y = 0;
        ClientToScreen(window, &origin);
        RECT client;
        GetClientRect(window, &client);
        OffsetRect(&client, origin.x, origin.y);
        CSystemManager* system = GFGetSystemManager();
        C2DExtentsI extents = system->DisplayManager->BackBuffer.GetDimensions();
        POINT centre;
        centre.x = extents.X / 2 + client.left;
        centre.y = extents.Y / 2 + client.top;
        SetCursorPos(centre.x, centre.y);
        Pos.X = (float)(extents.X / 2);
        Pos.Y = (float)(extents.Y / 2);
        NonExclusiveOldMouseX = Pos.X;
        NonExclusiveOldMouseY = Pos.Y;
    }
    for (long i = 0; i < NoStoredEvents; ++i)
        GetEventFromStore(i)->GetType();
    if (ExclusiveMode) {
        CEngineCursorPrimitive* cursor = GFGetSystemManager()->DisplayManager->PCursorPrimitive;
        cursor->SetCursorPosition((int)Pos.X, (int)Pos.Y, 1);
    } else {
        C2DVector screen = PeekScreenPos();
        CEngineCursorPrimitive* cursor = GFGetSystemManager()->DisplayManager->PCursorPrimitive;
        cursor->SetCursorPosition((int)screen.X, (int)screen.Y, 1);
    }
}
