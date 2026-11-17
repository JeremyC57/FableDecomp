// Behaviour test for CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum (0x00BCF040): false when the interpolated world position is
// strictly farther than the max draw distance from the camera; with distance_test_only that is the answer; otherwise the bounding
// sphere centre through the object transform and radius = SafeBoundingRadius * Scale must pass ClipSphere: depth + r >= near,
// depth - r <= far, and the dot with side planes 2, 3, 0, 1 <= r. Model of the row's logic.
#include <cstdio>
struct V { float X, Y, Z; };
static float Dot(V a, V b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct Cam { V pos, look; float Near, Far; V planes[4]; };
static V g_center;                                                       // test hook: the transformed bounding-sphere centre
struct Mesh { V start, end; float max_draw, safe_radius, scale; V centre; float m[12]; };
static bool IsInsideFrustum(const Mesh& me, float interp, const Cam& frustum, bool distance_test_only)
{
    V world_pos = { (me.end.X - me.start.X) * interp + me.start.X, (me.end.Y - me.start.Y) * interp + me.start.Y,
                    (me.end.Z - me.start.Z) * interp + me.start.Z };
    V d = { world_pos.X - frustum.pos.X, world_pos.Y - frustum.pos.Y, world_pos.Z - frustum.pos.Z };
    float distance_squared = Dot(d, d);
    float max_draw_dist = me.max_draw;
    if (distance_squared > max_draw_dist * max_draw_dist) return false;
    if (distance_test_only) return true;
    const float* m = me.m; V c = me.centre;
    V center = { m[0] * c.X + m[3] * c.Y + m[6] * c.Z + m[9], m[1] * c.X + m[4] * c.Y + m[7] * c.Z + m[10],
                 m[2] * c.X + m[5] * c.Y + m[8] * c.Z + m[11] };
    g_center = center;
    float radius = me.safe_radius * me.scale;
    V e = { center.X - frustum.pos.X, center.Y - frustum.pos.Y, center.Z - frustum.pos.Z };
    float depth = Dot(e, frustum.look);
    if (depth + radius < frustum.Near || depth - radius > frustum.Far) return false;
    static const int order[4] = { 2, 3, 0, 1 };
    for (int i = 0; i < 4; ++i) if (Dot(e, frustum.planes[order[i]]) > radius) return false;
    return true;
}
static int fail(int n) { std::printf("00bcf040_TEST FAIL %d\n", n); return 1; }
int main()
{
    Cam cam = { { 0, 0, 0 }, { 0, 0, 1 }, 1.0f, 100.0f, { { 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 } } };
    Mesh me = { { 0, 0, 0 }, { 0, 0, 20 }, 10.0f, 1.0f, 2.0f, { 0, 0, 0 }, { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 10 } };
    if (!IsInsideFrustum(me, 0.5f, cam, false)) return fail(1);          // pos (0,0,10): exactly at the draw distance -> inside
    if (IsInsideFrustum(me, 0.6f, cam, true)) return fail(2);            // 12 > 10
    if (!IsInsideFrustum(me, 0.25f, cam, true)) return fail(3);          // distance only
    me.m[11] = -3.0f;                                                    // depth -3, r 2: -1 < near 1 -> culled
    if (IsInsideFrustum(me, 0.25f, cam, false)) return fail(4);
    if (!IsInsideFrustum(me, 0.25f, cam, true)) return fail(16);         // distance only: the culled sphere is not tested
    me.m[11] = -1.0f;                                                    // depth + r == near exactly -> kept
    if (!IsInsideFrustum(me, 0.25f, cam, false)) return fail(5);
    me.m[11] = 102.0f;                                                   // depth - r == far exactly -> kept
    if (!IsInsideFrustum(me, 0.25f, cam, false)) return fail(6);
    me.m[11] = 102.5f; if (IsInsideFrustum(me, 0.25f, cam, false)) return fail(7);
    me.m[11] = 10.0f;
    for (int p = 0; p < 4; ++p) {                                        // each side plane: on the edge kept, beyond culled
        V& n = cam.planes[p]; me.m[9] = n.X * 2.0f; me.m[10] = n.Y * 2.0f;
        if (!IsInsideFrustum(me, 0.25f, cam, false)) return fail(8 + p);
        me.m[9] = n.X * 2.5f; me.m[10] = n.Y * 2.5f;
        if (IsInsideFrustum(me, 0.25f, cam, false)) return fail(12 + p);
    }
    Mesh mt = { { 0, 0, 0 }, { 0, 0, 0 }, 1000.0f, 1.0f, 1.0f, { 1, 2, 3 }, { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 } };
    IsInsideFrustum(mt, 0.5f, cam, false);                               // centre (1,2,3) through E11..E43 (distinct primes)
    if (g_center.X != 96.0f || g_center.Y != 113.0f || g_center.Z != 137.0f) return fail(20);
    std::printf("00bcf040_TEST PASS\n");
    return 0;
}
