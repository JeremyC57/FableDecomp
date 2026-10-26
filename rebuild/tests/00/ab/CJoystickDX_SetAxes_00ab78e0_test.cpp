// Behaviour test for CJoystickDX::SetAxes (0x00AB78E0): the two axis floats land in the retail words +0xd18 / +0xd1c.
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `XAxis`, `f_2` -> `YAxis`.
#include "engine/CJoystickDX.h"
#include <stdio.h>

class C2DVector { public: float x; float y; };

void __fastcall CJoystickDX_SetAxes(CJoystickDX* joystickDX, int /*edx*/, const C2DVector& v)
{
    *(float*)&joystickDX->XAxis = v.x;
    *(float*)&joystickDX->YAxis = v.y;
}

int main()
{
    CJoystickDX obj;
    *(float*)&obj.XAxis = -1.0f;
    *(float*)&obj.YAxis = -1.0f;
    C2DVector v;
    v.x = 3.5f;
    v.y = 7.25f;
    CJoystickDX_SetAxes(&obj, 0, v);
    if (*(float*)&obj.XAxis == 3.5f && *(float*)&obj.YAxis == 7.25f
        && (unsigned char*)&obj.XAxis - (unsigned char*)&obj == 0xd18 && (unsigned char*)&obj.YAxis - (unsigned char*)&obj == 0xd1c) {
        printf("00ab78e0_TEST PASS\n");
    } else {
        printf("FAIL %f %f\n", *(float*)&obj.XAxis, *(float*)&obj.YAxis);
    }
    return 0;
}
