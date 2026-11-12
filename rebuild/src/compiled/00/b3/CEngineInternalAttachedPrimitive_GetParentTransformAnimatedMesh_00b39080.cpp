// CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh(CMatrix3x4& transform, float interp) -- the parent animated mesh's dummy-object transform at the given interpolation point (the parent group child when a group index is set): false without a parent, while the parent's mesh stats are still loading (loading is started) or when the parent has no such dummy; else the dummy transform post-multiplied by the parent's interpolated object transform, with the parent's scale divided back out of the rotation rows
// Leaf: ?GetParentTransformAnimatedMesh@CEngineInternalAttachedPrimitive@@QAE_NAAVCMatrix3x4@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B39080, length 846 bytes (`ret 8`).
// Callees: GetChildPrimitive (CEngineInternalPrimitiveBase slot 12, +0x30), C3DMeshInfo::IsLoadedStats 0x00A23E80, BeginLoadingStats(bool)
//   0x00A24A80, GetMeshStats() const 0x00A25530 (counted pointer by hidden pointer; its destructor COMDAT 0x004C24C0 is called, not
//   expanded), CCRC::Calc 0x004014A0 (CCharString::GetCRC inline), C3DMeshStats::GetDummyObject 0x00A2E920, the stats' slot 8 (+0x20)
//   GetDummyObjectTransformAnimated (two blend states), CEngineInternalPrimitiveAnimatedMesh::CalcInterpolatedObjectTransform (slot 33,
//   +0x84). Constant 1.0f 0x0122DED8.
// Layout: shared headers -- CEngineInternalAttachedPrimitive (ParentHandle +0x80, data +0x84; ParentGroupIndex +0x88; AttachmentInfo
//   +0x94 {ParentDummyName +0x98, ParentDummyIndex +0x9c}); the parent is a CEngineInternalPrimitiveAnimatedMesh (MeshInfo +0x50, Scale
//   +0x58, Anim +0x70 {StartState +0x74, EndState +0x7c}); C3DMeshInfo's Bank +0x04 -> AnimationManager +0x3c0.
// Overlay opt-ins (hdr/README.md): C3DMESHINFO_LOADSTATS, CMESHDATABANK_GETANIMATIONMANAGER, C3DMESHSTATS_DUMMYOBJECTS,
//   CENGINEINTERNALATTACHEDPRIMITIVE_PARENTTRANSFORM.
// Shape: the parent is the attached sibling's GetParentPrimitive + static_cast (as the landed 0x00B395E0); the product is the inline
//   CMatrix3x4::SetMultiply over a named copy of the dummy transform (k-order sums) and the inverse scale the inline PreScale.
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_GETANIMATIONMANAGER
#define FABLE_RETAIL_C3DMESHINFO_GETMESHSTATS
#define FABLE_RETAIL_C3DMESHINFO_LOADSTATS
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_PARENTTRANSFORM
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#include "engine/CEngineInternalAttachedPrimitive.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMeshStats.h"
#include "engine/CCountedPointer.h"

bool CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh(CMatrix3x4& transform, float interp)
{
    CEnginePersistentPrimitiveBase* prim_base = GetParentPrimitive();
    if (!prim_base)
        return false;
    const CEngineInternalPrimitiveAnimatedMesh& parent_prim = *static_cast<const CEngineInternalPrimitiveAnimatedMesh*>(prim_base);
    if (parent_prim.MeshInfo->IsLoadedStats()) {
        CCountedPointer<const C3DMeshStats> stats = parent_prim.MeshInfo->GetMeshStats();
        const C3DDummyObject* dummy = stats->GetDummyObject(AttachmentInfo.ParentDummyName.GetCRC(), AttachmentInfo.ParentDummyIndex);
        if (dummy) {
            stats->GetDummyObjectTransformAnimated(*dummy, transform, parent_prim.Anim.StartState, parent_prim.Anim.EndState, interp,
                                                   parent_prim.MeshInfo->GetAnimationManager(), false);
            CMatrix3x4 object_transform;
            parent_prim.CalcInterpolatedObjectTransform(object_transform, interp, true);
            CMatrix3x4 dummy_transform(transform);
            transform.SetMultiply(dummy_transform, object_transform);
            transform.PreScale(1.0f / parent_prim.Scale);
            return true;
        }
        return false;
    }
    parent_prim.MeshInfo->BeginLoadingStats(false);
    return false;
}