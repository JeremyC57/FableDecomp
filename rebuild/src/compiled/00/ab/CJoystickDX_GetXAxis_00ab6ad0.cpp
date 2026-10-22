// CJoystickDX::GetXAxis() -- return the axis value stored in the retail word +0xd18 (engine/CJoystickDX.h types it as the retail-only long `f`; it is a float: retail `fld dword ptr [ecx+0d18h]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6AD0, 7 bytes, plain `ret`.
// Layout: the header's long `f` at +0xd18 is read through a float view (a long member returned as float would emit fild).
#include "engine/CJoystickDX.h"
struct CJoystickDX_Methods : CJoystickDX {
    float GetXAxis();
};
float CJoystickDX_Methods::GetXAxis() { return *(const float*)&this->f; }
