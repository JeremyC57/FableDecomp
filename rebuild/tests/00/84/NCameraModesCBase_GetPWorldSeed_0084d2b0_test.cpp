// Behaviour test for NCameraModes::CBase::GetPWorldSeed 0x0084D2B0: the seed pointer comes from the world of the camera manager the
// mode belongs to (CameraManager at +0x40 in retail, World at +0x74 in the manager), nothing else.
#include <cstdio>
#include <cstddef>
struct CWorld { unsigned long Seed; unsigned long* GetWorldSeed() const { return const_cast<unsigned long*>(&Seed); } };
#pragma pack(push, 4)
struct CGameCameraManager { unsigned char Before[0x74]; CWorld* World; };
struct CBase {
    void* VPtr; float DefaultFOV, DefaultRollAngle, DefaultObservedPositionZOffset; unsigned char LookAt[0x30];
    CGameCameraManager* CameraManager;
    unsigned long* GetPWorldSeed() const { return CameraManager->World->GetWorldSeed(); }
};
#pragma pack(pop)
int main()
{
    if (offsetof(CBase, CameraManager) != (sizeof(void*) == 4 ? 0x40u : offsetof(CBase, CameraManager))) { std::printf("0084d2b0_TEST FAIL\n"); return 1; }
    CWorld w1 = { 7 }, w2 = { 9 };
    CGameCameraManager m; m.World = &w1;
    CBase mode; mode.CameraManager = &m;
    if (mode.GetPWorldSeed() != &w1.Seed || *mode.GetPWorldSeed() != 7) { std::printf("0084d2b0_TEST FAIL\n"); return 1; }
    m.World = &w2;
    if (mode.GetPWorldSeed() != &w2.Seed) { std::printf("0084d2b0_TEST FAIL\n"); return 1; }
    std::printf("0084d2b0_TEST PASS\n");
    return 0;
}