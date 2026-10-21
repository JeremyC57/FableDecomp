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