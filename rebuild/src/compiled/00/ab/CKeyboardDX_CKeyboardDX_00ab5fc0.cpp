// CKeyboardDX::~CKeyboardDX() -- DirectInput keyboard dtor: unacquire and release the DirectInput keyboard device, then ~CKeyboard
// Leaf: ??1CKeyboardDX@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5FC0.
// Identity: Ego_r.pdb ??1CKeyboardDX@@UAE@XZ; members from +0x3414, sizeof 0x3614 (retail).
// Callees: IDirectInputDevice8W::Unacquire (slot 8) and Release (slot 2), ~CKeyboard (FABLE_RETAIL_CKEYBOARD).
// Shape: `DKeyboard->Unacquire(); DKeyboard->Release();`.
#include "stlport/fable_containers.h"
#include "engine/CBaseClass.h"
#include "engine/CInputEvent.h"
class CMaintainedEvent : public CBaseClass {                 // RTTI CMaintainedEvent : CBaseClass (0x38 bytes)
public:
    CInputEvent Event;                                       // +0x04
};
#define FABLE_RETAIL_CKEYBOARD
#include "engine/CKeyboard.h"

struct IUnknown {                                            // <unknwn.h>
    virtual long __stdcall QueryInterface(const struct _GUID& iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;           // slot 2 (+0x08)
};
struct DIDEVCAPS; struct DIDEVICEOBJECTINSTANCEW; struct DIPROPHEADER;
struct IDirectInputDevice8W : public IUnknown {              // <dinput.h> DIRECTINPUT_VERSION 0x0800, first slots
    virtual long __stdcall GetCapabilities(DIDEVCAPS* caps) = 0;                                         // 3
    virtual long __stdcall EnumObjects(int (__stdcall* callback)(const DIDEVICEOBJECTINSTANCEW*, void*), void* ref, unsigned long flags) = 0; // 4
    virtual long __stdcall GetProperty(const struct _GUID& prop, DIPROPHEADER* header) = 0;             // 5
    virtual long __stdcall SetProperty(const struct _GUID& prop, const DIPROPHEADER* header) = 0;       // 6
    virtual long __stdcall Acquire() = 0;                    // 7 (+0x1C)
    virtual long __stdcall Unacquire() = 0;                  // 8 (+0x20)
};

class CInputManagerDX;

class CKeyboardDX : public CKeyboard {                       // vtable 0x0129EA60
public:
    CKeyboardDX(CInputManagerDX&, bool);
    virtual ~CKeyboardDX();
    virtual bool Update();
    virtual bool SetExclusiveMode(bool);
    virtual void SetHasInputFocus(bool);
    virtual bool GetKeyName(EInputKey, CWideString&) const;
private:
    CInputManagerDX*      InputManagerDX;                    // +0x3414
    IDirectInputDevice8W* DKeyboard;                         // +0x3418
    signed char           KeyStates[256];                    // +0x341c
    unsigned short        InputKeyToDXKey[122];              // +0x351c
    bool                  Acquired;                          // +0x3610
};
FABLE_STATIC_ASSERT(sizeof(CKeyboardDX) == 0x3614);

CKeyboardDX::~CKeyboardDX()
{
    DKeyboard->Unacquire();
    DKeyboard->Release();
}