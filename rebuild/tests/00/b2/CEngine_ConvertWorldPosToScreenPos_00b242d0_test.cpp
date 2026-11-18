// Standalone C++03 test of the submitted view transformation, depth rejection, and screen projection.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };

struct CEngineCamera {
    int world_to_view_calls, view_to_screen_calls;
    CEngineCamera() : world_to_view_calls(0), view_to_screen_calls(0) {}

    void TransformWorldToView(const C3DVector& from, C3DVector& to)
    {
        ++world_to_view_calls;
        to.X = from.Y + 1.0f;
        to.Y = from.X - 2.0f;
        to.Z = from.Z + 1.0f;
    }

    void TransformViewToScreen(const C3DVector& from, C3DVector& to)
    {
        ++view_to_screen_calls;
        const float invz = 1.0f / from.Z;
        to.X = from.X * 320.0f * invz + 320.0f;
        to.Y = 240.0f - invz * 240.0f * from.Y;
        to.Z = from.Z;
    }
};

class CEngine {
public:
    explicit CEngine(bool active) : Active(active) {}
    bool IsActive() const { return Active; }
    bool ConvertWorldPosToScreenPos(const C3DVector& wpos, C2DVector& mpos) const;
private:
    bool Active;
};

namespace NEngineInternal { CEngineCamera* GPCamera; }

// Recovered method body, with game dependencies supplied by the fixture.
bool CEngine::ConvertWorldPosToScreenPos(const C3DVector& wpos, C2DVector& mpos) const
{
    if (IsActive()) {
        C3DVector from;
        // Assign the input so the view transform reads the preserved coordinates.
        from = wpos;
        C3DVector to;
        NEngineInternal::GPCamera->TransformWorldToView(from, to);
        from = to;
        if (from.Z > 0.0f) {
            NEngineInternal::GPCamera->TransformViewToScreen(from, to);
            mpos.X = to.X;
            mpos.Y = to.Y;
            return true;
        }
    }
    return false;
}

int main()
{
    CEngineCamera camera;
    NEngineInternal::GPCamera = &camera;
    const CEngine inactive(false), active(true);
    const C3DVector front = {3.0f, 0.0f, 1.0f};
    const C3DVector behind = {3.0f, 0.0f, -2.0f};
    const C3DVector on_plane = {3.0f, 0.0f, -1.0f};
    C2DVector screen = {-1.0f, -1.0f};
    bool ok = !inactive.ConvertWorldPosToScreenPos(front, screen)
        && screen.X == -1.0f && screen.Y == -1.0f && camera.world_to_view_calls == 0;
    ok = ok && !active.ConvertWorldPosToScreenPos(behind, screen)
        && !active.ConvertWorldPosToScreenPos(on_plane, screen)
        && screen.X == -1.0f && screen.Y == -1.0f && camera.view_to_screen_calls == 0;
    ok = ok && active.ConvertWorldPosToScreenPos(front, screen)
        && screen.X == 480.0f && screen.Y == 120.0f
        && camera.world_to_view_calls == 3 && camera.view_to_screen_calls == 1;
    std::puts(ok ? "00b242d0_TEST PASS" : "00b242d0_TEST FAIL");
    return ok ? 0 : 1;
}