// CJoystickDX::SetAxes(const C2DVector& axes) -- store the two axis values (retail +0xd18 / +0xd1c, declared as the header's retail-only longs) by a plain dword copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB78E0, 24 bytes, `ret 4`.
// Layout: engine/CJoystickDX.h types the two retail-only words `f` / `f_2` as long; retail copies them as dwords
//         (`mov edx,[eax] ; mov [ecx+0D18h],edx`), so the float axes are written through float views of those words
//         (a float-to-long assignment would emit an _ftol call).
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `XAxis`, `f_2` -> `YAxis`.
#include "engine/CJoystickDX.h"
class C2DVector { public: float x; float y; };

void __fastcall CJoystickDX_SetAxes(CJoystickDX* joystickDX, int /*edx*/, const C2DVector& v)
{
    *(float*)&joystickDX->XAxis = v.x;
    *(float*)&joystickDX->YAxis = v.y;
}
