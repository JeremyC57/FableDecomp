// Behaviour test for 00a58b50 (NBBBMaths::CSphere::GetFirstIntersectionWithTriangle): a standalone copy of the reconstructed math --
// the triangle plane cuts the sphere in a circle; an edge crossing the circle, a vertex inside it, or the centre inside the triangle
// is a contact; a triangle plane outside the radius is not.
#include <cstdio>
#include <cmath>
struct V3 { float x, y, z; };
struct V2 { float x, y; };
static V3 sub(V3 a, V3 b) { V3 r = { a.x - b.x, a.y - b.y, a.z - b.z }; return r; }
static V3 cross(V3 a, V3 b) { V3 r = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; return r; }
static float dot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static V3 norm(V3 a) { float m = std::sqrt(dot(a, a)); V3 r = { a.x / m, a.y / m, a.z / m }; return r; }
static bool Contact(V3 base, float radius, const V3* t)
{
    const float D = 1e-4f;
    V3 n = cross(sub(t[1], t[0]), sub(t[2], t[0]));
    if (!(dot(n, n) > D * D)) return false;
    n = norm(n);
    float dist = dot(n, t[0]) - dot(n, base);
    if (!(std::fabs(dist) < radius && std::fabs(dist) > D)) return false;
    float r2 = radius * radius - dist * dist; if (r2 < 0) r2 = 0;
    V3 c = { base.x + n.x * dist, base.y + n.y * dist, base.z + n.z * dist };
    V3 ax; ax.z = 0;
    if (std::fabs(n.x) > std::fabs(n.y)) { ax.y = 1; ax.x = -(n.y / n.x); } else { ax.x = 1; ax.y = -(n.x / n.y); }
    ax = norm(ax); V3 ay = norm(cross(ax, n));
    V2 p[3];
    for (int i = 0; i < 3; ++i) { V3 d = sub(t[i], c); p[i].x = dot(d, ax); p[i].y = dot(d, ay); }
    for (int i = 0; i < 3; ++i) {
        V2 s = p[(i + 2) % 3]; V2 l = { p[i].x - s.x, p[i].y - s.y };
        float a = l.x * l.x + l.y * l.y, b = 2 * (l.x * s.x + l.y * s.y), cc = s.x * s.x + s.y * s.y - radius * radius;
        float desc = b * b - 4 * a * cc;
        if (desc > 0) { desc = std::sqrt(desc); float l1 = (-b + desc) / (2 * a), l2 = (-b - desc) / (2 * a);
            if ((l1 > -D && l1 < 1 + D) || (l2 > -D && l2 < 1 + D)) return true; }
    }
    for (int i = 0; i < 3; ++i) if (p[i].x * p[i].x + p[i].y * p[i].y < r2) return true;
    return false;
}
int main()
{
    int fails = 0;
    V3 base = { 0, 0, 0 };
    // a tilted plane (normal (1,1,1)/sqrt 3) through q = (0.3,0.3,0.3), 0.52 from the centre
    V3 q = { 0.3f, 0.3f, 0.3f }, u = { 1, -1, 0 }, w = { 1, 1, -2 };
    V3 cut[3] = { { q.x + 20 * u.x, q.y + 20 * u.y, q.z + 20 * u.z }, { q.x - 20 * u.x, q.y - 20 * u.y, q.z - 20 * u.z },
                  { q.x + 20 * w.x, q.y + 20 * w.y, q.z + 20 * w.z } };          // an edge passes through the circle centre
    V3 far_[3] = { { 2 + u.x, 2 + u.y, 2 + u.z }, { 2 - u.x, 2 - u.y, 2 - u.z }, { 2 + w.x, 2 + w.y, 2 + w.z } };   // plane 3.5 away
    V3 small_[3] = { { q.x + 0.05f * u.x, q.y + 0.05f * u.y, q.z }, { q.x - 0.05f * u.x, q.y - 0.05f * u.y, q.z },
                     { q.x + 0.05f * w.x, q.y + 0.05f * w.y, q.z + 0.05f * w.z } };   // all vertices inside the circle
    if (!Contact(base, 1.0f, cut)) ++fails;
    if (Contact(base, 1.0f, far_)) ++fails;
    if (!Contact(base, 1.0f, small_)) ++fails;
    if (fails) { std::printf("00a58b50_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a58b50_TEST PASS\n");
    return 0;
}
