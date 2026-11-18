// Standalone C++03 test of the submitted horizontal circle scan and frustum short-circuit.
#include <cmath>
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector CrossWithUp() const { return C3DVector(Y, -X, 0.0f); }
    void AccurateNormalise() { *this = *this * (1.0f / (float)std::sqrt(X * X + Y * Y + Z * Z)); }
};

struct Frustum {
    int hits_at;
    mutable int calls;
    C3DVector expected_camera, expected_look, origin;
    float expected_radius;
    mutable bool arguments_valid;

    Frustum(int index, const C3DVector& camera, const C3DVector& look, const C3DVector& center, float radius)
        : hits_at(index), calls(0), expected_camera(camera), expected_look(look), origin(center),
          expected_radius(radius), arguments_valid(true) {}

    bool IsPosInViewingFrustrum(const C3DVector& r, const C3DVector& look, const C3DVector& camera, float fov) const
    {
        const float dx = r.X - origin.X;
        const float dy = r.Y - origin.Y;
        arguments_valid = arguments_valid && r.Z == origin.Z && fov == 0.75f
            && look.X == expected_look.X && look.Y == expected_look.Y && look.Z == expected_look.Z
            && camera.X == expected_camera.X && camera.Y == expected_camera.Y && camera.Z == expected_camera.Z
            && std::fabs(dx * dx + dy * dy - expected_radius * expected_radius) < 0.001f;
        if (calls == 0)
            arguments_valid = arguments_valid && dx == expected_radius && dy == 0.0f;
        if (calls == 1 && expected_radius > 0.0f)
            arguments_valid = arguments_valid && dx > 0.0f && dy < 0.0f;
        return calls++ == hits_at;
    }
};

struct CameraMode {
    C3DVector position, observed;
    CameraMode(const C3DVector& camera, const C3DVector& look) : position(camera), observed(look) {}
    const C3DVector& GetPosition() const { return position; }
    const C3DVector& GetObservedPos() const { return observed; }
};

struct CameraState {
    float GetFOV() const { return 0.75f; }
};

class CGameCameraManager {
public:
    CameraState Camera;
    const CameraMode* CurrentMode;
    Frustum CamTools;

    CGameCameraManager(const CameraMode& mode, int hit, const C3DVector& center, float radius)
        : CurrentMode(&mode), CamTools(hit, mode.position, mode.observed, center, radius) {}

    bool Is2DAreaInViewingFrustrum(const C3DVector& origin, float radius) const;
};

extern const double TWO_PI = 6.2831854820251465;
using std::cos;
using std::sin;

// Recovered method body, with game dependencies supplied by the fixture.
bool CGameCameraManager::Is2DAreaInViewingFrustrum(const C3DVector& origin, float radius) const
{
    float fov = Camera.GetFOV();
    C3DVector cam_pos = CurrentMode->GetPosition();
    C3DVector cam_look_at = CurrentMode->GetObservedPos();
    // Construct the horizontal basis for sampling the circle.
    C3DVector line = cam_look_at - cam_pos;
    line.Z = 0.0f;
    line.AccurateNormalise();
    C3DVector perp = line.CrossWithUp();
    perp.AccurateNormalise();
    float d_theta = 0.1f;
    for (float theta = 0.0f; theta <= 1.0f; theta += d_theta) {
        C3DVector r = origin + line * (float)cos(theta * TWO_PI) * radius
                            + perp * (float)sin(theta * TWO_PI) * radius;
        if (CamTools.IsPosInViewingFrustrum(r, cam_look_at, cam_pos, fov))
            return true;
    }
    return false;
}

int main()
{
    const C3DVector camera(1, 3, 2), look(5, 3, 9), center(4, 6, 8);
    const CameraMode mode(camera, look);
    const CGameCameraManager first(mode, 0, center, 2.0f);
    const CGameCameraManager fourth(mode, 3, center, 2.0f);
    const CGameCameraManager none(mode, -1, center, 2.0f);
    const CGameCameraManager point(mode, 0, center, 0.0f);
    const bool accepted_first = first.Is2DAreaInViewingFrustrum(center, 2.0f);
    const bool accepted_fourth = fourth.Is2DAreaInViewingFrustrum(center, 2.0f);
    const bool accepted_none = none.Is2DAreaInViewingFrustrum(center, 2.0f);
    const bool accepted_point = point.Is2DAreaInViewingFrustrum(center, 0.0f);
    const bool ok = accepted_first && first.CamTools.calls == 1 && first.CamTools.arguments_valid
        && accepted_fourth && fourth.CamTools.calls == 4 && fourth.CamTools.arguments_valid
        && !accepted_none && none.CamTools.calls == 10 && none.CamTools.arguments_valid
        && accepted_point && point.CamTools.calls == 1 && point.CamTools.arguments_valid;
    std::puts(ok ? "00697f80_TEST PASS" : "00697f80_TEST FAIL");
    return ok ? 0 : 1;
}