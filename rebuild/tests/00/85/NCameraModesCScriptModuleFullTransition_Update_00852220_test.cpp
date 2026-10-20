// Behaviour test for 00852220: body copied from src/00852220.cpp (NCameraModes::CScriptModuleFullTransition::Update), GFGetCurrentFrame stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool feq(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
static long g_frame = 0; static int g_frameCalls = 0;
long __fastcall GFGetCurrentFrame() { ++g_frameCalls; return g_frame; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
};
struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;
    unsigned char _pad1c[0x38];
    C3DVector     Position;
};
namespace NCameraModes {
struct CScriptModuleFullTransition {
    virtual void v00() {}
    unsigned char _pad04[0x34];
    CCameraState* POwner;
    unsigned char _pad3c[0x10];
    C3DVector     IncrementForLookAtPoints;
    C3DVector     Increment;
    C3DVector     LookAtPos;
    C3DVector     StartPos;
    long          EndFrame;
    void Update();
};
void CScriptModuleFullTransition::Update()
{
    if (GFGetCurrentFrame() < EndFrame) {
        POwner->Position = POwner->Position + Increment;
        POwner->LookAt = POwner->LookAt + IncrementForLookAtPoints;
    }
}
}
int main()
{
    CCameraState cam; memset(&cam, 0, sizeof(cam));
    cam.Position = C3DVector(1.0f, 2.0f, 3.0f); cam.LookAt = C3DVector(10.0f, 20.0f, 30.0f);
    NCameraModes::CScriptModuleFullTransition m;
    m.POwner = &cam; m.EndFrame = 100;
    m.Increment = C3DVector(0.5f, -1.0f, 2.0f); m.IncrementForLookAtPoints = C3DVector(1.0f, 1.0f, -1.0f);
    g_frame = 50; m.Update();
    CHECK(g_frameCalls == 1);
    CHECK(feq(cam.Position.X, 1.5f) && feq(cam.Position.Y, 1.0f) && feq(cam.Position.Z, 5.0f));
    CHECK(feq(cam.LookAt.X, 11.0f) && feq(cam.LookAt.Y, 21.0f) && feq(cam.LookAt.Z, 29.0f));
    g_frame = 100; m.Update();                       // frame == EndFrame: no step
    CHECK(g_frameCalls == 2);
    CHECK(feq(cam.Position.X, 1.5f) && feq(cam.LookAt.X, 11.0f));
    g_frame = 99; m.Update();
    CHECK(feq(cam.Position.X, 2.0f) && feq(cam.Position.Y, 0.0f) && feq(cam.Position.Z, 7.0f));
    CHECK(feq(cam.LookAt.X, 12.0f) && feq(cam.LookAt.Y, 22.0f) && feq(cam.LookAt.Z, 28.0f));
    if (fails == 0) printf("00852220_TEST PASS\n"); else printf("00852220_TEST FAIL\n");
    return fails;
}