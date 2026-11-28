// GetRandomPointInSphere(float radius) -- a random point inside a sphere. Descriptive name, not a PDB name:
//   no symbol in Ego_r, FableWin or Ego_d. The landed caller CAmbientTrigger::Play 0x00C62530 already binds
//   ?GetRandomPointInSphere@@YG?AVC3DVector@@M@Z here.
// Leaf: ?GetRandomPointInSphere@@YG?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62370, 118 bytes, `ret 4`. The hidden result pointer is in esi
//   (same-TU static convention). The catalog label Quaternion_FromAxisAngle is propagated.
// Callees: SetLungedThisFrame 0x00C62350 (defined in this TU, same body as the landed row, so the three
//   calls stay out of line) and _CIacos.
// Shape: phi = u*2pi, theta = acos(2u-1), r = u*radius,
//   (sin(theta)cos(phi)r, cos(theta)sin(phi)r, cos(theta)r).
#include <math.h>
#pragma intrinsic(sin, cos, acos)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
extern "C" __declspec(dllimport) int __cdecl rand();
extern float g_scale_0129c81c;                              // the global the landed 0x00C62350 row already uses
__declspec(noinline) float SetLungedThisFrame()             // 0x00C62350
{
    return (float)rand() * g_scale_0129c81c;
}
static __declspec(noinline) C3DVector __stdcall GetRandomPointInSphere(float radius)
{
    float phi = SetLungedThisFrame() * 6.28318548f;
    float theta = (float)acos(SetLungedThisFrame() * 2.0f - 1.0f);
    float r = SetLungedThisFrame() * radius;
    C3DVector point((float)sin(theta) * (float)cos(phi) * r, (float)cos(theta) * (float)sin(phi) * r, (float)cos(theta) * r);
    return point;
}
// Same-TU caller. External linkage so the static helper is not discarded. Not a retail function.
struct Holder { C3DVector offset; };
void UseRandomPoint(Holder* h, float radius)
{
    h->offset = GetRandomPointInSphere(radius);
}