// Behaviour test for 0x00B24150 CEngine::ClipSphere(const C3DBoundingSphere&) const: true while the sphere can be seen
#include <cstdio>

struct V3 { float X, Y, Z; };
struct Plane { V3 N; float D; };
struct Camera { V3 Pos; float Near, Far; V3 Look; Plane P[4]; };
static float Dot(const V3& a, const V3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static bool ClipSphere(const Camera& c, V3 centre, float radius)          // the row's body
{
    V3 d = { centre.X - c.Pos.X, centre.Y - c.Pos.Y, centre.Z - c.Pos.Z };
    float depth = Dot(d, c.Look);
    if (depth + radius < c.Near || depth - radius > c.Far ||
        Dot(d, c.P[2].N) > radius || Dot(d, c.P[3].N) > radius ||
        Dot(d, c.P[0].N) > radius || Dot(d, c.P[1].N) > radius)
        return false;
    return true;
}

int main()
{
    int ok = 1;
    // camera at the origin looking down +Z, 90-degree frustum: outward normals of x = +-z and y = +-z
    const float k = 0.70710678f;
    Camera c = { {0, 0, 0}, 1.0f, 100.0f, {0, 0, 1}, { {{k, 0, -k}, 0}, {{-k, 0, -k}, 0}, {{0, k, -k}, 0}, {{0, -k, -k}, 0} } };
    V3 ahead = { 0, 0, 10 }, behind = { 0, 0, -10 }, far = { 0, 0, 200 }, left = { -50, 0, 10 }, edge = { 12, 0, 10 };
    ok &= ClipSphere(c, ahead, 1.0f);
    ok &= !ClipSphere(c, behind, 1.0f);
    ok &= !ClipSphere(c, far, 1.0f) && ClipSphere(c, far, 150.0f);
    ok &= !ClipSphere(c, left, 1.0f);
    ok &= !ClipSphere(c, edge, 1.0f) && ClipSphere(c, edge, 2.0f);   // straddling the +x plane counts as visible
    if (ok) std::printf("00b24150_TEST PASS\n"); else std::printf("00b24150_TEST FAIL\n");
    return ok ? 0 : 1;
}