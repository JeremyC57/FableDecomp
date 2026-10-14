#include <cstdio>
struct C3DVector { unsigned long X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct SCameraState { unsigned char _raw[0x38]; };
static float g_blend = 0.0f; static C3DVector g_pos;
struct CAIGameCameraBase { virtual void v00() {} virtual const C3DVector& Evaluate(SCameraState& state, float blend) { g_blend = blend; state._raw[0] = 1; return g_pos; } C3DVector PeekCurrentPos(); };
C3DVector CAIGameCameraBase::PeekCurrentPos() { SCameraState state; return Evaluate(state, 1.0f); }
int main()
{
    g_pos.X = 1; g_pos.Y = 2; g_pos.Z = 3; CAIGameCameraBase cam;
    C3DVector p = cam.PeekCurrentPos();
    if (p.X != 1 || p.Y != 2 || p.Z != 3 || g_blend != 1.0f) { std::printf("0069c1b0_TEST FAIL\n"); return 1; }
    std::printf("0069c1b0_TEST PASS\n");
    return 0;
}
