// Behaviour test for 0x00B806E0 CEngineClothSimulationMeshBase::CalculateFadeOut: 0 inside SimFadeBegin of the zoomed
// simulation radius from the camera, rising linearly to 1 at the radius, clamped at 1.
#include <cstdio>
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CSimManager { float SimRadius, SimFadeBegin, Zoom; };
static CSimManager g_manager;
static C3DVector g_camera(1.0f, 2.0f, 3.0f);

static float CalculateFadeOut(const C3DVector& position)
{
    float fade = 0.0f;
    float distance = (position - g_camera).GetMagnitude();
    const CSimManager* manager = &g_manager;
    float fadeBegin = manager->SimFadeBegin * manager->Zoom;
    fadeBegin *= manager->SimRadius;
    if (distance > fadeBegin) {
        float radius = manager->Zoom * manager->SimRadius;
        fade = (distance - fadeBegin) / (radius - fadeBegin);
        if (fade > 1.0f)
            fade = 1.0f;
    }
    return fade;
}

static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    g_manager.SimRadius = 10.0f; g_manager.SimFadeBegin = 0.5f; g_manager.Zoom = 2.0f;   // radius 20, begin 10
    int ok = 1;
    if (!Near(CalculateFadeOut(C3DVector(1.0f, 2.0f, 3.0f)), 0.0f)) ok = 0;          // at the camera
    if (!Near(CalculateFadeOut(C3DVector(1.0f, 12.0f, 3.0f)), 0.0f)) ok = 0;         // exactly at the begin
    if (!Near(CalculateFadeOut(C3DVector(16.0f, 2.0f, 3.0f)), 0.5f)) ok = 0;         // half way (15)
    if (!Near(CalculateFadeOut(C3DVector(1.0f, 2.0f, 21.0f)), 0.8f)) ok = 0;         // 18
    if (!Near(CalculateFadeOut(C3DVector(1.0f, 2.0f, 103.0f)), 1.0f)) ok = 0;        // clamped
    std::printf(ok ? "00b806e0_TEST PASS\n" : "00b806e0_TEST FAIL\n");
    return ok ? 0 : 1;
}