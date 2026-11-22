// CMatrix3x4::Orthonormalise() -- orthonormalises the basis and resets the translation.
// Leaf: ?Orthonormalise@CMatrix3x4@@QAEXXZ
// Flags: default
#define FABLE_RETAIL_CMATRIX3X4_ORTHONORMALISE
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_ALL_GROUP_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#include "engine/CMatrix3x4.h"

void CMatrix3x4::Orthonormalise()
{
    C3DVector x(E11, E12, E13);
    C3DVector y(E21, E22, E23);
    C3DVector z;
    x.Normalise();
    (z = x.Cross(y)).Normalise();
    (y = z.Cross(x)).Normalise();
    SetIdentity();
    E11 = x.X; E12 = x.Y; E13 = x.Z;
    E21 = y.X; E22 = y.Y; E23 = y.Z;
    E31 = z.X; E32 = z.Y; E33 = z.Z;
}

#include <math.h>
#include <stdio.h>
#include <string.h>

unsigned long GFastOneOverSqrtTable[128];

static void BuildTestTable()
{
    for (unsigned int i = 0; i < 128; ++i) {
        const unsigned long bits = 0x3f000000UL | (i << 17);
        float sample;
        memcpy(&sample, &bits, sizeof(sample));
        const float inverse = (float)(1.0 / sqrt(sample));
        unsigned long inverse_bits;
        memcpy(&inverse_bits, &inverse, sizeof(inverse_bits));
        const unsigned long exponent = (0x17cUL - ((bits >> 23) & 0xff)) >> 1;
        const unsigned long target_exponent = (inverse_bits >> 23) & 0xff;
        GFastOneOverSqrtTable[i] = ((inverse_bits & 0x7fffff) >> 15);
        if (target_exponent > exponent)
            GFastOneOverSqrtTable[i] = 0xff;
        else if (target_exponent < exponent)
            GFastOneOverSqrtTable[i] = 0;
    }
}

static bool near(float value, float expected)
{
    return fabs(value - expected) < 0.004f;
}

int main()
{
    BuildTestTable();

    CMatrix3x4 base;
    base.E11 = 2.0f; base.E12 = 0.0f; base.E13 = 0.0f;
    base.E21 = 3.0f; base.E22 = 4.0f; base.E23 = 0.0f;
    base.E31 = 3.0f; base.E32 = 1.0f; base.E33 = 4.0f;
    base.E41 = 4.0f; base.E42 = -2.0f; base.E43 = 7.0f;
    base.Orthonormalise();
    if (!near(base.E11, 1.0f) || !near(base.E22, 1.0f) || !near(base.E33, 1.0f)
        || !near(base.E12, 0.0f) || !near(base.E13, 0.0f) || !near(base.E21, 0.0f)
        || !near(base.E23, 0.0f) || !near(base.E31, 0.0f) || !near(base.E32, 0.0f)
        || base.E41 != 0.0f || base.E42 != 0.0f || base.E43 != 0.0f)
        return 1;

    base.E11 = 0.0f; base.E12 = 2.0f; base.E13 = 0.0f;
    base.E21 = 4.0f; base.E22 = 3.0f; base.E23 = 0.0f;
    base.Orthonormalise();
    if (!near(base.E12, 1.0f) || !near(base.E21, 1.0f) || !near(base.E33, -1.0f)
        || !near(base.E11, 0.0f) || !near(base.E13, 0.0f)
        || !near(base.E22, 0.0f) || !near(base.E23, 0.0f)
        || !near(base.E31, 0.0f) || !near(base.E32, 0.0f))
        return 2;
    base.E11 = 1.0f; base.E12 = 2.0f; base.E13 = 3.0f;
    base.E21 = 2.0f; base.E22 = -3.0f; base.E23 = 4.0f;
    base.E31 = -2.0f; base.E32 = 3.0f; base.E33 = 9.0f;
    base.Orthonormalise();
    const C3DVector x(base.E11, base.E12, base.E13);
    const C3DVector y(base.E21, base.E22, base.E23);
    const C3DVector z(base.E31, base.E32, base.E33);
    const C3DVector cross = x.Cross(y);
    if (!near(x.X * x.X + x.Y * x.Y + x.Z * x.Z, 1.0f)
        || !near(y.X * y.X + y.Y * y.Y + y.Z * y.Z, 1.0f)
        || !near(z.X * z.X + z.Y * z.Y + z.Z * z.Z, 1.0f)
        || !near(x.X * y.X + x.Y * y.Y + x.Z * y.Z, 0.0f)
        || !near(cross.X, z.X) || !near(cross.Y, z.Y) || !near(cross.Z, z.Z))
        return 3;
    puts("00a56270_TEST PASS");
    return 0;
}