// Behaviour test for 00c41a70: src/00c41a70.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CCombatant::ComputeEffectiveRange()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatant::ComputeEffectiveRange() -- angle between two stored vectors (via |a-b| / |a+b|) mapped onto a linear range factor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C41A70, 320 bytes (incl. int3 padding); one caller,
// CCombatant::ComputeDerivedCombatStats 0x00C41FB0 (call at 0x00C4232C). No PDB class header exists for this
// module, so `this` is a local descriptive model: two 3-vectors at +0x114/+0x120, two unsigned angle limits at
// +0x190/+0x194 and the far-end factor at +0x198.
// Shape: the half-angle between the vectors is atan2(|a-b|, |a+b|); `*229.183f` (0x012B7B7C = 4 * 180/pi)
// turns it into the full angle in degrees, and the reflex case (|a+b| < |a-b|) swaps the atan2 operands
// (`fxch ; fpatan`) where the other branch subtracts from 360 (0x01238020). The unsigned limits convert with
// VC's `fild ; jge ; fadd 2^32` (0x0122DCB4) idiom; at or below the low limit the factor is 1.0 (0x0122DED8),
// at or above the high limit it is m_farFactor, and in between it is interpolated linearly. The in-between
// value accumulates in one local (`delta *= m_farFactor - 1.0f;` then the divide): the one-expression and
// named-delta spellings load m_farFactor first and need an fxch.
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float Length() const { return sqrt(X * X + Y * Y + Z * Z); }
};

struct CCombatant {
    unsigned char _pad0[0x114];
    C3DVector m_vecA;                   // +0x114
    C3DVector m_vecB;                   // +0x120
    unsigned char _pad12c[0x64];
    unsigned long m_minAngle;           // +0x190
    unsigned long m_maxAngle;           // +0x194
    float m_farFactor;                  // +0x198
    float ComputeEffectiveRange() const;
};

float CCombatant::ComputeEffectiveRange() const
{
    C3DVector diff = m_vecA - m_vecB;
    C3DVector sum = m_vecA + m_vecB;
    float lenDiff = diff.Length();
    float lenSum = sum.Length();
    float angle;
    if (lenSum < lenDiff)
        angle = (float)atan2(lenSum, lenDiff) * 229.18311f;
    else
        angle = 360.0f - (float)atan2(lenDiff, lenSum) * 229.18311f;
    float minAngle = (float)m_minAngle;
    float maxAngle = (float)m_maxAngle;
    if (angle <= minAngle)
        return 1.0f;
    if (angle >= maxAngle)
        return m_farFactor;
    float delta = angle - minAngle;
    delta *= m_farFactor - 1.0f;
    return delta / (maxAngle - minAngle) + 1.0f;
}

// ---- stubs and driver ----
#include <math.h>
static float Ref(const CCombatant& c)
{
    float dx = c.m_vecA.X - c.m_vecB.X, dy = c.m_vecA.Y - c.m_vecB.Y, dz = c.m_vecA.Z - c.m_vecB.Z;
    float sx = c.m_vecA.X + c.m_vecB.X, sy = c.m_vecA.Y + c.m_vecB.Y, sz = c.m_vecA.Z + c.m_vecB.Z;
    float ld = (float)sqrt(dx * dx + dy * dy + dz * dz), ls = (float)sqrt(sx * sx + sy * sy + sz * sz);
    float angle = ls < ld ? (float)atan2(ls, ld) * 229.18311f : 360.0f - (float)atan2(ld, ls) * 229.18311f;
    float lo = (float)c.m_minAngle, hi = (float)c.m_maxAngle;
    if (angle <= lo) return 1.0f;
    if (angle >= hi) return c.m_farFactor;
    return (angle - lo) * (c.m_farFactor - 1.0f) / (hi - lo) + 1.0f;
}
static bool Near(float a, float b) { return fabs(a - b) <= 1e-3f * (fabs(b) + 1.0f); }
int main()
{
    static CCombatant c; memset(&c, 0, sizeof c);
    c.m_minAngle = 30; c.m_maxAngle = 330; c.m_farFactor = 3.0f;
    // A and B the same: |a-b| = 0 -> angle = 360 - 0 = 360 >= the high limit -> the far factor
    c.m_vecA = C3DVector(1, 0, 0); c.m_vecB = C3DVector(1, 0, 0);
    CHECK(c.ComputeEffectiveRange() == 3.0f);
    // opposite: |a+b| = 0 < |a-b| -> angle = atan2(0, 2)*k = 0 <= the low limit -> 1.0 (reflex branch)
    c.m_vecB = C3DVector(-1, 0, 0);
    CHECK(c.ComputeEffectiveRange() == 1.0f && Ref(c) == 1.0f);
    // perpendicular: 90-degree half... angle = 360 - atan2(sqrt2, sqrt2)*229.18 = 360 - 180 = 180 -> interpolated
    c.m_vecB = C3DVector(0, 1, 0);
    float r = c.ComputeEffectiveRange();
    CHECK(Near(r, Ref(c)) && r > 1.0f && r < 3.0f);
    // a range of directions against the reference
    for (int i = 0; i < 36; ++i) {
        float t = i * 0.1745f;
        c.m_vecB = C3DVector((float)cos(t), (float)sin(t), 0.25f);
        CHECK(Near(c.ComputeEffectiveRange(), Ref(c)));
    }
    // limits above 2^31 convert as unsigned
    c.m_minAngle = 0x80000000UL; c.m_maxAngle = 0x90000000UL;
    CHECK(c.ComputeEffectiveRange() == 1.0f);
    c.m_minAngle = 0; c.m_maxAngle = 10; c.m_farFactor = 7.5f;
    c.m_vecA = C3DVector(1, 0, 0); c.m_vecB = C3DVector(0, 1, 0);
    CHECK(c.ComputeEffectiveRange() == 7.5f);
    if (fails == 0) printf("00c41a70_TEST PASS\n");
    return fails;
}
