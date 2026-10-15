// Behaviour test for 0x00852560 NCameraModes::CScriptModuleFOVZoom constructor (frame source and the
// increment helper modelled on the host).
#include <cstdio>
static long g_frame = 100;
long GFGetCurrentFrame() { return g_frame; }
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct Module {
    C3DVector LookAtPos; long EndFrame; long WorldFrames; float FOVFraction; C3DVector Increment; C3DVector StartPos;
    bool ShortenOnOcclusion; bool ResetCamera;
    void CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames)
    {
        float inv = 1.0f / frames;
        Increment = C3DVector((to.X - from.X) * inv, (to.Y - from.Y) * inv, (to.Z - from.Z) * inv);
    }
    Module(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to, float fovFraction, long frames, bool shorten)
        : LookAtPos(lookAt), FOVFraction(fovFraction), StartPos(from), ShortenOnOcclusion(shorten), ResetCamera(true)
    {
        WorldFrames = frames ? frames : 15;
        CalculateIncrementVector(from, to, WorldFrames);
        EndFrame = GFGetCurrentFrame() + frames;
    }
};
int main()
{
    C3DVector look(1, 2, 3), from(0, 0, 0), to(10, 20, 30);
    Module a(look, from, to, 0.5f, 10, true);
    Module b(look, from, to, 0.25f, 0, false);
    bool ok = a.WorldFrames == 10 && a.EndFrame == 110 && a.Increment.X == 1.0f && a.Increment.Z == 3.0f && a.ResetCamera
        && a.ShortenOnOcclusion && a.FOVFraction == 0.5f && a.LookAtPos.Y == 2 && a.StartPos.X == 0
        && b.WorldFrames == 15 && b.EndFrame == 100 && b.Increment.X > 0.666f && b.Increment.X < 0.667f && !b.ShortenOnOcclusion;
    if (!ok) { std::printf("00852560_TEST FAIL\n"); return 1; }
    std::printf("00852560_TEST PASS\n");
    return 0;
}