// Behaviour test for CThingFilter_IsInFOV::operator()(const CTargetPoint&) (0x006A7380): links the reconstructed object;
// the C3DVector overload is a recording stub, so the test checks the forwarded position and the passed-through result.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
namespace NPlayerTargeting {
class CTargetPoint { public: unsigned char PThing[0x8]; long Index; C3DVector TargetingPosition; long FrameRejected; };
}
class CThingFilter_IsInFOV {
public:
    C3DVector FromPos; C3DVector Direction; float CosFOV; float FOV; bool AngleTest3D;
    bool operator()(const C3DVector& pos) const;
    bool operator()(const NPlayerTargeting::CTargetPoint& point) const;
};
static const C3DVector* g_seen = 0; static const CThingFilter_IsInFOV* g_self = 0;
bool CThingFilter_IsInFOV::operator()(const C3DVector& pos) const { g_seen = &pos; g_self = this; return pos.X > 0.0f; }
int main()
{
    CThingFilter_IsInFOV f = { };
    NPlayerTargeting::CTargetPoint p = { };
    p.TargetingPosition.X = 1.0f;
    bool in = f(p);
    if (!in || g_seen != &p.TargetingPosition || g_self != &f) { std::printf("006a7380_TEST FAIL 1\n"); return 1; }
    p.TargetingPosition.X = -1.0f;
    if (f(p)) { std::printf("006a7380_TEST FAIL 2\n"); return 1; }
    std::printf("006a7380_TEST PASS\n");
    return 0;
}
