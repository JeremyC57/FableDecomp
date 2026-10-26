// Behaviour test for 006a7270: src/006a7270.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CThingFilter_IsInFOV::operator()(const C3DVector& pos) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingFilter_IsInFOV::operator()(const C3DVector& pos) const -- true when the point is (almost) at the eye or within the cone: the normalised offset dotted with Direction beats CosFOV (Z ignored unless AngleTest3D)
// Leaf: ??RCThingFilter_IsInFOV@@QBE_NABVC3DVector
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A7270, 260 bytes (+12 int3), `ret 4`, no calls (the landed thing overload
//   0x006A7390 forwards here).
// Globals: 0.01f 0x01230A5C (the "at the eye" squared distance), the 128-entry reciprocal-square-root mantissa table
//   0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: PDB CThingFilter_IsInFOV { C3DVector FromPos +0; C3DVector Direction +0xC; float CosFOV +0x18; float FOV +0x1C;
//   bool AngleTest3D +0x20; } (no engine header).
// Shape: the offset is a frame local; the squared length is taken twice and VC7.1 keeps the X and Y squares on the x87
//   stack between the two sums. The near-eye test is the plain left-to-right GetLengthSquared(); the normalisation is a
//   forced-inline FastNormalise(v) whose own sum is written `Y*Y + (X*X + Z*Z)` (so Z*Z pairs with X*X first, as in retail,
//   while the Y square is loaded first) and which scales the offset IN PLACE with `*=` (a by-value `offset * r` temporary
//   schedules the Z product before the Y one). The reciprocal length is the table-driven fast 1/sqrt with one Newton step
//   (its bit pattern built in the dead argument slot).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector& operator*=(C3DVector& a, float s) { a.X *= s; a.Y *= s; a.Z *= s; return a; }
inline float operator*(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

class CThingFilter_IsInFOV { public:
    C3DVector FromPos;                              // +0x00
    C3DVector Direction;                            // +0x0c
    float     CosFOV;                               // +0x18
    float     FOV;                                  // +0x1c
    bool      AngleTest3D;                          // +0x20
    bool operator()(const C3DVector& pos) const;
};

__forceinline void FastNormalise(C3DVector& v)
{
    v *= FastInvSqrt(v.Y * v.Y + (v.X * v.X + v.Z * v.Z));
}

bool CThingFilter_IsInFOV::operator()(const C3DVector& pos) const
{
    C3DVector offset = pos - FromPos;
    if (offset.GetLengthSquared() < 0.01f)
        return true;
    if (!AngleTest3D)
        offset.Z = 0.0f;
    FastNormalise(offset);
    if (offset * Direction > CosFOV)
        return true;
    return false;
}

// ---- stubs and driver ----
#include <math.h>
unsigned long g_InvSqrtMantissaTable[128];
// The mantissa table: index = exponent parity bit (odd biased exponent -> 1/sqrt(1+m) scaled by 2) and the top six
// mantissa bits; each entry is the top eight result-mantissa bits for the middle of its bucket.
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double v = (i & 64) ? 2.0 / sqrt(m) : sqrt(2.0) / sqrt(m);
        long t = (long)((v - 1.0) * 256.0 + 0.5);
        if (t > 255) t = 255;
        if (t < 0) t = 0;
        g_InvSqrtMantissaTable[i] = (unsigned long)t;
    }
}
static bool Ref(const CThingFilter_IsInFOV& f, const C3DVector& p)
{
    double x = p.X - f.FromPos.X, y = p.Y - f.FromPos.Y, z = p.Z - f.FromPos.Z;
    if (x * x + y * y + z * z < 0.01) return true;
    if (!f.AngleTest3D) z = 0.0;
    double len = sqrt(x * x + y * y + z * z);
    return (x * f.Direction.X + y * f.Direction.Y + z * f.Direction.Z) / len > f.CosFOV;
}
static double Margin(const CThingFilter_IsInFOV& f, const C3DVector& p)
{
    double x = p.X - f.FromPos.X, y = p.Y - f.FromPos.Y, z = p.Z - f.FromPos.Z;
    if (!f.AngleTest3D) z = 0.0;
    double len = sqrt(x * x + y * y + z * z);
    if (len == 0.0) return 1.0;
    return fabs((x * f.Direction.X + y * f.Direction.Y + z * f.Direction.Z) / len - f.CosFOV);
}
int main()
{
    BuildTable();
    // the fast reciprocal square root is close to the real one across magnitudes
    float samples[] = { 0.02f, 0.5f, 1.0f, 2.0f, 3.0f, 10.0f, 1234.5f, 1.0e6f };
    for (int i = 0; i < 8; ++i)
        CHECK(fabs(FastInvSqrt(samples[i]) * sqrt(samples[i]) - 1.0) < 1.0e-3);
    CThingFilter_IsInFOV f;
    f.FromPos = C3DVector(10.0f, 20.0f, 5.0f);
    f.Direction = C3DVector(1.0f, 0.0f, 0.0f);
    f.CosFOV = 0.5f; f.FOV = 1.0471976f; f.AngleTest3D = true;
    // at (or within 0.1 of) the eye: always in view, whatever the direction
    CHECK(f(C3DVector(10.0f, 20.0f, 5.0f)));
    CHECK(f(C3DVector(9.95f, 20.05f, 5.02f)));
    // straight ahead in, straight behind out, 45 degrees in, 75 degrees out
    CHECK(f(C3DVector(30.0f, 20.0f, 5.0f)));
    CHECK(!f(C3DVector(-30.0f, 20.0f, 5.0f)));
    CHECK(f(C3DVector(20.0f, 30.0f, 5.0f)));
    CHECK(!f(C3DVector(10.0f + 2.588f, 20.0f + 9.659f, 5.0f)));
    // a point high above but ahead: out in 3D, in once Z is ignored
    C3DVector high(13.0f, 20.0f, 25.0f);
    CHECK(!f(high));
    f.AngleTest3D = false;
    CHECK(f(high));
    // tilted direction, both modes, against a double-precision reference (points kept clear of the cone edge)
    f.Direction = C3DVector(0.6f, 0.8f, 0.0f); f.CosFOV = 0.7071f;
    int checked = 0;
    for (int mode = 0; mode < 2; ++mode) {
        f.AngleTest3D = mode != 0;
        for (int i = 0; i < 72; ++i) {
            double a = i * 0.0872665;
            for (int k = 0; k < 3; ++k) {
                float r = k == 0 ? 0.5f : (k == 1 ? 7.0f : 300.0f);
                C3DVector p(f.FromPos.X + r * (float)cos(a), f.FromPos.Y + r * (float)sin(a), f.FromPos.Z + r * 0.3f * (float)(k - 1));
                if (Margin(f, p) < 0.01) continue;
                ++checked;
                CHECK(f(p) == Ref(f, p));
            }
        }
    }
    CHECK(checked > 300);
    if (fails == 0) printf("006a7270_TEST PASS\n");
    return fails;
}
