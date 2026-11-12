// C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint&, const C3DAnimationBlendState&, C3DAnimationManager&) const -- the mesh-space position of a helper point under one animation blend state: a bone-relative point is localised by its bone, scaled by the bone's animated scale and moved by the animated bone transformation; a point without a bone is returned as stored
// Leaf: ?GetHelperPointPositionAnimated@C3DMesh2@@UBE?AVC3DVector@@ABVC3DHelperPoint@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A89740, length 389 bytes (`ret 10h`, C3DVector by hidden pointer); C3DMesh2 vtable
//   0x0129DEF0 slot 5 (the one-state overload; slot 4 0x00A898D0 is the two-state one).
// Identity: Ego_r.pdb / FableWin.pdb ?GetHelperPointPositionAnimated@C3DMesh2@@UBE?AVC3DVector@@ABVC3DHelperPoint@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@@Z.
// Callees: C3DAnimationManager::CalcBoneTransformation(const C3DAnimationBlendState&, const C3DMesh2&, unsigned long, CMatrix3x4&,
//   C3DVector&) @ 0x00AA0FA0 (Ego_r.pdb / FableWin.pdb ?CalcBoneTransformation@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@ABVC3DMesh2@@KAAVCMatrix3x4@@AAVC3DVector@@@Z).
//   Inline, expanded in place: CMatrix3x4::TransformPoint(C3DVector&) twice, C3DVector::GetScaled (FableWin.pdb
//   ?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z), the C3DVector copy constructor.
// Layout: shared headers -- C3DMesh2 on its retail base C3DMeshStats (Bones +0x9C), CMatrix3x4 (E11..E43), C3DVector. No shared
//   header exists for C3DBone2 (0x3C, LocalizationMatrix +0x0C), C3DHelperPoint (Position +0, BoneIndex +0x0C) or
//   C3DAnimationManager; they are modelled here with their Ego_r.pdb member names.
// Overlay opt-ins (hdr/README.md): C3DMESH2_HELPERPOINTS, C3DVECTOR_GETSCALED, CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS.
// Shape: the point is copied to a local and transformed in place (TransformPoint's xx / yy / zz copies are the spills at
//   0x00A89762 / 0x00A89766 and, after the scale, 0x00A8980D / 0x00A89825); `&scale` is pushed before the first transform's sums
//   (0x00A8977B). GetScaled's result is a temporary (+0x14) copied back to the local, `scale` being the left operand (`fld scale.k ;
//   fmul pos.k`). Both TransformPoint expansions sum each row Z, Y, X then the translation, the plain left-to-right spelling of
//   the ZYX_SUMS opt-in; with the header's parenthesised default the instructions are the same but the result-pointer load and
//   `pop esi` are scheduled four x87 instructions early. The no-bone return is placed last (0x00A898AA, three integer moves).
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DMesh2.h"

// C3DHelperPoint (Ego_r.pdb, 0x10; no shared header).
class C3DHelperPoint {
public:
    C3DVector Position;                         // +0x00
    long      BoneIndex;                        // +0x0c  -1 = not attached to a bone
};

// C3DBone2 (Ego_r.pdb, 0x3C; no shared header).
class C3DBone2 {
public:
    unsigned long NameCRC;                      // +0x00
    long          Parent;                       // +0x04
    long          OriginalNoChildren;           // +0x08
    CMatrix3x4    LocalizationMatrix;           // +0x0c
};

// C3DAnimationManager (no shared header): only the bone transformation query is used.
class C3DAnimationManager {
public:
    void CalcBoneTransformation(const C3DAnimationBlendState& state, const C3DMesh2& mesh, unsigned long bone,
        CMatrix3x4& transform, C3DVector& scale);   // 0x00AA0FA0
};

C3DVector C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& state,
    C3DAnimationManager& manager) const
{
    if (helper.BoneIndex != -1) {
        C3DVector pos(helper.Position);
        Bones[helper.BoneIndex].LocalizationMatrix.TransformPoint(pos);
        CMatrix3x4 transform;
        C3DVector scale;
        manager.CalcBoneTransformation(state, *this, helper.BoneIndex, transform, scale);
        pos = scale.GetScaled(pos);
        transform.TransformPoint(pos);
        return pos;
    }
    return helper.Position;
}