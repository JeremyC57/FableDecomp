// Behaviour test for CGameCameraManager::GetAngleXY 0x0069E1B0: the manager's AngleXY angle is its camera's. The body forwards to the member at +0x4
// (`add ecx,4h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CCamera { float XY, YZ; float GetAngleXY() const { return XY; } float GetAngleYZ() const { return YZ; } };
#pragma pack(push, 4)
struct CGameCameraManager {                                  // retail is 32-bit: 4-byte packing keeps the +0x4 offset on a 64-bit host
    unsigned char Before[0x4];
    CCamera Camera;
    float GetAngleXY() const { return Camera.GetAngleXY(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CGameCameraManager, Camera) != 0x4u) { std::printf("0069e1b0_TEST FAIL\n"); return 1; }
    CGameCameraManager m; m.Camera.XY = 1.5f; m.Camera.YZ = -0.25f;
    if (m.GetAngleXY() != m.Camera.GetAngleXY()) { std::printf("0069e1b0_TEST FAIL\n"); return 1; }
    std::printf("0069e1b0_TEST PASS\n");
    return 0;
}