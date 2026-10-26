#include <cstdio>
// Behaviour model of CTCPhysicsStandard::UpdateIsMoving (0x008E5270)
static float g_fMinMovingSpeed = 1e-4f;
class C3DVector { public: float X, Y, Z; float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; } };
struct CTCPhysicsStandard { C3DVector Velocity; bool Moving, onSurface, gravity, immoveable;
    void UpdateIsMoving() {
        if (Moving) {
            if ((onSurface || !gravity) && !(Velocity.GetMagnitudeSquared() > g_fMinMovingSpeed * g_fMinMovingSpeed)) { Moving = false; return; }
            if (!immoveable) { Moving = true; return; }
            Moving = false;
        }
    } };
int main()
{
    CTCPhysicsStandard a = { { 0, 0, 0 }, true, true, true, false };
    a.UpdateIsMoving(); if (a.Moving) { std::printf("008e5270_TEST FAIL\n"); return 1; }
    CTCPhysicsStandard b = { { 1, 0, 0 }, true, true, true, false };
    b.UpdateIsMoving(); if (!b.Moving) { std::printf("008e5270_TEST FAIL\n"); return 1; }
    CTCPhysicsStandard c = { { 0, 0, 0 }, true, false, true, false };
    c.UpdateIsMoving(); if (!c.Moving) { std::printf("008e5270_TEST FAIL\n"); return 1; }
    CTCPhysicsStandard d = { { 1, 0, 0 }, true, false, true, true };
    d.UpdateIsMoving(); if (d.Moving) { std::printf("008e5270_TEST FAIL\n"); return 1; }
    std::printf("008e5270_TEST PASS\n");
    return 0;
}