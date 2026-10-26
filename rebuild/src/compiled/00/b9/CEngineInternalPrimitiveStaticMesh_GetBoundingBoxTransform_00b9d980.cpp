// CEngineInternalPrimitiveStaticMesh::GetBoundingBoxTransform(CMatrix3x4& transform) -- the mesh's object-space bounding box as a matrix, post-multiplied by the object transform; always true
// Leaf: GetBoundingBoxTransform
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9D980, 510 bytes (+2 int3), `ret 4`. Ego_r.pdb:
//   ?GetBoundingBoxTransform@CEngineInternalPrimitiveStaticMesh@@UAE_NAAVCMatrix3x4@@@Z (virtual).
// Callees: C3DBoundingBox::CalculateMatrix(CMatrix3x4&) const @ 0x00ADEBC0 (on a 24-byte copy of the box).
// Layout: engine/CEngineInternalPrimitiveStaticMesh.h -- CEngineInternalPrimitiveMeshBase::MeshInfo (C3DMeshInfo*) is the
//   header's `_pad_0x50` word, ObjectTransform (CMatrix3x4) the opaque bytes at +0x70; engine/C3DMeshInfo.h BoundingBox +0x10.
//   `this` is the shared header; the virtual is written as a __fastcall free function (ecx = this, edx unused, `ret 4`).
// Shape: the box is copied into a named frame local in its own `{}` scope, and C3DVector has a user copy constructor --
//   that schedules the first dword load ahead of the `add eax,10h` as retail does (an implicit copy adds first); then `transform *= ObjectTransform`:
//   a `rep movsd` copy of the left operand into the same frame words (lexical scope) and the twelve sums written to the
//   destination. VC7.1 reassociates the three-term sums; the forms below reproduce retail's x87 term order (rows 1..3.1
//   x,y,z; 3.2/3.3 and the translation row y,z,x) -- found by a per-element search over the 12 sum forms.
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/C3DMeshInfo.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    __forceinline CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a(*this);
        E11 = a.E11 * b.E11 + a.E13 * b.E31 + a.E12 * b.E21;
        E12 = a.E11 * b.E12 + a.E13 * b.E32 + a.E12 * b.E22;
        E13 = a.E11 * b.E13 + a.E13 * b.E33 + a.E12 * b.E23;
        E21 = a.E21 * b.E11 + a.E23 * b.E31 + a.E22 * b.E21;
        E22 = a.E21 * b.E12 + a.E23 * b.E32 + a.E22 * b.E22;
        E23 = a.E21 * b.E13 + a.E23 * b.E33 + a.E22 * b.E23;
        E31 = a.E31 * b.E11 + a.E33 * b.E31 + a.E32 * b.E21;
        E32 = a.E31 * b.E12 + (a.E32 * b.E22 + a.E33 * b.E32);
        E33 = a.E31 * b.E13 + (a.E32 * b.E23 + a.E33 * b.E33);
        E41 = (a.E41 * b.E11 + (a.E43 * b.E31 + a.E42 * b.E21)) + b.E41;
        E42 = (a.E41 * b.E12 + (a.E42 * b.E22 + a.E43 * b.E32)) + b.E42;
        E43 = (a.E41 * b.E13 + (a.E42 * b.E23 + a.E43 * b.E33)) + b.E43;
        return *this;
    }
};

class C3DBoundingBox { public:
    C3DVector MinCoord;                            // +0x00
    C3DVector MaxCoord;                            // +0x0c
    void CalculateMatrix(CMatrix3x4& matrix) const;   // 0x00ADEBC0
};

static __forceinline C3DMeshInfo* GetMeshInfo(const CEngineInternalPrimitiveStaticMesh* self)
{
    return *(C3DMeshInfo* const*)self->_pad_0x50;      // CEngineInternalPrimitiveMeshBase::MeshInfo (+0x50)
}

bool __fastcall GetBoundingBoxTransform(CEngineInternalPrimitiveStaticMesh* self, int /*edx*/, CMatrix3x4& transform)
{
    {
        C3DBoundingBox box(*(const C3DBoundingBox*)GetMeshInfo(self)->BoundingBox);
        box.CalculateMatrix(transform);
    }
    transform *= *(const CMatrix3x4*)self->ObjectTransform;
    return true;
}