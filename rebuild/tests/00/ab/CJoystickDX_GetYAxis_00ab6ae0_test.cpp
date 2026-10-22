// Behaviour test for CJoystickDX::GetYAxis (0x00AB6AE0): the float view of the retail word +0xd1c is returned.
#include "engine/CJoystickDX.h"
#include <stdio.h>
struct CJoystickDX_Methods : CJoystickDX {
    float GetYAxis();
};
float CJoystickDX_Methods::GetYAxis() { return *(const float*)&this->f_2; }
int main()
{
    CJoystickDX_Methods obj;
    *(float*)&obj.f_2 = 2.5f;
    if (obj.GetYAxis() == 2.5f && (unsigned char*)&obj.f_2 - (unsigned char*)&obj == 0xd1c) {
        printf("00ab6ae0_TEST PASS\n");
        return 0;
    }
    printf("FAIL %f\n", obj.GetYAxis());
    return 1;
}
