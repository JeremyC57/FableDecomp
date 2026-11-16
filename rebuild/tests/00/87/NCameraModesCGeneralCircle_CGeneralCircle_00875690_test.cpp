// Behaviour test for 0x00875690 NCameraModes::CGeneralCircle::CGeneralCircle(a, b, c): a copy of the row's logic on plain structs.
// The plane axes come from the first edge (ab, normal = ab x ac, in_plane = ab x normal, both normalised), the 2-D circumcentre of
// (0,0), (|ab|,0), (ac.ab, ac.in_plane) is solved in that plane, Origin = a + ab*u + in_plane*v, Radius = |(u,v)|, Ei = unit(a - Origin),
// Ej = unit(Ei x normal). Checked against fixed answers and an independent barycentric circumcentre, plus the geometric invariants
// (Origin equidistant from the three points at Radius, Ei / Ej unit, orthogonal, in the plane, Ej's sign).
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() : X(0), Y(0), Z(0) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V sub(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V add(const V& a, const V& b) { return V(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V mul(const V& a, float s) { return V(a.X * s, a.Y * s, a.Z * s); }
static V cross(const V& a, const V& o) { return V(a.Y * o.Z - a.Z * o.Y, a.Z * o.X - a.X * o.Z, a.X * o.Y - a.Y * o.X); }
static float dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static float len(const V& a) { return (float)sqrt(dot(a, a)); }
static void norm(V& a) { float m = len(a); if (m > 0.0f) a = mul(a, 1.0f / m); }   // AccurateNormalise
struct Circle {
    V Origin; float Radius; V Ei, Ej;
    Circle(const V& pa, const V& pb, const V& pc) {
        V ab = sub(pb, pa);
        V normal = cross(ab, sub(pc, pa));
        V in_plane = cross(ab, normal);
        norm(ab); norm(in_plane);
        float ox = 0.0f, oy = 0.0f;
        float p2x = len(sub(pb, pa)), p2y = 0.0f;
        float p3x = dot(sub(pc, pa), ab), p3y = dot(sub(pc, pa), in_plane);
        float bx = p2x - ox, by = p2y - oy, cx = p3x - ox, cy = p3y - oy;
        float half_c = (p3y * p3y + p3x * p3x) * 0.5f;
        float half_b = p2x * p2x * 0.5f;
        float u = (by * half_c - half_b * cy) / (cx * by - bx * cy);
        float v = (half_c - u * cx) / cy;
        float centre_x = u + ox, centre_y = v + oy;
        Origin = add(add(pa, mul(ab, u)), mul(in_plane, v));
        Radius = (float)sqrt(centre_x * centre_x + centre_y * centre_y);
        Ei = sub(pa, Origin);
        Ej = cross(Ei, normal);
        norm(Ei); norm(Ej);
    }
};
static int fails = 0;
static void near(float got, float want, float tol, const char* what) {
    if (!(fabs(got - want) <= tol)) { ++fails; printf("FAIL %s: got %f want %f\n", what, got, want); }
}
static void nearv(const V& g, const V& w, float tol, const char* what) { near(g.X, w.X, tol, what); near(g.Y, w.Y, tol, what); near(g.Z, w.Z, tol, what); }
// Independent circumcentre: barycentric weights |BC|^2 (|CA|^2+|AB|^2-|BC|^2) etc.
static V circumcentre(const V& A, const V& B, const V& C) {
    float a2 = dot(sub(B, C), sub(B, C)), b2 = dot(sub(C, A), sub(C, A)), c2 = dot(sub(A, B), sub(A, B));
    float wa = a2 * (b2 + c2 - a2), wb = b2 * (c2 + a2 - b2), wc = c2 * (a2 + b2 - c2), s = wa + wb + wc;
    return mul(add(add(mul(A, wa), mul(B, wb)), mul(C, wc)), 1.0f / s);
}
static void check(const V& A, const V& B, const V& C, const char* name) {
    Circle k(A, B, C);
    V cc = circumcentre(A, B, C);
    float scale = len(sub(B, A)) + len(sub(C, A));
    float tol = 2e-4f * scale;
    nearv(k.Origin, cc, tol, name);
    near(k.Radius, len(sub(A, cc)), tol, name);
    near(len(sub(B, k.Origin)), k.Radius, tol, name);
    near(len(sub(C, k.Origin)), k.Radius, tol, name);
    near(len(k.Ei), 1.0f, 1e-4f, name); near(len(k.Ej), 1.0f, 1e-4f, name);
    near(dot(k.Ei, k.Ej), 0.0f, 1e-4f, name);
    V n = cross(sub(B, A), sub(C, A)); norm(n);
    near(dot(k.Ei, n), 0.0f, 1e-4f, name); near(dot(k.Ej, n), 0.0f, 1e-4f, name);
    V ei = sub(A, cc); norm(ei); nearv(k.Ei, ei, 1e-3f, name);
    V ej = cross(ei, n); norm(ej); nearv(k.Ej, ej, 1e-3f, name);     // Ej = Ei x normal, not normal x Ei
}
int main() {
    // Unit circle in the XY plane: centre 0, radius 1, Ei = (1,0,0), normal = ab x ac = (0,0,2), Ej = Ei x normal -> (0,-1,0).
    Circle u(V(1, 0, 0), V(0, 1, 0), V(-1, 0, 0));
    nearv(u.Origin, V(0, 0, 0), 1e-5f, "unit origin"); near(u.Radius, 1.0f, 1e-5f, "unit radius");
    nearv(u.Ei, V(1, 0, 0), 1e-5f, "unit Ei"); nearv(u.Ej, V(0, -1, 0), 1e-5f, "unit Ej");
    // Right triangle: centre = hypotenuse midpoint (2, 1.5, 7), radius 2.5.
    Circle r(V(0, 0, 7), V(4, 0, 7), V(0, 3, 7));
    nearv(r.Origin, V(2.0f, 1.5f, 7.0f), 1e-4f, "right origin"); near(r.Radius, 2.5f, 1e-4f, "right radius");
    nearv(r.Ei, V(-0.8f, -0.6f, 0.0f), 1e-4f, "right Ei");
    // General tilted triangles, both windings, obtuse and acute.
    check(V(1.5f, -2.0f, 0.25f), V(4.0f, 1.0f, 3.0f), V(-2.0f, 5.0f, 1.0f), "tilted");
    check(V(1.5f, -2.0f, 0.25f), V(-2.0f, 5.0f, 1.0f), V(4.0f, 1.0f, 3.0f), "tilted reversed");
    check(V(10.0f, 0.0f, 0.0f), V(-10.0f, 0.5f, 1.0f), V(0.0f, 1.0f, -0.5f), "obtuse");
    check(V(-3.0f, 2.0f, 8.0f), V(5.0f, -1.0f, 6.0f), V(2.0f, 7.0f, -4.0f), "acute");
    if (fails == 0) printf("00875690_TEST PASS\n");
    return 0;
}