// Behaviour test for CJoystickDX::SetAxes (0x00AB78E0): the two axis floats land in the retail words +0xd18 / +0xd1c.
#include "engine/CJoystickDX.h"
#include <stdio.h>

class C2DVector { public: float x; float y; };

void __fastcall CJoystickDX_SetAxes(CJoystickDX* joystickDX, int /*edx*/, const C2DVector& v)
{
    *(float*)&joystickDX->f = v.x;
    *(float*)&joystickDX->f_2 = v.y;
}

int main()
{
    CJoystickDX obj;
    *(float*)&obj.f = -1.0f;
    *(float*)&obj.f_2 = -1.0f;
    C2DVector v;
    v.x = 3.5f;
    v.y = 7.25f;
    CJoystickDX_SetAxes(&obj, 0, v);
    if (*(float*)&obj.f == 3.5f && *(float*)&obj.f_2 == 7.25f
        && (unsigned char*)&obj.f - (unsigned char*)&obj == 0xd18 && (unsigned char*)&obj.f_2 - (unsigned char*)&obj == 0xd1c) {
        printf("00ab78e0_TEST PASS\n");
    } else {
        printf("FAIL %f %f\n", *(float*)&obj.f, *(float*)&obj.f_2);
    }
    return 0;
}
