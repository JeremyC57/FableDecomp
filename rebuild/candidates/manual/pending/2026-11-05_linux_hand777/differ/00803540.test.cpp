// Behaviour test for CTCTargetingPlayer::UpdateTargetedPositionRandomWalk (0x00803540): a model of the row -- nothing happens
// without a ranged-attacks component or when the target sits on the camera; otherwise the second-order offset becomes the
// (sin, cos) of a random turn, the 2D offset advances by its delta, and the 3D offset lies across the camera-to-target line:
// the first term is scaled by 0, the second is the normalised up vector times cos * random amount * shake.
#include <stdio.h>
#include <math.h>
struct V3 { float X, Y, Z; };
struct V2 { float X, Y; };
static V3 Cross(const V3& a, const V3& b) { V3 r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
static void Normalise(V3& v) { float m = (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); v.X /= m; v.Y /= m; v.Z /= m; }
struct Targeting {
    bool HasRanged; float PullBack; V3 Camera, TargetedPos, Offset3D; V2 Offset, Delta, SecondOrder; float RandomAngle, RandomAmount;
    void UpdateTargetedPositionRandomWalk()
    {
        if (!HasRanged) return;
        float shake = PullBack;
        V3 d = { TargetedPos.X - Camera.X, TargetedPos.Y - Camera.Y, TargetedPos.Z - Camera.Z };
        float distance = (float)sqrt(d.X * d.X + d.Z * d.Z + d.Y * d.Y);
        if (distance < 1e-4f) return;
        d.X /= distance; d.Y /= distance; d.Z /= distance;
        V3 perpendicular = { 0.0f, 0.0f, 0.0f };
        if (fabs(d.X) > fabs(d.Y)) { perpendicular.Y = 1.0f; perpendicular.X = d.Y / d.X * -1.0f; }
        else { perpendicular.X = 1.0f; perpendicular.Y = d.X / d.Y * -1.0f; }
        Normalise(perpendicular);
        V3 up = Cross(perpendicular, d); Normalise(up);
        float sine = (float)sin(RandomAngle * 6.2831853), cosine = (float)cos(RandomAngle * 6.2831853);
        SecondOrder.X = sine; SecondOrder.Y = cosine;
        Offset.X += Delta.X; Offset.Y += Delta.Y;
        float scale = cosine * RandomAmount * shake;
        Offset3D.X = up.X * scale; Offset3D.Y = up.Y * scale; Offset3D.Z = up.Z * scale;
    }
};
int main()
{
    Targeting t = { true, 2.0f, { 0, 0, 0 }, { 10, 0, 0 }, { 0, 0, 0 }, { 1, 1 }, { 0.5f, 0.25f }, { 0, 0 }, 0.0f, 0.5f };
    t.UpdateTargetedPositionRandomWalk();
    float dot = t.Offset3D.X * 1.0f;
    float m = (float)sqrt(t.Offset3D.X * t.Offset3D.X + t.Offset3D.Y * t.Offset3D.Y + t.Offset3D.Z * t.Offset3D.Z);
    int ok = fabs(dot) < 1e-5f && fabs(m - 1.0f) < 1e-5f && t.Offset.X == 1.5f && t.Offset.Y == 1.25f && t.SecondOrder.Y == 1.0f;
    Targeting u = t; u.TargetedPos = u.Camera; u.Offset.X = 7.0f;
    u.UpdateTargetedPositionRandomWalk();
    ok = ok && u.Offset.X == 7.0f;
    Targeting w = t; w.HasRanged = false; w.Offset.X = 3.0f; w.UpdateTargetedPositionRandomWalk();
    ok = ok && w.Offset.X == 3.0f;
    printf(ok ? "00803540_TEST PASS\n" : "00803540_TEST FAIL\n");
    return ok ? 0 : 1;
}
