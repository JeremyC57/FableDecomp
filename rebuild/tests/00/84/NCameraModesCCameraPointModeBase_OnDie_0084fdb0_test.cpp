// Test for NCameraModes::CCameraPointModeBase::OnDie() -- sets the camera FOV to 0.2 then runs the derived OnDieLeaf.
#include <cstdio>
struct CBase { float CameraFOV; CBase() : CameraFOV(1.0f) { } virtual ~CBase() { } };
struct CCameraPointModeBase : CBase {
    int leafCalls; float fovAtLeaf;
    CCameraPointModeBase() : leafCalls(0), fovAtLeaf(0) { }
    virtual void OnDieLeaf() { ++leafCalls; fovAtLeaf = CameraFOV; }
    virtual void OnDie() { CameraFOV = 0.2f; OnDieLeaf(); }
};
int main()
{
    CCameraPointModeBase m;
    m.OnDie();
    if (m.leafCalls != 1 || m.CameraFOV != 0.2f || m.fovAtLeaf != 0.2f) { std::printf("0084fdb0_TEST FAIL\n"); return 1; }
    std::printf("0084fdb0_TEST PASS\n");
    return 0;
}