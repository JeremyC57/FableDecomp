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