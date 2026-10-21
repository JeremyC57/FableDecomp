// Behaviour test for 004884d0 (CPlayer::UpdateFreeCamera): the old camera takes last frame's free camera, the YZ
// tracking arm points the working camera at the target, the XY arm sets an angle in turns wrapped into [0,1), and the
// free camera always ends up equal to the working camera.
#include <cstdio>
#include <cmath>

struct V3 { float X, Y, Z; };
struct Cam { V3 Position; float angle; V3 lookAt; int pointed; };
struct Model {
    bool Using, Tracking, TrackingYZ; bool hasCreature; V3 target;
    Cam Current, Free, Old;
    void Update()
    {
        Old = Free;
        if (Using && hasCreature) {
            if (TrackingYZ) {
                Current.lookAt = target; ++Current.pointed;
            } else if (Tracking) {
                const V3& from = Current.Position;
                float angle = (float)(std::atan2(target.X - from.X, target.Y - from.Y) * 0.15915494f);
                if (angle < 0.0f || angle >= 1.0f) {
                    angle = (float)std::fmod((double)angle, 1.0);
                    if (angle < 0.0f) angle += 1.0f;
                }
                Current.angle = angle;
            }
        }
        Free = Current;
    }
};
int main()
{
    Model m = {};
    m.Using = true; m.hasCreature = true; m.Tracking = true;
    m.Current.Position.X = 0; m.Current.Position.Y = 0;
    m.target.X = -1.0f; m.target.Y = 0.0f;               // atan2(-1,0) = -pi/2 -> -0.25 turns -> 0.75
    m.Free.angle = 9.0f;
    m.Update();
    int ok = std::fabs(m.Current.angle - 0.75f) < 1e-5f && m.Old.angle == 9.0f && m.Free.angle == m.Current.angle;
    m.target.X = 1.0f; m.target.Y = 0.0f; m.Update();   // +0.25
    if (std::fabs(m.Current.angle - 0.25f) > 1e-5f) ok = 0;
    m.TrackingYZ = true; m.target.Z = 5.0f; m.Update();
    if (m.Current.pointed != 1 || m.Free.lookAt.Z != 5.0f) ok = 0;
    m.Using = false; m.Current.angle = 0.5f; m.Update();
    if (m.Free.angle != 0.5f || m.Current.pointed != 1) ok = 0;
    if (!ok) { std::printf("004884d0_TEST FAIL\n"); return 1; }
    std::printf("004884d0_TEST PASS\n");
    return 0;
}
