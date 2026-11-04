// CJoystickDX::~CJoystickDX() -- DirectInput joystick destructor: unacquire and release the DirectInput device and forget it, then the CJoystick base
// Leaf: ??1CJoystickDX@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB6A90, 55 bytes, thiscall, tail `jmp ??1CJoystick@@UAE@XZ` (0x009E4420).
// Identity (two sources): stores vtable 0x0129EA7C first (RTTI .?AVCJoystickDX@@ : CJoystick), Ego_r.pdb ??1CJoystickDX@@UAE@XZ;
//   the body is the user part the landed ??_GCJoystickDX 0x00AB7950 inlines (same device test, Unacquire slot 8, Release slot 2,
//   zero store), and the tail target 0x009E4420 is ~CJoystick (Ego_r ??1CJoystick@@UAE@XZ; landed caller binding).
// Shape: `if (DJoystick) { DJoystick->Unacquire(); DJoystick->Release(); DJoystick = 0; }`; VC7.1 loads DJoystick before the vptr store.
// Callees: IDirectInputDevice8W::Unacquire (slot 8) / Release (slot 2), ??1CJoystick@@UAE@XZ 0x009E4420.
// Header: engine/CJoystickDX.h (FABLE_RETAIL_CJOYSTICKDX, new), engine/CJoystick.h (FABLE_RETAIL_CJOYSTICK, new).
#define FABLE_RETAIL_CJOYSTICKDX
#include "engine/CJoystickDX.h"

CJoystickDX::~CJoystickDX()
{
    if (DJoystick) {
        DJoystick->Unacquire();
        DJoystick->Release();
        DJoystick = 0;
    }
}