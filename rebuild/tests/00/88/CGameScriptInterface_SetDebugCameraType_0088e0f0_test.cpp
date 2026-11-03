#include <cstdio>
// Model of CGameScriptInterface::SetDebugCameraType: World->GetGameCamera()->SetDebugCameraType(type) (virtual slot 21).
struct Camera { long type; Camera() : type(0) { } virtual ~Camera() { } virtual void SetDebugCameraType(long t) { type = t; } };
struct World { Camera* PCurrentGameCamera; Camera* GetGameCamera() { return PCurrentGameCamera; } };
struct Interface { World* World_; void SetDebugCameraType(long t) const { World_->GetGameCamera()->SetDebugCameraType(t); } };
int main()
{
    Camera cam; World w = { &cam }; Interface gsi = { &w };
    gsi.SetDebugCameraType(3);
    if (cam.type != 3) { std::printf("0088e0f0_TEST FAIL\n"); return 1; }
    std::printf("0088e0f0_TEST PASS\n"); return 0;
}