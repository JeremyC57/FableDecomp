// CMouseDX::~CMouseDX() -- DirectInput mouse destructor: restore the CMouseDX vtable, unacquire and release the DirectInput mouse device, release the cursor graphic counted pointer, then the CMouse base
// Leaf: ??1CMouseDX@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5320, 109 bytes, thiscall, tail `jmp ??1CMouse@@UAE@XZ` (0x00A66F80).
// Identity (proven): stores vtable 0x0129EA14 first (RTTI .?AVCMouseDX@@ : CMouse : CBaseClassNonCopyable : CBaseClass); sole caller
//   is that vtable's slot-0 scalar deleting destructor 0x00AB5FA0 (call at 0x00AB5FA3); Ego_r.pdb ??1CMouseDX@@UAE@XZ. The tail
//   target 0x00A66F80 stores CMouse's vtable 0x0129DBC4 and is also called by ??_GCMouse 0x00A673B0 = Ego_r ??1CMouse@@UAE@XZ
//   (catalog "CGuiControlTreePane_Destructor"). The catalog label of this row is right.
// Callees: IDirectInputDevice8W::Unacquire (slot 8) and Release (slot 2), the CCPPointerInfo deleter (fastcall), operator delete
//   0x00BFE9BC (count block), ~CMouse 0x00A66F80.
// Layout: Ego_r CMouseDX over the retail CMouse (engine/CMouse.h: STLport sizeof 0x3414 -- the Ego_r offsets less 8 after the
//   4-byte MaintainedEventsList, as the landed SetExclusiveMode 0x00AB5710 / CMouseDX 0x00AB5D00 models): DMouse +0x341C,
//   CursorGraphic (CCountedPointer<CSurface>) +0x4858 (Ego_r +0x4860). engine/CMouse.h is the flattened donor layout with a
//   `void* __vftable` word and byte-array members, so CMouse is re-declared over CBaseClassNonCopyable with its member sizes.
// Shape: the user destructor `if (DMouse) { DMouse->Unacquire(); DMouse->Release(); }` (no zero store), then the CursorGraphic
//   member's ~CCountedPointer (engine/CCountedPointer.h; no shared zero register here: `cmp [eax],0` and immediate stores).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
struct IDirectInputDevice8W {
    virtual void __stdcall QueryInterface();
    virtual void __stdcall AddRef();
    virtual unsigned long __stdcall Release();                                          // slot 2 (+0x08)
    virtual void __stdcall GetCapabilities();
    virtual void __stdcall EnumObjects();
    virtual void __stdcall GetProperty();
    virtual void __stdcall SetProperty();
    virtual long __stdcall Acquire();                                                  // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();                                                // slot 8 (+0x20)
};
struct DIDEVICEOBJECTDATA {                                  // DirectInput 8: 20 bytes
    unsigned long dwOfs, dwData, dwTimeStamp, dwSequence, uAppData;
};
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: unsigned char Data[0x34]; };     // 0x34 bytes (CMouse::EventStore[256] = 0x3400)
class CInputManager;
class CInputManagerDX;
class CSurface;
class CMouse : public CBaseClassNonCopyable {
public:
    virtual ~CMouse();                                       // 0x00A66F80 ??1CMouse@@UAE@XZ
    CInputEvent    EventStore[256];                          // +0x0004
    long           NoStoredEvents;                           // +0x3404
    long           NoEventScansRunning;                      // +0x3408
    void*          MaintainedEventsList;                     // +0x340c list<CMaintainedEvent> (STLport, 4 bytes)
    CInputManager* InputManager;                             // +0x3410
};
class CMouseDX : public CMouse {
public:
    virtual ~CMouseDX();
    C2DVector                 Pos;                           // +0x3414
    IDirectInputDevice8W*     DMouse;                        // +0x341c
    CInputManagerDX*          InputManagerDX;                // +0x3420
    bool                      Acquired;                      // +0x3424
    float                     MouseStep;                     // +0x3428
    bool                      NonExclusiveOldButton0Pressed; // +0x342c
    bool                      NonExclusiveOldButton1Pressed; // +0x342d
    bool                      NonExclusiveOldButton2Pressed; // +0x342e
    bool                      NonExclusiveOldButton3Pressed; // +0x342f
    bool                      NonExclusiveOldButton4Pressed; // +0x3430
    float                     NonExclusiveOldMouseX;         // +0x3434
    float                     NonExclusiveOldMouseY;         // +0x3438
    bool                      ExclusiveMode;                 // +0x343c
    bool                      InputEnabled;                  // +0x343d
    bool                      ForceRecentre;                 // +0x343e
    bool                      DrawingCursor;                 // +0x343f
    DIDEVICEOBJECTDATA        ExclusiveModeData[256];        // +0x3440
    long                      ExclusiveModeEventsCount;      // +0x4840
    long                      NoExclusiveModeEvents;         // +0x4844
    bool                      LMBOn;                         // +0x4848
    bool                      MMBOn;                         // +0x4849
    bool                      RMBOn;                         // +0x484a
    bool                      Filtering;                     // +0x484b
    C3DVector                 LastUpdateMovement;            // +0x484c
    CCountedPointer<CSurface> CursorGraphic;                 // +0x4858
};
CMouseDX::~CMouseDX()
{
    if (DMouse) {
        DMouse->Unacquire();
        DMouse->Release();
    }
}