// C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet&) -- build an object orientation matrix.
// Leaf: ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z
// Flags:
// Retail: 0x00a358f0, 185 bytes, static fast call with hidden result.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
// Declaration only: this static function does not instantiate or access a C3DMeshObject.
class C3DMeshObject {
public:
    static CMatrix3x4 FABLE_FASTCALL GetPositionMatrixFromRHSet(const CRightHandedSet& set);
};
CMatrix3x4 FABLE_FASTCALL C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet& set)
{
    C3DVector up(set.GetUp());
    C3DVector forward(set.GetForward());
    const C3DVector& right=set.GetRight();
    CMatrix3x4 m;
    m.E11=-right.X; m.E12=-right.Y; m.E13=-right.Z;
    m.E21=-forward.X; m.E22=-forward.Y; m.E23=-forward.Z;
    m.E31=up.X; m.E32=up.Y; m.E33=up.Z;
    m.E41=m.E42=m.E43=0.0f;
    return m;
}