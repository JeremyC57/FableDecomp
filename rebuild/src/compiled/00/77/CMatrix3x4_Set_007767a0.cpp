// CMatrix3x4::Set(const CRightHandedSet&, const C3DVector&) -- store rhset axes and translate.
// Leaf: ?Set@CMatrix3x4@@QAEXABVCRightHandedSet@@ABVC3DVector@@@Z
// Retail: 0x007767a0, 127 bytes, thiscall.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SET_ORIGINAL_NAMES
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
void CMatrix3x4::Set(const CRightHandedSet& rhset,const C3DVector& translate)
{
    Set(rhset.GetRight(), rhset.GetForward(), rhset.GetUp(), translate);
}