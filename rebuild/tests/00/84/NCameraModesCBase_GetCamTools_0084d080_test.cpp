// Test for 0x0084D080 NCameraModes::CBase::GetCamTools: reaches the camera manager member at its retail offset.
#include <stdio.h>
class CWorld;
class CCamera;

namespace NCameraModes {
class CTools;
}

class CGameCameraManager {                     // only the members the accessors reach (PC offsets = Ego_r)
public:
    unsigned char         _pad00[0x04];
    unsigned char         Camera[0x38];          // +0x04 CCamera
    unsigned char         OldCamera[0x38];       // +0x3C CCamera
    CWorld*               World;                 // +0x74
    unsigned char         CamTools[0x24];        // +0x78 NCameraModes::CTools
};

int main();
namespace NCameraModes {

class CBase {
public:
    unsigned char         _pad00[0x40];
    CGameCameraManager*   CameraManager;         // +0x40 (Ego_r +0x50)
protected:
    const CTools& GetCamTools() const;
    friend int ::main();
};

}

int main()
{
    CGameCameraManager m; m.World = (CWorld*)&m.OldCamera[0];
    NCameraModes::CBase b; b.CameraManager = &m;
    const NCameraModes::CBase& rb = b;
    const void* got = &rb.GetCamTools();
    const void* want = &m.CamTools[0];
    if (got != want) { printf("0084d080_TEST FAIL\n"); return 1; }
    printf("0084d080_TEST PASS\n");
    return 0;
}
