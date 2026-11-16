// 00a89e40 C3DMesh2::GetDummyObjectPositionAnimated
// Inline-budget: original SetMultiply and PostMultiply expand in retail; ordinary T74/200/400 leave calls, T800 expands the original chain.
// Leaf: ?GetDummyObjectPositionAnimated@C3DMesh2@@UBEXABVC3DDummyObject@@AAV2@ABVC3DAnimationBlendState@@2MAAVC3DAnimationManager@@_N@Z
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
void C3DMesh2::GetDummyObjectPositionAnimated(const C3DDummyObject& dummy, C3DDummyObject& dest, const C3DAnimationBlendState& anim1,const C3DAnimationBlendState& anim2,float weight2, C3DAnimationManager& anim_manager,bool use_morph_scale)const
{
    dest.BoneIndex=dummy.BoneIndex;
    if(dummy.GetBoneIndex()!=-1) {
        long bone_index=dummy.GetBoneIndex();
        const C3DBone2* bone=GetBone(bone_index);
        CMatrix3x4& target=dest.Transform;
        target.SetMultiply(dummy.Transform,bone->PeekLocalizationMatrix());
        CMatrix3x4 bone_transform;
        C3DVector morph_scale;
        anim_manager.CalcBlendedBoneTransformation(anim1,anim2,weight2,*this,bone_index,bone_transform,morph_scale);
        if(use_morph_scale) dest.Transform.PreScale(morph_scale);
        else dest.Transform.SetTranslateOnly(dest.Transform.GetTranslation().GetScaled(morph_scale));
        dest.Transform.PostMultiply(bone_transform);
    }else dest.Transform=dummy.Transform;
}