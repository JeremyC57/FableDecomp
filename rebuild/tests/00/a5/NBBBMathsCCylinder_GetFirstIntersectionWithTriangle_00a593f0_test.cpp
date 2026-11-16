// Behaviour test for 0x00A593F0 NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle: the triangle plane cuts the cylinder in an
// ellipse (minor radius = Radius, major radius = Radius / cos); the triangle is projected into the plane, and the result is, in order:
// the first edge / ellipse crossing whose height lies on the axis segment, else the first triangle vertex inside the ellipse, else
// (when the ellipse centre is inside the triangle) the first vertex; a degenerate triangle or a plane parallel to the axis gives false.
// The checks are geometric (point on the surface, on the plane, outward normal horizontal) plus the order of the fallbacks.
#include <cstdio>
#include <cmath>
#include <vector>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct V2 { float X, Y; V2() {} V2(float x, float y) : X(x), Y(y) {} };
static V3 sub(V3 a, V3 b) { return V3(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V3 add(V3 a, V3 b) { return V3(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V3 mul(V3 a, float s) { return V3(a.X * s, a.Y * s, a.Z * s); }
static float dot(V3 a, V3 b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static V3 cross(V3 a, V3 b) { return V3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static V3 norm(V3 a) { float m = std::sqrt(dot(a, a)); return m > 0 ? mul(a, 1.0f / m) : a; }
static const float D = 1e-4f;   // DELTA_NOTIONAL_ZERO
static bool InTri(const std::vector<V2>& p, V2 pos)
{
    V2 e(p[0].X - p[2].X, p[0].Y - p[2].Y), t(pos.X - p[2].X, pos.Y - p[2].Y);
    float sign = e.X * t.Y - e.Y * t.X;
    for (int i = 0; i != 2; ++i) {
        e = V2(p[i + 1].X - p[i].X, p[i + 1].Y - p[i].Y); t = V2(pos.X - p[i].X, pos.Y - p[i].Y);
        float c = e.X * t.Y - e.Y * t.X;
        if ((sign > 0 && c < 0) || (sign < 0 && c > 0)) return false;
    }
    return true;
}
struct Cyl { float Radius; V3 AxisBottom, AxisTop; };
static bool Try(const Cyl& c, V3 centre, V3 ma, V3 mi, V3 ax, float h, V2 pos, V3& ip, V3& on)
{
    V3 p = add(add(centre, mul(ma, pos.X)), mul(mi, pos.Y));
    V3 out = sub(p, c.AxisBottom);
    float hp = dot(out, ax) / (h * h);
    if (hp > -D && hp < 1.0f + D) { ip = p; on = sub(out, mul(ax, hp)); return true; }
    return false;
}
static int g_path;
static bool Hit(const Cyl& c, const V3* tri, V3& ip, V3& on)
{
    g_path = 0;
    V3 ax = sub(c.AxisTop, c.AxisBottom); float h = std::sqrt(dot(ax, ax));
    V3 n = cross(sub(tri[1], tri[0]), sub(tri[2], tri[0]));
    if (!(dot(n, n) > D * D)) return false;
    n = norm(n);
    float pd = dot(n, tri[0]), ca = dot(n, ax) / h;
    if (ca < -1) ca = -1; else if (ca > 1) ca = 1;
    if (!(std::fabs(ca) > D)) return false;
    V3 centre = add(c.AxisBottom, mul(ax, (pd - dot(n, c.AxisBottom)) / (ca * h)));
    V3 mi = cross(ax, n);
    if (!(dot(mi, mi) > D * D)) {
        mi.Z = 0;
        if (std::fabs(n.X) > std::fabs(n.Y)) { mi.Y = 1; mi.X = std::fabs(n.X) > D ? -(n.Y / n.X) : 0; }
        else { mi.X = 1; mi.Y = std::fabs(n.Y) > D ? -(n.X / n.Y) : 0; }
    }
    mi = norm(mi); V3 ma = norm(cross(mi, n));
    float r1 = c.Radius, r2 = c.Radius / ca;
    std::vector<V2> pts, hits;
    for (int k = 0; k < 3; ++k) { V3 o = sub(tri[k], centre); pts.push_back(V2(dot(o, ma), dot(o, mi))); }
    float r2s = r2 * r2, r1s = r1 * r1, pp = r2 * r1, pps = pp * pp;
    for (int i = 0; i < 3; ++i) {
        V2 s = pts[(i + 2) % 3], e = pts[(i + 3) % 3], l(e.X - s.X, e.Y - s.Y);
        float a = l.X * r1 * l.X * r1 + l.Y * r2 * l.Y * r2;
        float b = 2.0f * (l.Y * (s.Y * r2s) + l.X * (s.X * r1s));
        float cc = s.X * r1 * s.X * r1 + s.Y * r2 * s.Y * r2 - pps;
        float d = b * b - 4.0f * a * cc;
        if (d > 0) {
            d = std::sqrt(d);
            float l1 = (-b + d) / (2 * a), l2 = (-b - d) / (2 * a);
            if (l1 > -D && l1 < 1 + D) hits.push_back(V2(s.X + l1 * l.X, s.Y + l1 * l.Y));
            if (l2 > -D && l2 < 1 + D) hits.push_back(V2(s.X + l2 * l.X, s.Y + l2 * l.Y));
        }
    }
    for (size_t i = 0; i < hits.size(); ++i) if (Try(c, centre, ma, mi, ax, h, hits[i], ip, on)) { g_path = 1; return true; }
    long inside = 0;
    for (size_t i = 0; i < 3; ++i) {
        float lim = r2s;
        if (std::fabs(pts[i].Y) > D) { float sl = pts[i].X / pts[i].Y; sl = sl / r2 * r1; sl = sl * sl; lim = (r2s * sl + r1s) / (sl + 1); }
        if (pts[i].X * pts[i].X + pts[i].Y * pts[i].Y < lim) ++inside;
    }
    if (inside > 0) for (size_t i = 0; i < 3; ++i) if (Try(c, centre, ma, mi, ax, h, pts[i], ip, on)) { g_path = 2; return true; }
    if (InTri(pts, V2(0, 0)) && Try(c, centre, ma, mi, ax, h, pts[0], ip, on)) { g_path = 3; return true; }
    return false;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
int main()
{
    int fails = 0; V3 ip, on;
    Cyl c; c.Radius = 1.0f; c.AxisBottom = V3(0, 0, 0); c.AxisTop = V3(0, 0, 2);
    V3 t1[3] = { V3(0, 0, 1), V3(2, 0, 1), V3(0, 2, 1) };                // crosses the circle: first crossing on the surface
    if (!Hit(c, t1, ip, on) || g_path != 1 || !Near(std::sqrt(ip.X * ip.X + ip.Y * ip.Y), 1) || !Near(ip.Z, 1)
        || !Near(on.Z, 0) || !Near(std::sqrt(on.X * on.X + on.Y * on.Y), 1)) ++fails;
    V3 t2[3] = { V3(0.1f, 0, 1), V3(0.2f, 0, 1), V3(0.1f, 0.1f, 1) };   // wholly inside: the first vertex
    if (!Hit(c, t2, ip, on) || g_path != 2 || !Near(ip.X, 0.1f) || !Near(ip.Y, 0) || !Near(on.X, 0.1f) || !Near(on.Z, 0)) ++fails;
    V3 t3[3] = { V3(-10, -10, 1), V3(10, -10, 1), V3(0, 10, 1) };      // encloses the circle: the first vertex
    if (!Hit(c, t3, ip, on) || g_path != 3 || !Near(ip.X, -10) || !Near(ip.Y, -10) || !Near(on.Z, 0)) ++fails;
    V3 t4[3] = { V3(-10, -10, 5), V3(10, -10, 5), V3(0, 10, 5) };      // above the top cap: no hit
    if (Hit(c, t4, ip, on)) ++fails;
    V3 t5[3] = { V3(1, 0, 0), V3(1, 1, 0), V3(1, 0, 1) };              // plane parallel to the axis
    if (Hit(c, t5, ip, on)) ++fails;
    V3 t6[3] = { V3(0, 0, 0), V3(1, 1, 1), V3(2, 2, 2) };              // degenerate
    if (Hit(c, t6, ip, on)) ++fails;
    V3 t7[3] = { V3(-3, 0, -1.6f), V3(3, 0, 3.6f), V3(0, 3, 1) };      // tilted plane z = 1 + x*13/15: an ellipse, crossing on the surface
    if (!Hit(c, t7, ip, on) || g_path != 1 || !Near(std::sqrt(ip.X * ip.X + ip.Y * ip.Y), 1)
        || !Near(ip.Z, 1 + ip.X * 13.0f / 15.0f) || !Near(on.Z, 0)) ++fails;
    if (fails) { std::printf("00a593f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a593f0_TEST PASS\n");
    return 0;
}