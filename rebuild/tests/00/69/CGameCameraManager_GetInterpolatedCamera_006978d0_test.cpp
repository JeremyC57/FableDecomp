// Behaviour test for CGameCameraManager::GetInterpolatedCamera (0x006978D0): a standalone copy of the model. The camera that comes
// back must carry every non-blended field of the CURRENT camera, and its field of view, position, up vector and view direction must
// be (new - old) * t + old per component: the old camera at t = 0, the current one at t = 1, the blend in between.
#include <cstdio>
#include <cmath>

static float GFGetInterpolatedValue(float old_val, float new_val, float interp) { return (new_val - old_val) * interp + old_val; }

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCamera {
public:
    CCamera() : HeightLocked(false), ViewVecZLocked(false), FOVFlags(0), HorizontalFOV(0), VerticalFOV(0), Zoom(0) {}
    float GetFOV() const { return HorizontalFOV; }
    void SetFOV(float horizontal_fov, float vertical_fov) { HorizontalFOV = horizontal_fov; VerticalFOV = vertical_fov; FOVFlags = 1; if (vertical_fov > 0.0f) FOVFlags = 3; } // retail 0x00A0BED0
    void SetView(const C3DVector& forward, const C3DVector& up) { RHSet.Forward = forward; RHSet.Up = up; }
    void MoveTo(const C3DVector& p) { WorldPosition = p; }
    const C3DVector& PeekWorldPos() const { return WorldPosition; }
    const C3DVector& PeekViewDirection() const { return RHSet.Forward; }
    const C3DVector& PeekUpDirection() const { return RHSet.Up; }
    C3DVector WorldPosition;
    CRightHandedSet RHSet;
    bool HeightLocked, ViewVecZLocked;
    unsigned long FOVFlags;
    float HorizontalFOV, VerticalFOV, Zoom;
};

static CCamera GetInterpolatedCamera(const float interpolate, const CCamera& camera, const CCamera& old_camera)
{
    CCamera passcam(camera);
    passcam.SetFOV(GFGetInterpolatedValue(old_camera.GetFOV(), camera.GetFOV(), interpolate), 0.0f);
    C3DVector vecman;
    vecman.X = (GFGetInterpolatedValue(old_camera.PeekWorldPos().X, camera.PeekWorldPos().X, interpolate));
    vecman.Y = (GFGetInterpolatedValue(old_camera.PeekWorldPos().Y, camera.PeekWorldPos().Y, interpolate));
    vecman.Z = (GFGetInterpolatedValue(old_camera.PeekWorldPos().Z, camera.PeekWorldPos().Z, interpolate));
    passcam.MoveTo(vecman);
    const C3DVector old_up = old_camera.PeekUpDirection();
    const C3DVector new_up = camera.PeekUpDirection();
    C3DVector up;
    up.X = (GFGetInterpolatedValue(old_up.X, new_up.X, interpolate));
    up.Y = (GFGetInterpolatedValue(old_up.Y, new_up.Y, interpolate));
    up.Z = (GFGetInterpolatedValue(old_up.Z, new_up.Z, interpolate));
    const C3DVector old_forward = old_camera.PeekViewDirection();
    const C3DVector new_forward = camera.PeekViewDirection();
    C3DVector forward;
    forward.X = (GFGetInterpolatedValue(old_forward.X, new_forward.X, interpolate));
    forward.Y = (GFGetInterpolatedValue(old_forward.Y, new_forward.Y, interpolate));
    forward.Z = (GFGetInterpolatedValue(old_forward.Z, new_forward.Z, interpolate));
    passcam.SetView(forward, up);
    return passcam;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static bool NearV(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }

int main()
{
    CCamera old_camera, camera;
    old_camera.WorldPosition = C3DVector(0.0f, 10.0f, -4.0f);
    old_camera.RHSet.Up = C3DVector(0.0f, 0.0f, 1.0f);
    old_camera.RHSet.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    old_camera.HorizontalFOV = 1.0f; old_camera.VerticalFOV = 7.0f; old_camera.Zoom = 3.0f;
    old_camera.FOVFlags = 1; old_camera.HeightLocked = false; old_camera.ViewVecZLocked = true;
    camera.WorldPosition = C3DVector(8.0f, 2.0f, 4.0f);
    camera.RHSet.Up = C3DVector(1.0f, 0.0f, 0.0f);
    camera.RHSet.Forward = C3DVector(0.0f, -1.0f, 2.0f);
    camera.HorizontalFOV = 2.0f; camera.VerticalFOV = 9.0f; camera.Zoom = 5.0f;
    camera.FOVFlags = 6; camera.HeightLocked = true; camera.ViewVecZLocked = false;

    bool ok = true;
    // t = 0: everything blended comes from the old camera, everything else from the current one.
    CCamera a = GetInterpolatedCamera(0.0f, camera, old_camera);
    ok = ok && NearV(a.WorldPosition, 0.0f, 10.0f, -4.0f) && NearV(a.RHSet.Up, 0.0f, 0.0f, 1.0f) && NearV(a.RHSet.Forward, 0.0f, 1.0f, 0.0f);
    ok = ok && Near(a.HorizontalFOV, 1.0f) && Near(a.VerticalFOV, 0.0f);
    ok = ok && Near(a.Zoom, 5.0f) && a.FOVFlags == 1 && a.HeightLocked && !a.ViewVecZLocked;
    // t = 1: the current camera.
    CCamera b = GetInterpolatedCamera(1.0f, camera, old_camera);
    ok = ok && NearV(b.WorldPosition, 8.0f, 2.0f, 4.0f) && NearV(b.RHSet.Up, 1.0f, 0.0f, 0.0f) && NearV(b.RHSet.Forward, 0.0f, -1.0f, 2.0f);
    ok = ok && Near(b.HorizontalFOV, 2.0f) && b.FOVFlags == 1;
    // t = 0.25: a quarter of the way from old to current, per component.
    CCamera c = GetInterpolatedCamera(0.25f, camera, old_camera);
    ok = ok && NearV(c.WorldPosition, 2.0f, 8.0f, -2.0f) && NearV(c.RHSet.Up, 0.25f, 0.0f, 0.75f) && NearV(c.RHSet.Forward, 0.0f, 0.5f, 0.5f);
    ok = ok && Near(c.HorizontalFOV, 1.25f) && Near(c.Zoom, 5.0f) && c.FOVFlags == 1;
    // The arguments are not modified.
    ok = ok && NearV(camera.WorldPosition, 8.0f, 2.0f, 4.0f) && NearV(old_camera.RHSet.Up, 0.0f, 0.0f, 1.0f) && Near(old_camera.HorizontalFOV, 1.0f);
    // Same camera twice: the blend is the identity for any factor.
    CCamera d = GetInterpolatedCamera(0.6f, camera, camera);
    ok = ok && NearV(d.WorldPosition, 8.0f, 2.0f, 4.0f) && NearV(d.RHSet.Forward, 0.0f, -1.0f, 2.0f) && Near(d.HorizontalFOV, 2.0f);

    if (!ok) { std::printf("006978d0_TEST FAIL\n"); return 1; }
    std::printf("006978d0_TEST PASS\n");
    return 0;
}