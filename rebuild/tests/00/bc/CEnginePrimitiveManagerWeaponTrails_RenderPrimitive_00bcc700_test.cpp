// Behaviour test for CEnginePrimitiveManagerWeaponTrails::RenderPrimitive (0x00BCC700), g++ model of the retail logic.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct Queued { int count; float distance; unsigned alpha, flags; };
static int RenderPrimitive(bool enable, const V* strip, int points, const V& camera, Queued& q)
{
    if (!enable) return 0;
    if (points != 0) {
        V d = { strip[0].X - camera.X, strip[0].Y - camera.Y, strip[0].Z - camera.Z };
        q.distance = (float)std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z); q.alpha = 0xff; q.flags = 1; ++q.count;
    }
    return 2;
}
int main()
{
    V strip[2] = { { 3, 4, 12 }, { 0, 0, 0 } }; V cam = { 0, 0, 0 };
    Queued q = { 0, 0, 0, 0 };
    int ok = RenderPrimitive(false, strip, 2, cam, q) == 0 && q.count == 0;
    ok &= RenderPrimitive(true, strip, 0, cam, q) == 2 && q.count == 0;
    ok &= RenderPrimitive(true, strip, 2, cam, q) == 2 && q.count == 1 && std::fabs(q.distance - 13.0f) < 1e-4f && q.alpha == 0xff && q.flags == 1;
    std::printf(ok ? "00bcc700_TEST PASS\n" : "00bcc700_TEST FAIL\n");
    return ok ? 0 : 1;
}