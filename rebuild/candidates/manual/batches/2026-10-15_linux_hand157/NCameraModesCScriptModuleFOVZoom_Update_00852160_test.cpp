// Behaviour test for 0x00852160 NCameraModes::CScriptModuleFOVZoom::Update (frame source stubbed).
#include <cstdio>
static long g_frame = 0;
long GFGetCurrentFrame() { return g_frame; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { C3DVector r; r.X = X + o.X; r.Y = Y + o.Y; r.Z = Z + o.Z; return r; }
};
struct CCameraState { C3DVector LookAt; C3DVector Position; float FOV; };
struct Module {
    CCameraState* POwner; C3DVector LookAtPos; long EndFrame; float FOVIncrement; C3DVector Increment;
    void Update()
    {
        if (GFGetCurrentFrame() < EndFrame) {
            POwner->Position = POwner->Position + Increment;
            POwner->LookAt = LookAtPos;
            POwner->FOV += FOVIncrement;
        }
    }
};
int main()
{
    CCameraState cam; cam.Position = C3DVector(0, 0, 0); cam.LookAt = C3DVector(9, 9, 9); cam.FOV = 1.0f;
    Module m; m.POwner = &cam; m.LookAtPos = C3DVector(1, 1, 1); m.EndFrame = 2; m.FOVIncrement = 0.5f; m.Increment = C3DVector(1, 2, 3);
    g_frame = 0; m.Update(); g_frame = 1; m.Update(); g_frame = 2; m.Update();
    if (cam.Position.X != 2 || cam.Position.Y != 4 || cam.Position.Z != 6 || cam.LookAt.X != 1 || cam.FOV != 2.0f) {
        std::printf("00852160_TEST FAIL\n"); return 1;
    }
    std::printf("00852160_TEST PASS\n");
    return 0;
}
