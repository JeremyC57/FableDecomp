#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V3 operator-(const V3& o) const { return V3(X - o.X, Y - o.Y, Z - o.Z); }
    V3 operator+(const V3& o) const { return V3(X + o.X, Y + o.Y, Z + o.Z); }
    V3 operator*(float s) const { return V3(X * s, Y * s, Z * s); }
    float Dot(const V3& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetMagnitudeAndNormalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); float i = 1.0f / l; X *= i; Y *= i; Z *= i; return l; }
    V3& AccurateNormalise() { GetMagnitudeAndNormalise(); return *this; } };
struct V2 { float X, Y; V2(float x, float y) : X(x), Y(y) {} };
static float Dist2D(const V2& s, const V2& e, const V2& p, float& lp)
{
    float dx = s.X - e.X, dy = s.Y - e.Y, tx = s.X - p.X, ty = s.Y - p.Y;
    float t = (tx * dx + ty * dy) / (dx * dx + dy * dy);
    float ox = dx * t - tx, oy = dy * t - ty;
    lp = t; return ox * ox + oy * oy;
}
struct Thing { float physRadius, combatRadius; unsigned long flags; V3 pos, up; float meshOff, meshHeight;
bool Hit(float radius, const V3& from, const V3& to, V3& hit_position, V3& hit_normal) const
{
    float cylinder_radius = physRadius;
    if (flags & 8) cylinder_radius = combatRadius;
    V3 bottom = up * meshOff + pos;
    V3 top = up * meshHeight + bottom;
    float dx = from.X - to.X, dy = from.Y - to.Y;
    if (dy * dy + dx * dx > 0.0001f) {
        V3 delta = to - from;
        float length = delta.GetMagnitudeAndNormalise();
        float t;
        float d2 = Dist2D(V2(from.X, from.Y), V2(to.X, to.Y), V2(bottom.X, bottom.Y), t);
        radius += cylinder_radius;
        if (t * length > -radius && (t - 1.0f) * length < radius && d2 < radius * radius) {
            hit_position = delta * length * t + from;
            V3 offset = hit_position - bottom;
            V3 axis = top - bottom;
            axis.AccurateNormalise();
            float along = (float)std::fabs(axis.Dot(offset));
            hit_normal = offset - axis * along;
            hit_normal.AccurateNormalise();
            if (hit_position.Z < top.Z && hit_position.Z > bottom.Z) return true;
        }
    }
    return false;
} };
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Thing th; th.physRadius = 1.0f; th.combatRadius = 3.0f; th.flags = 0; th.pos = V3(0, 0, 0); th.up = V3(0, 0, 1);
    th.meshOff = 0.0f; th.meshHeight = 2.0f;
    V3 hp(0, 0, 0), hn(0, 0, 0);
    // segment along X at height 1, passing through the axis: hit at x=0 (closest point), normal from axis is zero-length in X,Y
    bool r = th.Hit(0.5f, V3(-5, 1, 1), V3(5, 1, 1), hp, hn);
    if (!r || !Near(hp.X, 0.0f) || !Near(hp.Y, 1.0f) || !Near(hp.Z, 1.0f) || !Near(hn.Y, 1.0f) || !Near(hn.X, 0.0f)) { std::printf("006a5610_TEST FAIL\n"); return 1; }
    // too far away for the physics radius, but within the creature combat radius
    if (th.Hit(0.5f, V3(-5, 2, 1), V3(5, 2, 1), hp, hn)) { std::printf("006a5610_TEST FAIL\n"); return 1; }
    th.flags = 8;
    if (!th.Hit(0.5f, V3(-5, 2, 1), V3(5, 2, 1), hp, hn)) { std::printf("006a5610_TEST FAIL\n"); return 1; }
    // above the cylinder top: rejected by the Z test
    if (th.Hit(0.5f, V3(-5, 1, 3), V3(5, 1, 3), hp, hn)) { std::printf("006a5610_TEST FAIL\n"); return 1; }
    // vertical segment (no 2D length): rejected
    if (th.Hit(0.5f, V3(0, 0, -5), V3(0, 0, 5), hp, hn)) { std::printf("006a5610_TEST FAIL\n"); return 1; }
    // segment ending before reaching the cylinder: rejected by the t tests
    if (th.Hit(0.5f, V3(-10, 0, 1), V3(-6, 0, 1), hp, hn)) { std::printf("006a5610_TEST FAIL\n"); return 1; }
    std::printf("006a5610_TEST PASS\n");
    return 0;
}