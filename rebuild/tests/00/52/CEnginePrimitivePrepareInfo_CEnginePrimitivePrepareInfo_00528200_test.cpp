// Behaviour test for 00528200 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
static int g_CameraCopies = 0;
class C3DVector { public: float X, Y, Z; };
struct CInterpolationInfoSet { int Frame; float Fraction; };
class CInterpolationInfo { public: CInterpolationInfoSet S[6]; int WFServerCurrentTime; };
class CCamera { public: C3DVector WorldPosition, Up, Forward; bool HeightLocked, ViewVecZLocked; unsigned int FOVFlags; float HFOV, VFOV, Zoom;
    CCamera() { } CCamera(const CCamera& o) : WorldPosition(o.WorldPosition), Up(o.Up), Forward(o.Forward), HeightLocked(o.HeightLocked), ViewVecZLocked(o.ViewVecZLocked), FOVFlags(o.FOVFlags), HFOV(o.HFOV), VFOV(o.VFOV), Zoom(o.Zoom) { ++g_CameraCopies; } };
class CEnginePrimitivePrepareInfo { public: CInterpolationInfo Interpolate; void* Engine; bool Edit2DEngine; CCamera Camera;
    CEnginePrimitivePrepareInfo() { }
    CEnginePrimitivePrepareInfo(const CEnginePrimitivePrepareInfo& o) : Interpolate(o.Interpolate), Engine(o.Engine), Edit2DEngine(o.Edit2DEngine), Camera(o.Camera) { } };
int main() {
    CEnginePrimitivePrepareInfo a; a.Interpolate.WFServerCurrentTime = 77; a.Interpolate.S[3].Fraction = 0.5f; a.Engine = &a; a.Edit2DEngine = true;
    a.Camera.Zoom = 2.0f; a.Camera.WorldPosition.Y = -3.0f; a.Camera.FOVFlags = 9; a.Camera.HeightLocked = true; a.Camera.ViewVecZLocked = false;
    CEnginePrimitivePrepareInfo b(a);
    if (b.Interpolate.WFServerCurrentTime != 77 || b.Interpolate.S[3].Fraction != 0.5f || b.Engine != &a || !b.Edit2DEngine) { std::printf("00528200_TEST FAIL\n"); return 1; }
    if (b.Camera.Zoom != 2.0f || b.Camera.WorldPosition.Y != -3.0f || b.Camera.FOVFlags != 9 || g_CameraCopies != 1) { std::printf("00528200_TEST FAIL\n"); return 1; }
    { std::printf("00528200_TEST PASS\n"); return 0; }
}