// Behaviour test for CJoystickDX::GetXAxis (0x00AB6AD0): the float view of the retail word +0xd18 is returned.
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `XAxis`.
#include "engine/CJoystickDX.h"
#include <stdio.h>
struct CJoystickDX_Methods : CJoystickDX {
    float GetXAxis();
};
float CJoystickDX_Methods::GetXAxis() { return *(const float*)&this->XAxis; }
int main()
{
    CJoystickDX_Methods obj;
    *(float*)&obj.XAxis = 2.5f;
    if (obj.GetXAxis() == 2.5f && (unsigned char*)&obj.XAxis - (unsigned char*)&obj == 0xd18) {
        printf("00ab6ad0_TEST PASS\n");
        return 0;
    }
    printf("FAIL %f\n", obj.GetXAxis());
    return 1;
}
