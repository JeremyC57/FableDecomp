// CEngineInternalPrimitive2DAnimatedMesh::GetTransformFrameStart(CMatrix3x4& transform) const -- the object matrix at the start of the frame: the start orientation re-orthogonalised and normalised, CalcObjectMatrix at the start position with a scale of the mesh bounding radius / 100, then rotated -90 degrees about X (Y-up to Z-up); always true
// Leaf: GetTransformFrameStart
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCF760, 464 bytes, `ret 4`. Ego_r.pdb:
//   ?GetTransformFrameStart@CEngineInternalPrimitive2DAnimatedMesh@@UBE_NAAVCMatrix3x4@@@Z (virtual, const).
// Callees: CRightHandedSet::MakePerpendicular 0x00A22E10, C3DVector::Normalise 0x00A14440 (x2; the manifest's "GetScaled" is
//   propagated), CalcObjectMatrix 0x00BEBAA0 (`__fastcall`: matrix in ecx, position in edx, orientation and scale pushed).
//   Constants: 0.01f 0x01230A5C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: engine/CEngineInternalPrimitive2DAnimatedMesh.h is flattened, so the base members are read from its pad words:
//   CEngineInternalPrimitiveMeshBase::MeshInfo (C3DMeshInfo*) +0x50; WorldPos (C3DPositionInterpolator, vptr +0x84)
//   StartPosition +0x88 / EndPosition +0x94; Orientation (C3DOrientationInterpolator, vptr +0xa0) StartOrientation +0xa4 /
//   EndOrientation +0xbc (CRightHandedSet = Up, Forward) -- the same offsets as the landed CalcInterpolatedObjectTransform
//   0x00BCF670; engine/C3DMeshInfo.h BoundingSphere +0x28 (Radius +0x34). The virtual is a __fastcall free function.
// Shape: the orientation copy is a frame local in its own `{}` scope, so the `rep movsd` copy of the matrix taken by the
//   inlined rotation reuses its words. The rotation multiplies every row (translation included, no translation term) by a
//   constant 3x3 built from literals: VC7.1 propagates the constants, folds the `* 1.0f` terms, keeps the `* 0.0f` products
//   (CSE'd across a row, one spilled to the dead argument slot) and turns `+ y * -1.0f` into `- y * 1.0f`.
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"
#include "engine/C3DMeshInfo.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void Normalise();                              // 0x00A14440
};
struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
    void MakePerpendicular();                      // 0x00A22E10
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
struct CMatrix3x3 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    CMatrix3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
        : E11(a), E12(b), E13(c), E21(d), E22(e), E23(f), E31(g), E32(h), E33(i) {}
};
__forceinline void Rotate(CMatrix3x4& m, const CMatrix3x3& b)
{
    CMatrix3x4 a(m);
    m.E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
    m.E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
    m.E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
    m.E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
    m.E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
    m.E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
    m.E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
    m.E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
    m.E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
    m.E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31;
    m.E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32;
    m.E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33;
}
void __fastcall CalcObjectMatrix(CMatrix3x4& out, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEBAA0

bool __fastcall GetTransformFrameStart(const CEngineInternalPrimitive2DAnimatedMesh* self, int /*edx*/, CMatrix3x4& transform)
{
    {
        CRightHandedSet orientation(*(const CRightHandedSet*)self->_pad_0xa4);
        orientation.MakePerpendicular();
        orientation.Up.Normalise();
        orientation.Forward.Normalise();
        const C3DMeshInfo* info = *(C3DMeshInfo* const*)self->_pad_0x50;
        CalcObjectMatrix(transform, *(const C3DVector*)self->_pad_0x88, orientation, ((const float*)info->BoundingSphere)[3] * 0.01f);
    }
    Rotate(transform, CMatrix3x3(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));
    return true;
}
