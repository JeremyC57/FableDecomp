// Behaviour test for 0x00A58B50 NBBBMaths::CSphere::GetFirstIntersectionWithTriangle (links the REAL row object: its same-TU
// IsPointInsideTriangleInThePlane, the transform / CTransformWorldCoordsToPlaneCoords projection, STLport push_back<C2DVector> and the
// vector frees all run as compiled). The sphere cut by the triangle plane is a circle; the result is the first edge / circle crossing,
// else the first triangle vertex inside the circle, else (circle centre inside the triangle) the first vertex; a degenerate triangle,
// a plane at or beyond the radius, or a plane through the centre gives false. Fixtures: CSphere {Radius +0, Base +4}, the STLport vector
// {start, finish, end_of_storage}. Out-of-line callees: C3DVector::AccurateNormalise (0x00A14440) = a normalising __fastcall stub
// (counted), DELTA_NOTIONAL_ZERO = 1e-4f. Expected values come from an independent float model of the same geometry.
#include <stdio.h>
#include <math.h>
#pragma comment(linker, "/alternatename:@CallRow@20=?GetFirstIntersectionWithTriangle@CSphere@NBBBMaths@@QBE_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@1@Z")
#pragma comment(linker, "/alternatename:?AccurateNormalise@C3DVector@@QAEAAV1@XZ=@StubNorm@8")
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;
struct V3 { float X, Y, Z; };
struct V2 { float X, Y; };
struct Sphere { float Radius; V3 Base; };
struct Vec3s { V3* start; V3* finish; V3* eos; };
extern "C" bool __fastcall CallRow(const Sphere* self, int edx, const Vec3s* tri, V3* ip, V3* on);
static int g_norm;
extern "C" V3* __fastcall StubNorm(V3* v, int) { ++g_norm; float m = (float)sqrt(v->X * v->X + v->Y * v->Y + v->Z * v->Z); if (m > 0) { v->X /= m; v->Y /= m; v->Z /= m; } return v; }
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static V3 v3(float x, float y, float z) { V3 r = { x, y, z }; return r; }
static V3 sub(V3 a, V3 b) { return v3(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V3 add(V3 a, V3 b) { return v3(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V3 mul(V3 a, float s) { return v3(a.X * s, a.Y * s, a.Z * s); }
static float dot(V3 a, V3 b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static V3 cross(V3 a, V3 b) { return v3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static V3 norm(V3 a) { float m = (float)sqrt(dot(a, a)); return m > 0 ? mul(a, 1.0f / m) : a; }
static const float D = 1e-4f;
static bool InTri(const V2* p, V2 pos)
{
    V2 e = { p[0].X - p[2].X, p[0].Y - p[2].Y }, t = { pos.X - p[2].X, pos.Y - p[2].Y };
    float sign = e.X * t.Y - e.Y * t.X;
    for (int i = 0; i != 2; ++i) {
        e.X = p[i + 1].X - p[i].X; e.Y = p[i + 1].Y - p[i].Y; t.X = pos.X - p[i].X; t.Y = pos.Y - p[i].Y;
        float c = e.X * t.Y - e.Y * t.X;
        if ((sign > 0 && c < 0) || (sign < 0 && c > 0)) return false;
    }
    return true;
}
static int g_path;   // model: 1 crossing, 2 vertex inside, 3 centre inside
static bool Model(const Sphere& s, const V3* tri, V3& ip, V3& on)
{
    g_path = 0;
    V3 n = cross(sub(tri[1], tri[0]), sub(tri[2], tri[0]));
    if (!(dot(n, n) > D * D)) return false;
    n = norm(n);
    float dist = dot(n, tri[0]) - dot(n, s.Base);
    if (!((float)fabs(dist) < s.Radius && (float)fabs(dist) > D)) return false;
    float rs = s.Radius * s.Radius, crs = rs - dist * dist;
    if (crs < 0) crs = 0; else if (crs > rs) crs = rs;
    V3 cc = add(s.Base, mul(n, dist));
    V3 xa; xa.Z = 0;
    if ((float)fabs(n.X) > (float)fabs(n.Y)) { xa.Y = 1; xa.X = -(n.Y / n.X); } else { xa.X = 1; xa.Y = -(n.X / n.Y); }
    xa = norm(xa); V3 ya = norm(cross(xa, n));
    V2 p[3], hit = { 0, 0 }; bool have = false;
    for (int k = 0; k < 3; ++k) { V3 o = sub(tri[k], cc); p[k].X = dot(o, xa); p[k].Y = dot(o, ya); }
    for (int i = 0; i < 3 && !have; ++i) {
        V2 a = p[(i + 2) % 3], b = p[(i + 3) % 3], l = { b.X - a.X, b.Y - a.Y };
        float A = l.X * l.X + l.Y * l.Y, Bc = 2 * (l.X * a.X + l.Y * a.Y), C = a.X * a.X + a.Y * a.Y - rs;
        float d = Bc * Bc - 4 * A * C;
        if (d > 0) {
            float l1 = (-Bc + (float)sqrt(d)) / (2 * A), l2 = (-Bc - (float)sqrt(d)) / (2 * A);
            if (l1 > -D && l1 < 1 + D) { hit.X = a.X + l1 * l.X; hit.Y = a.Y + l1 * l.Y; have = true; g_path = 1; }
            else if (l2 > -D && l2 < 1 + D) { hit.X = a.X + l2 * l.X; hit.Y = a.Y + l2 * l.Y; have = true; g_path = 1; }
        }
    }
    if (!have) {
        long inside = 0;
        for (int k = 0; k < 3; ++k) if (p[k].X * p[k].X + p[k].Y * p[k].Y < crs) ++inside;
        V2 o = { 0, 0 };
        if (inside != 0) { hit = p[0]; have = true; g_path = 2; }
        else if (InTri(p, o)) { hit = p[0]; have = true; g_path = 3; }
    }
    if (!have) return false;
    ip = add(add(cc, mul(xa, hit.X)), mul(ya, hit.Y));
    on = sub(ip, s.Base);
    if (dot(on, on) > D * D) on = norm(on);
    return true;
}
static bool Near(float a, float b) { return fabs(a - b) < 2e-3f; }
static bool NearV(V3 a, V3 b) { return Near(a.X, b.X) && Near(a.Y, b.Y) && Near(a.Z, b.Z); }
static int Run(const Sphere& s, V3 a, V3 b, V3 c, int expect_path, int line)
{
    // every case is rotated about the sphere centre (x by 0.5, then z by 0.3) so the plane normal has non-zero X and Y: for an exactly
    // horizontal plane the original takes x_axis.Y = -(0/0), a NaN that the row and the model share.
    V3 in[3] = { a, b, c }, t[3];
    for (int k = 0; k < 3; ++k) {
        V3 q = sub(in[k], s.Base);
        float cx = (float)cos(0.5), sx = (float)sin(0.5), cz = (float)cos(0.3), sz = (float)sin(0.3);
        V3 r1 = v3(q.X, cx * q.Y - sx * q.Z, sx * q.Y + cx * q.Z);
        t[k] = add(s.Base, v3(cz * r1.X - sz * r1.Y, sz * r1.X + cz * r1.Y, r1.Z));
    }
    Vec3s v = { t, t + 3, t + 3 };
    V3 ip = v3(-99, -99, -99), on = v3(-99, -99, -99), mip = ip, mon = on;
    bool r = CallRow(&s, 0, &v, &ip, &on);
    bool m = Model(s, t, mip, mon);
    Check(r == m, line); Check(g_path == expect_path, line);
    if (r && m) { Check(NearV(ip, mip), line); Check(NearV(on, mon), line); }
    if (!r) { Check(ip.X == -99 && on.X == -99, line); }   // outputs untouched on false
    return r;
}
int main()
{
    Sphere s; s.Radius = 1.0f; s.Base = v3(0.5f, -0.25f, 0.0f);
    // 1. edge crossing (plane z = 0.5; circle radius ~0.866; triangle edges cross it)
    g_norm = 0;
    CHECK(Run(s, v3(0.5f, -0.25f, 0.5f), v3(2.5f, -0.25f, 0.5f), v3(0.5f, 1.75f, 0.5f), 1, __LINE__));
    CHECK(g_norm == 4);   // plane normal, x axis, y axis, outward normal
    // 2. small triangle wholly inside the circle: first vertex
    CHECK(Run(s, v3(0.6f, -0.2f, 0.3f), v3(0.7f, -0.2f, 0.3f), v3(0.6f, -0.1f, 0.3f), 2, __LINE__));
    // 3. triangle enclosing the circle (in-plane (-3,-3) (3,-3) (0,3) about the centre, so (5,5) is outside): centre inside, first vertex
    CHECK(Run(s, v3(-2.5f, -3.25f, -0.4f), v3(3.5f, -3.25f, -0.4f), v3(0.5f, 2.75f, -0.4f), 3, __LINE__));
    // 4. steep plane, crossing (after the rotation its normal is x-dominant: the other x_axis branch)
    CHECK(Run(s, v3(0.9f, -2.0f, -2.0f), v3(0.9f, 2.0f, -2.0f), v3(1.1f, 0.0f, 2.0f), 1, __LINE__));
    // 5. plane beyond the radius, plane through the centre, degenerate triangle, triangle beside the circle: false
    CHECK(!Run(s, v3(0, 0, 1.5f), v3(1, 0, 1.5f), v3(0, 1, 1.5f), 0, __LINE__));
    CHECK(!Run(s, v3(0, 0, 0), v3(1, 0, 0), v3(0, 1, 0), 0, __LINE__));
    CHECK(!Run(s, v3(0, 0, 0.5f), v3(1, 1, 0.5f), v3(2, 2, 0.5f), 0, __LINE__));
    CHECK(!Run(s, v3(5, 5, 0.5f), v3(6, 5, 0.5f), v3(5, 6, 0.5f), 0, __LINE__));
    // 6. second root only (first root outside [0,1]): edge starts inside the circle
    CHECK(Run(s, v3(0.5f, -0.25f, 0.2f), v3(3.0f, -0.25f, 0.2f), v3(0.5f, -3.0f, 0.2f), 1, __LINE__));
    // 7. a larger sphere (Radius 1.5: Radius * Radius differs from Radius), crossing
    Sphere s2; s2.Radius = 1.5f; s2.Base = v3(-1.0f, 2.0f, 0.5f);
    CHECK(Run(s2, v3(-1.0f, 2.0f, 1.2f), v3(2.0f, 2.0f, 1.2f), v3(-1.0f, 5.0f, 1.2f), 1, __LINE__));
    // 8. small triangle in the annulus between the cut circle (radius 0.8 at distance 0.6) and the sphere radius: no crossing, no vertex
    //    inside the circle, centre outside the triangle -> false
    CHECK(!Run(s, v3(1.4f, -0.25f, 0.6f), v3(1.45f, -0.25f, 0.6f), v3(1.4f, -0.2f, 0.6f), 0, __LINE__));
    if (!g_fail) printf("00a58b50_TEST PASS\n");
    return g_fail;
}