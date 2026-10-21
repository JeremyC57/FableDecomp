#include <cstdio>
#include <cmath>
// behaviour test: UpdateAppliedMovement leaves everything alone while the creature performs an action; otherwise it takes and
// clears the movement vector, rotates it by the relative movement angle, normalises it and (if its length is above 1e-4)
// zeroes the no-movement timer and applies the unit vector scaled by half its length (0.5) as acceleration
struct V { float X, Y, Z; };
struct CSpirit {
    bool Busy; float Angle; long NoMovementTimer; V MovementVector; bool Applied; V Acceleration;
    void UpdateAppliedMovement()
    {
        if (Busy)
            return;
        V m = MovementVector;
        MovementVector.X = MovementVector.Y = MovementVector.Z = 0.0f;
        float c = std::cos(Angle), s = std::sin(Angle);
        V r = { m.X * c - m.Y * s, m.X * s + m.Y * c, m.Z };
        float l0 = std::sqrt(r.X * r.X + r.Y * r.Y + r.Z * r.Z);
        if (l0 > 0.0f) { r.X /= l0; r.Y /= l0; r.Z /= l0; }
        float length = std::sqrt((r.X * r.X + r.Y * r.Y) + r.Z * r.Z);
        if (length > 1e-4f) {
            NoMovementTimer = 0;
            r.X *= length * 0.5f; r.Y *= length * 0.5f; r.Z *= length * 0.5f;
            Applied = true; Acceleration = r;
        }
    }
};
int main()
{
    CSpirit s = { true, 0.0f, 7, { 3.0f, 0.0f, 0.0f }, false, { 0, 0, 0 } };
    s.UpdateAppliedMovement();
    bool ok = !s.Applied && s.MovementVector.X == 3.0f && s.NoMovementTimer == 7;
    s.Busy = false;
    s.UpdateAppliedMovement();
    ok = ok && s.Applied && s.MovementVector.X == 0.0f && s.NoMovementTimer == 0 && std::fabs(s.Acceleration.X - 0.5f) < 1e-4f;
    s.Applied = false; s.NoMovementTimer = 9;
    s.UpdateAppliedMovement();                          // zero vector: nothing applied
    ok = ok && !s.Applied && s.NoMovementTimer == 9;
    s.MovementVector.X = 0.0f; s.MovementVector.Y = 2.0f; s.Angle = 1.5707964f;
    s.UpdateAppliedMovement();
    ok = ok && s.Applied && std::fabs(s.Acceleration.X + 0.5f) < 1e-3f;
    if (!ok) { std::printf("006702e0_TEST FAIL\n"); return 1; }
    std::printf("006702e0_TEST PASS\n");
    return 0;
}