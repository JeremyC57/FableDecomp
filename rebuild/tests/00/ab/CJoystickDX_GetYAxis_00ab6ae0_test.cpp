// Behaviour test for CJoystickDX::GetYAxis (0x00AB6AE0): the float view of the retail word +0xd1c is returned.
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f_2` -> `YAxis`.
#include "engine/CJoystickDX.h"
#include <stdio.h>
struct CJoystickDX_Methods : CJoystickDX {
    float GetYAxis();
};
float CJoystickDX_Methods::GetYAxis() { return *(const float*)&this->YAxis; }
int main()
{
    CJoystickDX_Methods obj;
    *(float*)&obj.YAxis = 2.5f;
    if (obj.GetYAxis() == 2.5f && (unsigned char*)&obj.YAxis - (unsigned char*)&obj == 0xd1c) {
        printf("00ab6ae0_TEST PASS\n");
        return 0;
    }
    printf("FAIL %f\n", obj.GetYAxis());
    return 1;
}
