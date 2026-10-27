// Behaviour test for CTCLook::GetLookAngleRelToBodyXY (0x00751A80): head angle relative to body plus eye angle relative to head (XY).
#include <cstdio>
class CTCLook { public:
    float HeadAngleRelToBodyXY, HeadAngleRelToBodyYZ, EyeAnglesRelToHeadXY, EyeAnglesRelToHeadYZ;
    float GetLookAngleRelToBodyXY() const { return HeadAngleRelToBodyXY + EyeAnglesRelToHeadXY; }
};
int main()
{
    CTCLook l; l.HeadAngleRelToBodyXY = 0.25f; l.HeadAngleRelToBodyYZ = 9.0f; l.EyeAnglesRelToHeadXY = -1.5f; l.EyeAnglesRelToHeadYZ = 7.0f;
    if (l.GetLookAngleRelToBodyXY() != -1.25f) { std::printf("00751a80_TEST FAIL\n"); return 1; }
    std::printf("00751a80_TEST PASS\n");
    return 0;
}