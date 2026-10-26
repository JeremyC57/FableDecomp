// CJoystickDX::GetYAxis() -- return the axis value stored in the retail word +0xd1c (engine/CJoystickDX.h types it as the retail-only long `f_2`; it is a float: retail `fld dword ptr [ecx+0d1ch]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6AE0, 7 bytes, plain `ret`.
// Layout: the header's long `f_2` at +0xd1c is read through a float view (a long member returned as float would emit fild).
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f_2` -> `YAxis`.
#include "engine/CJoystickDX.h"
struct CJoystickDX_Methods : CJoystickDX {
    float GetYAxis();
};
float CJoystickDX_Methods::GetYAxis() { return *(const float*)&this->YAxis; }
