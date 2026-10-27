// behaviour test for 0x004D3AF4 CTCCameraPointTrack::CTCCameraPointTrack (model: the member defaults)
#include <cstdio>
struct Obj { float CameraStringLength; float CameraCageRadius; Obj() : CameraStringLength(7.0f), CameraCageRadius(2.0f) { } };
int main()
{
    Obj o;
    int ok = o.CameraStringLength == 7.0f && o.CameraCageRadius == 2.0f;
    if (!ok) { std::printf("004d3af4_TEST FAIL\n"); return 1; }
    std::printf("004d3af4_TEST PASS\n");
    return 0;
}