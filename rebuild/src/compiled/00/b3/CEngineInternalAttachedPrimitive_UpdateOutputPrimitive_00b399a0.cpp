// CEngineInternalAttachedPrimitive::UpdateOutputPrimitive(float interp) -- place the attachment's output primitive: from the parent's transform at `interp` (animated-mesh parents only), pre-multiplied by the attachment's own transform (flag 2), by the child's cached inverse transform (flag 1; when that cannot be computed the output primitive is disabled) and again by the attachment transform (flag 4), converted back to engine units, set as the output primitive's object transform and enabled; a parent without a transform disables it
// Leaf: ?UpdateOutputPrimitive@CEngineInternalAttachedPrimitive@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B399A0, 1414 bytes, thiscall, `ret 4`, frame 0x60 (transform +0x10, the PreMultiply copy +0x40).
// Identity (two sources): Ego_r.pdb 0x009D8810 and FableWin.pdb 0x02D975C0 ?UpdateOutputPrimitive@CEngineInternalAttachedPrimitive@@QAEXM@Z;
//   called (with 0.0f) by the landed CEnginePrimitiveAttachmentManager::UpdateSceneForPreload 0x00B3A0D0, which binds this spelling.
// Same-TU members (retail order, both PDB name sets): RedoMatrixConversion 0x00B38E30 (QBEXAAVCMatrix3x4@@@Z; its body is visible here,
//   so VC7.1 knows the call leaves edx alone and passes `&transform` again as `mov ecx,edx`, retail 0x00B39EEE),
//   GetParentTransformAnimatedMesh 0x00B39080 (called), GetParentTransform 0x00B398E0 (out-of-line copy unreferenced; expanded here as
//   the `sub eax,2 ; jne` one-case switch), GetChildInverseTransform 0x00B39900 (called); virtual slot 22 SetObjectTransform
//   (CEngineInternalPrimitiveBase vtable +0x58); CEngineInternalPrimitiveBase::Enable is bit 0 of +0x34 (SetEnable inline).
// Layout: engine/CEngineInternalAttachedPrimitive.h (Ego_r.pdb = retail): PrimitiveTransform +0x48, ParentType +0x78, ChildType +0x7c,
//   OutputPrimitive +0x8c (Data +0x90), AttachmentInfo.Flags +0x94, ChildInverseTransform +0xa8, ChildInverseTransformValid +0xd8.
// Shape (each measured against retail, chunk 2006):
//   - the three products are CMatrix3x4::PreMultiply written through SetMultiply on a copy of *this (hdr opt-in
//     FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B): VC7.1 reorders every x87 sum by its own rule, and only this nesting gives retail's 36 term
//     orders (the column-dependent block at 0x00B399F9 and the two uniform blocks); the order written in the sums does not matter.
//   - GetParentTransform is written with a `result` local and `break` (out of line at 0x00B398E0): the direct `return` form expands
//     to the same branch code but leaves the first block's E11 / E12 sums in another order.
//   - the child-inverse step is an if / else whose else arm disables and returns: that return block is laid out after the main path
//     (retail 0x00B39F08), and the disabled-parent arm (`else` of the outer if) last (0x00B39F1A).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_TRANSFORMS
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B
#include "engine/CEngineInternalAttachedPrimitive.h"

void CEngineInternalAttachedPrimitive::RedoMatrixConversion(CMatrix3x4& matrix) const
{
    matrix.E31 *= 0.01f;
    matrix.E32 *= 0.01f;
    matrix.E33 *= 0.01f;
    matrix.E11 *= -0.01f;
    matrix.E12 *= -0.01f;
    matrix.E13 *= -0.01f;
    matrix.E21 *= -0.01f;
    matrix.E22 *= -0.01f;
    matrix.E23 *= -0.01f;
}

bool CEngineInternalAttachedPrimitive::GetParentTransform(CMatrix3x4& transform, float interp)
{
    bool result = false;
    switch (ParentType) {
    case PARENT_TYPE_ANIMATED_MESH:
        result = GetParentTransformAnimatedMesh(transform, interp);
        break;
    }
    return result;
}

bool CEngineInternalAttachedPrimitive::GetChildInverseTransform()
{
    if (ChildInverseTransformValid)
        return true;
    bool valid = false;
    switch (ChildType) {
    case CHILD_TYPE_STATIC_MESH:
        valid = GetChildInverseTransformStaticMesh(ChildInverseTransform);
        break;
    }
    ChildInverseTransformValid = valid;
    return valid;
}

void CEngineInternalAttachedPrimitive::UpdateOutputPrimitive(float interp)
{
    if (OutputPrimitive.IsNull())
        return;
    CEngineInternalPrimitiveBase& output_prim = GetOutputPrimitive();
    CMatrix3x4 transform;
    if (GetParentTransform(transform, interp)) {
        if (AttachmentInfo.Flags & 2)
            transform.PreMultiply(PrimitiveTransform);
        if (AttachmentInfo.Flags & 1) {
            if (GetChildInverseTransform())
                transform.PreMultiply(ChildInverseTransform);
            else {
                output_prim.SetEnable(false);
                return;
            }
        }
        if (AttachmentInfo.Flags & 4)
            transform.PreMultiply(PrimitiveTransform);
        RedoMatrixConversion(transform);
        output_prim.SetObjectTransform(transform);
        output_prim.SetEnable(true);
    } else
        output_prim.SetEnable(false);
}