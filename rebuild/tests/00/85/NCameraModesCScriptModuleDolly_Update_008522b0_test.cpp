// Behaviour test for 0x008522B0 NCameraModes::CScriptModuleDolly::Update: camera placement and the angle
// handed to C3DVector::Rotate (recorded by a host stub).
#include <cstdio>
long g_FramesPerSecond = 15;
static float g_angle = 0; static float g_axisZ = 0;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { C3DVector r; r.X = X + o.X; r.Y = Y + o.Y; r.Z = Z + o.Z; return r; }
    void Rotate(const C3DVector& axis, float angle) { g_angle = angle; g_axisZ = axis.Z; }
};
struct CCameraState { C3DVector LookAt; C3DVector Position; };
struct Module {
    CCameraState* POwner; C3DVector PosAround; C3DVector DollyVector; float TimeFor360Turn;
    void Update()
    {
        long fps = g_FramesPerSecond;
        POwner->Position = PosAround + DollyVector;
        POwner->LookAt = PosAround;
        DollyVector.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 1.0f / (fps * TimeFor360Turn));
    }
};
int main()
{
    CCameraState cam; Module m; m.POwner = &cam; m.PosAround = C3DVector(1, 2, 3); m.DollyVector = C3DVector(10, 0, 0); m.TimeFor360Turn = 2.0f;
    m.Update();
    if (cam.Position.X != 11 || cam.Position.Z != 3 || cam.LookAt.Y != 2 || g_axisZ != 1.0f || g_angle < 0.0333f || g_angle > 0.0334f) { std::printf("008522b0_TEST FAIL\n"); return 1; }
    std::printf("008522b0_TEST PASS\n");
    return 0;
}