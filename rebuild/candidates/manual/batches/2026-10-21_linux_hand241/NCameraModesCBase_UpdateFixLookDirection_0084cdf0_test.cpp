#include <cstdio>
#include <cmath>
// Behaviour copy of NCameraModes::CBase::UpdateFixLookDirection (0x0084cdf0): flatten the look direction and tilt it back by at most the turn fraction.
struct V { float X, Y, Z;
    V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); if (l > 0) { X /= l; Y /= l; Z /= l; } }
    V GetPerpendicularXY() const { return V(Y, -X, 0.0f); }
    void Rotate(const V& axis, float turns) {               // Rodrigues rotation by turns * 2pi about a unit axis
        float a = turns * 6.2831853f, c = std::cos(a), s = std::sin(a);
        float d = axis.X * X + axis.Y * Y + axis.Z * Z;
        V cr(axis.Y * Z - axis.Z * Y, axis.Z * X - axis.X * Z, axis.X * Y - axis.Y * X);
        V r(X * c + cr.X * s + axis.X * d * (1 - c), Y * c + cr.Y * s + axis.Y * d * (1 - c), Z * c + cr.Z * s + axis.Z * d * (1 - c));
        *this = r;
    }
};
static void UpdateFixLookDirection(V& lookDir, float angle)
{
    bool lookingUp = lookDir.Z > 0.0f;
    float minCos = (float)std::cos(angle * 6.2831854820251465);
    V dir = lookDir; dir.Normalise();
    V flat(dir.X, dir.Y, 0.0f); flat.Normalise();
    V axis = flat.GetPerpendicularXY();
    float dot = dir.Z * flat.Z + flat.X * dir.X + flat.Y * dir.Y;
    if (dot < minCos) dot = minCos;
    if (dot < -1.0f) dot = -1.0f; else if (dot > 1.0f) dot = 1.0f;
    float sign = lookingUp ? -1.0f : 1.0f;
    float turn = (float)std::acos(dot) * 0.15915493667125702f;
    flat.Rotate(axis, turn * sign);
    lookDir = flat;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
int main()
{
    bool ok = true;
    V d(1.0f, 0.0f, 1.0f);                        // 45 degrees up, unlimited (angle 0.25 turn = 90 degrees)
    UpdateFixLookDirection(d, 0.25f);
    float l = std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
    ok = ok && near(l, 1.0f) && near(d.X, 0.7071f) && near(d.Y, 0.0f) && std::fabs(d.Z) > 0.7f;   // tilted back out of the plane
    V e(0.0f, 1.0f, -1.0f);                       // 45 degrees down, limited to 1/36 turn (10 degrees): dot clamped to cos(10deg)
    UpdateFixLookDirection(e, 1.0f / 36.0f);
    ok = ok && near(std::fabs(e.Z), std::sin(10.0f * 3.14159265f / 180.0f));
    V f(1.0f, 0.0f, 0.0f);                        // already flat: unchanged
    UpdateFixLookDirection(f, 0.25f);
    ok = ok && near(f.X, 1.0f) && near(f.Y, 0.0f) && near(f.Z, 0.0f);
    std::printf(ok ? "0084cdf0_TEST PASS\n" : "0084cdf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
