#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
struct CUpForward { C3DVector Up, Forward; CUpForward(const C3DVector& f) : Up(0.0f, 0.0f, 1.0f), Forward(f) { } };
struct Body { C3DVector facing; C3DVector up, fwd; C3DVector GetFacing() { return facing; } void SetOrientation(const CUpForward& o) { up = o.Up; fwd = o.Forward; } };
struct Modes { int removed; void RemoveMode(unsigned long m) { removed = (int)m; } };
static const float g_fMinFacingLength = 1e-4f;
struct Action {
    Body* body; Modes* modes; int baseFinished;
    void FinishAction()
    {
        modes->RemoveMode(0x1f);
        C3DVector forward = body->GetFacing();
        forward.Z = 0.0f;
        if (forward.X * forward.X + forward.Y * forward.Y > g_fMinFacingLength * g_fMinFacingLength) forward.Normalise();
        else forward = C3DVector(1.0f, 0.0f, 0.0f);
        body->SetOrientation(CUpForward(forward));
        ++baseFinished;
    }
};
int main()
{
    Body b; b.facing = C3DVector(3.0f, 4.0f, 9.0f); Modes m = { 0 };
    Action a = { &b, &m, 0 };
    a.FinishAction();
    if (m.removed != 0x1f || std::fabs(b.fwd.X - 0.6f) > 1e-5f || std::fabs(b.fwd.Y - 0.8f) > 1e-5f || b.fwd.Z != 0.0f || b.up.Z != 1.0f || a.baseFinished != 1) { std::printf("00866e80_TEST FAIL\n"); return 1; }
    b.facing = C3DVector(0.0f, 0.0f, 1.0f); a.FinishAction();
    if (b.fwd.X != 1.0f || b.fwd.Y != 0.0f) { std::printf("00866e80_TEST FAIL\n"); return 1; }
    std::printf("00866e80_TEST PASS\n");
    return 0;
}