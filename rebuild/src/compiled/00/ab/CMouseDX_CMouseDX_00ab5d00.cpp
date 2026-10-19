// CMouseDX::CMouseDX(CInputManagerDX*, bool) -- DirectInput mouse: CMouse base, no device yet, unit mouse step, input enabled and cursor drawn, event buffers cleared, Windows cursor shown unless exclusive, engine cursor enabled, then SetExclusiveMode
// Leaf: ??0CMouseDX
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5D00, 188 bytes (`ret 8`). Vtable 0x0129EA14 (CMouseDX).
// Callees: CMouse::CMouse(CInputManagerDX*, bool) 0x00A66F20; CMouse::ClearStoredEvents 0x00A66B10 (zeroes NoStoredEvents;
// manifest label "CGameEventPackage::Clear" is a propagated byte-twin); USER32 ShowCursor (import 0x01440350);
// GFGetSystemManager 0x009A4EC0 (`__fastcall`); CMouseDX::SetExclusiveMode(bool) 0x00AB5710 (called directly: in the ctor).
// The engine cursor primitive is a COM-style (`__stdcall`, `this` pushed) interface; slot 12 (+0x30) enables the cursor.
// Layout (retail = PDB - 8 after CMouse, whose STLport list is the 4-byte sentinel pointer, not 12): CMouse 0x3414
// (NoStoredEvents +0x3404, NoEventScansRunning +0x3408); Pos +0x3414, DMouse +0x341c, InputManagerDX +0x3420,
// Acquired +0x3424, MouseStep +0x3428, the five NonExclusiveOldButton flags +0x342c, NonExclusiveOldMouseX/Y +0x3434,
// ExclusiveMode +0x343c, InputEnabled +0x343d, ForceRecentre +0x343e, DrawingCursor +0x343f,
// ExclusiveModeData +0x3440 (256 DIDEVICEOBJECTDATA), ExclusiveModeEventsCount +0x4840, NoExclusiveModeEvents +0x4844,
// LMBOn/MMBOn/RMBOn/Filtering +0x4848, LastUpdateMovement +0x484c, CursorGraphic +0x4858 (CCountedPointer<CSurface>).
// Shape: only InputManagerDX and the CursorGraphic counted pointer are initialisers (stored right after the vptr);
// the remaining fields are body assignments in retail's store order around the ClearStoredEvents call.
struct CInputManagerDX;
struct CSurface;

extern "C" __declspec(dllimport) int __stdcall ShowCursor(int show);   // USER32, IAT 0x01440350

struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };

struct DIDEVICEOBJECTDATA {                // dinput.h, DIRECTINPUT_VERSION 0x0800: 20 bytes
    unsigned long dwOfs;
    unsigned long dwData;
    unsigned long dwTimeStamp;
    unsigned long dwSequence;
    unsigned long uAppData;
};

struct IDirectInputDevice8W;

template <class T> struct CCountedPointer {   // 8 bytes: object + shared count block
    T*    Object;
    void* Count;
    CCountedPointer() : Object(0), Count(0) { }
};

struct CEngineCursorPrimitive {            // COM-style engine interface
    virtual void __stdcall v00();
    virtual void __stdcall v01();
    virtual void __stdcall v02();
    virtual void __stdcall v03();
    virtual void __stdcall v04();
    virtual void __stdcall v05();
    virtual void __stdcall v06();
    virtual void __stdcall v07();
    virtual void __stdcall v08();
    virtual void __stdcall v09();
    virtual void __stdcall v10();
    virtual void __stdcall v11();
    virtual void __stdcall SetCursorEnabled(int enabled);   // slot 12 (+0x30)
};

struct CEngineDisplay {
    unsigned char _pad[0x58];
    CEngineCursorPrimitive* PCursorPrimitive;   // +0x58
};

struct CSystemManager {
    unsigned char _pad[0x60];
    CEngineDisplay* PDisplay;              // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct CMouse {                            // vtable 0x0129DBC4, retail 0x3414 bytes
    virtual void v00();
    unsigned char _events[0x3400];         // +0x04 EventStore (256 CInputEvent)
    long          NoStoredEvents;          // +0x3404
    long          NoEventScansRunning;     // +0x3408
    void*         MaintainedEventsList;    // +0x340c (STLport list sentinel)
    long          InputManager;            // +0x3410
    CMouse(CInputManagerDX* manager, bool exclusiveMode);   // 0x00A66F20
    void ClearStoredEvents();              // 0x00A66B10
};

struct CMouseDX : CMouse {                 // vtable 0x0129EA14
    C2DVector              Pos;                            // +0x3414
    IDirectInputDevice8W*  DMouse;                         // +0x341c
    CInputManagerDX*       InputManagerDX;                 // +0x3420
    bool                   Acquired;                       // +0x3424
    float                  MouseStep;                      // +0x3428
    bool                   NonExclusiveOldButton0Pressed;  // +0x342c
    bool                   NonExclusiveOldButton1Pressed;  // +0x342d
    bool                   NonExclusiveOldButton2Pressed;  // +0x342e
    bool                   NonExclusiveOldButton3Pressed;  // +0x342f
    bool                   NonExclusiveOldButton4Pressed;  // +0x3430
    float                  NonExclusiveOldMouseX;          // +0x3434
    float                  NonExclusiveOldMouseY;          // +0x3438
    bool                   ExclusiveMode;                  // +0x343c
    bool                   InputEnabled;                   // +0x343d
    bool                   ForceRecentre;                  // +0x343e
    bool                   DrawingCursor;                  // +0x343f
    DIDEVICEOBJECTDATA     ExclusiveModeData[256];         // +0x3440
    long                   ExclusiveModeEventsCount;       // +0x4840
    long                   NoExclusiveModeEvents;          // +0x4844
    bool                   LMBOn;                          // +0x4848
    bool                   MMBOn;                          // +0x4849
    bool                   RMBOn;                          // +0x484a
    bool                   Filtering;                      // +0x484b
    C3DVector              LastUpdateMovement;             // +0x484c
    CCountedPointer<CSurface> CursorGraphic;               // +0x4858
    virtual void v00();
    CMouseDX(CInputManagerDX* manager, bool exclusiveMode);
    bool SetExclusiveMode(bool exclusiveMode);             // 0x00AB5710
};

CMouseDX::CMouseDX(CInputManagerDX* manager, bool exclusiveMode)
    : CMouse(manager, exclusiveMode)
    , InputManagerDX(manager)
{
    DMouse = 0;
    MouseStep = 1.0f;
    InputEnabled = true;
    NoEventScansRunning = 0;
    ClearStoredEvents();
    ForceRecentre = false;
    DrawingCursor = true;
    ExclusiveModeEventsCount = 0;
    NoExclusiveModeEvents = 0;
    LMBOn = false;
    MMBOn = false;
    RMBOn = false;
    Filtering = false;
    ShowCursor(!exclusiveMode);
    GFGetSystemManager()->PDisplay->PCursorPrimitive->SetCursorEnabled(1);
    SetExclusiveMode(exclusiveMode);
}