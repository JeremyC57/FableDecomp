// Leaf: ?GetHelperPointPositionAnimated@C3DMesh2@@UBE?AVC3DVector@@ABVC3DHelperPoint@@ABVC3DAnimationBlendState@@1MAAVC3DAnimationManager@@@Z
// Flags:
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM
#define FABLE_RETAIL_C3DHELPERPOINT_LAYOUT_PASS27
#define FABLE_RETAIL_C3DBONE2_PEEK
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DBone2.h"
#include "engine/C3DHelperPoint.h"
#include "engine/C3DAnimationManager.h"

C3DVector C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint& point, const C3DAnimationBlendState& anim1,
                                                  const C3DAnimationBlendState& anim2, float weight2,
                                                  C3DAnimationManager& anim_manager) const
{
    if (point.BoneIndex != -1)
    {
        const unsigned long bone_index = point.BoneIndex;
        const C3DBone2* bone = &Bones[bone_index];
        C3DVector position(point.Position);
        bone->PeekLocalizationMatrix().TransformPoint(position);
        CMatrix3x4 bone_transform;
        C3DVector morph_scale;
        anim_manager.CalcBlendedBoneTransformation(anim1, anim2, weight2, *this, bone_index, bone_transform, morph_scale);
        position = morph_scale.GetScaled(position);
        bone_transform.TransformPoint(position);
        return position;
    }
    return point.Position;
}