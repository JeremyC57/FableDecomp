// CPhysicsMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale) -- build the object's position matrix from an orientation frame, a position and a uniform scale: identity, translated to the position, rows -Right, -Forward, Up, the 3x3 part scaled; remember the scale and its reciprocal
// Leaf: ?SetPositionMatrix@CPhysicsMeshObject@@QAEXABVC3DVector@@ABVCRightHandedSet@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A114B0, 338 bytes, __fastcall, `ret 0Ch`, no frame pointer, `sub esp,30h` (four C3DVector
//   slots: the position copy +0, the cross temporary +0Ch, forward +18h, up +24h). Callees: none (every helper is inline).
//   Data: 1.0f @ 0x0122DED8.
// Identity: Ego_r.pdb / FableWin.pdb ?SetPositionMatrix@CPhysicsMeshObject@@QAEXABVC3DVector@@ABVCRightHandedSet@@M@Z; sole caller
//   the landed constructor 0x00A12CC0. Twin: C3DMeshObject::SetPositionMatrix 0x00A357B0 (same code and the same schedule on
//   PositionMatrix +10h, no Scale / OO_Scale stores); sibling C3DMeshObject::GetPositionMatrixFromRHSet 0x00A358F0 (landed) builds
//   the same rows with the same up / forward copies.
// Headers: engine/CPhysicsMeshObject.h (opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX: the declaration),
//   engine/CMatrix3x4.h (opt-in FABLE_RETAIL_CMATRIX3X4_SETIDENTITY: FableWin.pdb ?SetIdentity@CMatrix3x4@@QAEXXZ; existing
//   FABLE_RETAIL_CMATRIX3X4: PostTranslate(const C3DVector&), PreScale(float)), engine/CRightHandedSet.h, engine/C3DVector.h
//   (opt-in FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP: operator^ with each component parenthesised).
// Shape (each point proven by a grade, see NOTES.md):
//   - up and forward are copies taken before the cross product (X, Y stay on the x87 stack, Z goes through edx to the frame);
//   - the cross product is the unnamed `Forward ^ Up` temporary bound to a const reference: a named `C3DVector right` or
//     `axes.GetRight()` flips the commutative operands of the Y / Z terms (0x00A114E3 / 0x00A114EB);
//   - SetIdentity's chained zeros then ones (the dead diagonal stores stay: position may alias the matrix);
//   - PostTranslate on a by-value copy of position gives `fld pos.X ; fadd E41`, the Y / Z words copied to the frame, and keeps
//     the E33 integer store behind the E43 store (0x00A11559);
//   - the Z of a C3DVector local is never kept on the x87 stack (X and Y are), so the cross product's Z is stored to the frame.
//     Retail stores it late (0x00A1152A, after the twelve identity stores and the position.Y load) because that store is the last
//     instruction of VC7.1's first scheduling region (81-tuple cap) and its reload (0x00A11568) lies in the next one. The three
//     parenthesised components of operator^ are three no-code tuples that put the cut there; they also put the second cut where
//     retail has it (the `mov eax,[esp+3Ch]` for Scale at 0x00A1159F) with no parentheses in the rows.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX
#include "engine/CRightHandedSet.h"
#include "engine/CPhysicsMeshObject.h"
void CPhysicsMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale)
{
    C3DVector up = axes.GetUp();
    C3DVector forward = axes.GetForward();
    const C3DVector& right = axes.GetForward() ^ axes.GetUp();
    PositionMatrix.SetIdentity();
    PositionMatrix.PostTranslate(C3DVector(position));
    PositionMatrix.E11 = -right.X; PositionMatrix.E12 = -right.Y; PositionMatrix.E13 = -right.Z;
    PositionMatrix.E21 = -forward.X; PositionMatrix.E22 = -forward.Y; PositionMatrix.E23 = -forward.Z;
    PositionMatrix.E31 = up.X; PositionMatrix.E32 = up.Y; PositionMatrix.E33 = up.Z;
    PositionMatrix.PreScale(scale);
    Scale = scale;
    OO_Scale = 1.0f / scale;
}