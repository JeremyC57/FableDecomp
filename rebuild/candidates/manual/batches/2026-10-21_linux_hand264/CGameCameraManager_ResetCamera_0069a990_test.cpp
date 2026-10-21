// Behaviour test for CGameCameraManager::ResetCamera @ 0x0069A990: the height band becomes [heroZ, heroZ + 10], the
// mode and both script modules are dropped, earthquakes cleared, the old camera snapshot equals the camera, the
// single-cut and angle-change flags are set, and the renderer message is the "not using fast mode" text.
#include <cstdio>
#include <string>
#include <vector>
struct Manager {
    float minH, maxH; bool mode, scriptA, scriptB, singleCut, angleChange; std::vector<int> quakes; int camera, oldCamera;
    std::string message;
    void Reset(float heroZ)
    {
        minH = heroZ; maxH = minH + 10.0f;
        mode = false; scriptA = false; scriptB = false; quakes.clear();
        oldCamera = camera; singleCut = true; angleChange = true;
        bool fastMode = false;
        message = "telling engine that the camera has moved and";
        if (!fastMode) message += " not ";
        message += "using fast mode";
        oldCamera = camera;
    }
};
int main()
{
    Manager m; m.mode = m.scriptA = m.scriptB = true; m.singleCut = m.angleChange = false; m.quakes.push_back(1); m.camera = 7; m.oldCamera = 0;
    m.Reset(3.5f);
    if (m.minH != 3.5f || m.maxH != 13.5f || m.mode || m.scriptA || m.scriptB || !m.quakes.empty()) { std::printf("0069a990_TEST FAIL\n"); return 1; }
    if (m.oldCamera != 7 || !m.singleCut || !m.angleChange) { std::printf("0069a990_TEST FAIL\n"); return 1; }
    if (m.message != "telling engine that the camera has moved and not using fast mode") { std::printf("0069a990_TEST FAIL\n"); return 1; }
    std::printf("0069a990_TEST PASS\n");
    return 0;
}
