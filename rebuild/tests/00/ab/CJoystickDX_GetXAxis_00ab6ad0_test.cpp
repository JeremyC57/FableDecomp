// Behaviour test for CJoystickDX::GetXAxis (0x00AB6AD0): the float view of the retail word +0xd18 is returned.
#include "engine/CJoystickDX.h"
#include <stdio.h>
struct CJoystickDX_Methods : CJoystickDX {
    float GetXAxis();
};
float CJoystickDX_Methods::GetXAxis() { return *(const float*)&this->f; }
int main()
{
    CJoystickDX_Methods obj;
    *(float*)&obj.f = 2.5f;
    if (obj.GetXAxis() == 2.5f && (unsigned char*)&obj.f - (unsigned char*)&obj == 0xd18) {
        printf("00ab6ad0_TEST PASS\n");
        return 0;
    }
    printf("FAIL %f\n", obj.GetXAxis());
    return 1;
}
