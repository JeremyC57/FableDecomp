// Standalone behaviour test for GetControlAngleXY @ 0x00697700
// Compiles with cl /O2 ; prints the PassPattern on success.
#include <stdio.h>

class CGameCameraManagerLayout
{ public:
    char _pad0[0xB8];
    float m_controlAngleXY;
};

// NF335 (NF324 Q2): the row now defines the PDB member CGameCameraManager::GetControlAngleXY
// (?GetControlAngleXY@CGameCameraManager@@UBEMXZ).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CGameCameraManager { public:
    virtual float GetControlAngleXY() const;
};

int main(void)
{
    CGameCameraManagerLayout cam;
    // Sanity: the accessed member must be at byte offset 0xB8.
    if ((char*)&cam.m_controlAngleXY - (char*)&cam != 0xB8)
    {
        printf("OFFSET FAIL\n");
        return 1;
    }

    cam.m_controlAngleXY = 1.5f;
    float a = ((CGameCameraManager*)(&cam))->CGameCameraManager::GetControlAngleXY();

    cam.m_controlAngleXY = -0.25f;
    float b = ((CGameCameraManager*)(&cam))->CGameCameraManager::GetControlAngleXY();

    if (a == 1.5f && b == -0.25f)
        printf("CGAMECAMERAMANAGER_00697700_TEST PASS\n");
    else
        printf("TEST FAIL a=%f b=%f\n", a, b);
    return 0;
}
