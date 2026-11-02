// Behaviour test for CGameCameraManager::SetRightStickLookAroundVector (0x00697B10): the target look-around angles become
// (x * 0.25, y * 0.125); the current angles are untouched; the current camera mode receives the unscaled vector.
#include <cstdio>
class C2DVector { public: float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { } };
static int gModeCalls; static C2DVector gModeVec;
namespace NCameraModes { class CBase { public: void SetRightStickLookAround(const C2DVector& v) { ++gModeCalls; gModeVec = v; } }; }
template <class T> class CCountedPointer { public: T* P; T* operator->() const { return P; } };
class CGameCameraManager { public:
    CCountedPointer<NCameraModes::CBase> CurrentMode;
    C2DVector RightStickLookAroundAngles;
    C2DVector RightStickTargetLookAroundAngles;
    void SetRightStickLookAroundVector(const C2DVector& vector);
};
void CGameCameraManager::SetRightStickLookAroundVector(const C2DVector& vector)
{
    RightStickTargetLookAroundAngles = C2DVector(vector.X * 0.25f, vector.Y * 0.125f);
    CurrentMode->SetRightStickLookAround(vector);
}
int main()
{
    NCameraModes::CBase mode; CGameCameraManager cam; cam.CurrentMode.P = &mode;
    cam.RightStickLookAroundAngles = C2DVector(7.0f, 9.0f);
    cam.SetRightStickLookAroundVector(C2DVector(-0.5f, 1.0f));
    if (cam.RightStickTargetLookAroundAngles.X != -0.125f || cam.RightStickTargetLookAroundAngles.Y != 0.125f
        || cam.RightStickLookAroundAngles.X != 7.0f || cam.RightStickLookAroundAngles.Y != 9.0f
        || gModeCalls != 1 || gModeVec.X != -0.5f || gModeVec.Y != 1.0f) { std::printf("00697b10_TEST FAIL\n"); return 1; }
    std::printf("00697b10_TEST PASS\n");
    return 0;
}