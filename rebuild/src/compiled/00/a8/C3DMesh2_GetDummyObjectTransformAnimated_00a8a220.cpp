// 00a8a220 C3DMesh2::GetDummyObjectTransformAnimated
// Inline-budget: original SetMultiply and PostMultiply expand in retail; ordinary T74/200/400 leave calls, T800 expands the original chain.
// Leaf: ?GetDummyObjectTransformAnimated@C3DMesh2@@UBEXABVC3DDummyObject@@AAVCMatrix3x4@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@_N@Z
// Flags: /d2inlT800
#include "fable_compiler.h"
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_C3DBONE2_PEEK
#define FABLE_RETAIL_C3DDUMMYOBJECT_MESH_FRIEND_PASS23
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRAILING_DATA_PASS23
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CMATRIX3X4_PLAIN_INLINE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_PRESCALE_VECTOR_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DAnimationManager.h"
void C3DMesh2::GetDummyObjectTransformAnimated(const C3DDummyObject& dummy, CMatrix3x4& dest, const C3DAnimationBlendState& anim_state, C3DAnimationManager& anim_manager,bool use_morph_scale)const
{
    if(dummy.GetBoneIndex()!=-1) {
        long bone_index=dummy.GetBoneIndex();
        const C3DBone2* bone=GetBone(bone_index);
        CMatrix3x4 bone_transform;
        C3DVector morph_scale;
        anim_manager.CalcBoneTransformation(anim_state,*this,bone_index,bone_transform,morph_scale);
        CMatrix3x4& target=dest;
        target.SetMultiply(dummy.Transform,bone->PeekLocalizationMatrix());
        if(use_morph_scale) dest.PreScale(morph_scale);
        else dest.SetTranslateOnly(dest.GetTranslation().GetScaled(morph_scale));
        dest.PostMultiply(bone_transform);
    }else dest=dummy.Transform;
}