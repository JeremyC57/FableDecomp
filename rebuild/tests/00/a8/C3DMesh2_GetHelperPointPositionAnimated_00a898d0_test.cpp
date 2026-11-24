#include <assert.h>
#include <stdio.h>

#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#define FABLE_RETAIL_C3DBONE2_PEEK
#define FABLE_RETAIL_C3DHELPERPOINT_LAYOUT_PASS27
#include "engine/C3DAnimationBlendState.h"
#include "engine/C3DBone2.h"
#include "engine/C3DHelperPoint.h"

// Use the real point, bone, and transform classes with an isolated animation manager.
#define FABLE_ENGINE_C3DMESH2_H
#define FABLE_ENGINE_C3DANIMATIONMANAGER_H

class C3DMesh2
{
public:
    explicit C3DMesh2(C3DBone2* bones) : Bones(bones) {}

    C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& point, const C3DAnimationBlendState& anim1,
                                           const C3DAnimationBlendState& anim2, float weight2,
                                           class C3DAnimationManager& anim_manager) const;
private:
    C3DBone2* Bones;
};

class C3DAnimationManager
{
public:
    C3DAnimationManager(const C3DMesh2& mesh, const C3DAnimationBlendState& first, const C3DAnimationBlendState& second)
        : Mesh(mesh), First(first), Second(second), Calls(0) {}

    void CalcBlendedBoneTransformation(const C3DAnimationBlendState& anim1, const C3DAnimationBlendState& anim2,
                                      float weight2, const C3DMesh2& mesh, unsigned long bone_index,
                                      CMatrix3x4& transform, C3DVector& scale)
    {
        ++Calls;
        assert(&anim1 == &First && &anim2 == &Second && &mesh == &Mesh);
        assert(weight2 == 0.25f && bone_index == 1);
        transform.E11 = 0; transform.E21 =  1; transform.E31 = 2; transform.E41 = 5;
        transform.E12 = 1; transform.E22 = -2; transform.E32 = 0; transform.E42 = 6;
        transform.E13 = 0; transform.E23 =  1; transform.E33 = 1; transform.E43 = -2;
        scale = C3DVector(-2.0f, 0.5f, 4.0f);
    }

    const C3DMesh2& Mesh;
    const C3DAnimationBlendState& First;
    const C3DAnimationBlendState& Second;
    int Calls;
};

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


CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
C3DAnimationBlendState::C3DAnimationBlendState() {}
C3DAnimationBlendState::~C3DAnimationBlendState() {}

int main()
{
    C3DBone2 bones[2];
    CMatrix3x4& matrix = bones[1].LocalizationMatrix;
    matrix.E11 = 2; matrix.E21 = 1; matrix.E31 = 0; matrix.E41 =  1;
    matrix.E12 = 0; matrix.E22 = 3; matrix.E32 = 1; matrix.E42 = -2;
    matrix.E13 = 1; matrix.E23 = 0; matrix.E33 = 2; matrix.E43 =  3;

    const C3DMesh2 mesh(bones);
    const C3DAnimationBlendState first;
    const C3DAnimationBlendState second;
    C3DAnimationManager manager(mesh, first, second);
    C3DHelperPoint point;
    point.Position = C3DVector(1.0f, 2.0f, 3.0f);
    point.BoneIndex = -1;
    const C3DVector unbound = mesh.GetHelperPointPositionAnimated(point, first, second, 0.25f, manager);
    assert(unbound.X == 1.0f && unbound.Y == 2.0f && unbound.Z == 3.0f && manager.Calls == 0);

    point.BoneIndex = 1;
    const C3DVector bound = mesh.GetHelperPointPositionAnimated(point, first, second, 0.25f, manager);
    assert(bound.X == 88.5f && bound.Y == -11.0f && bound.Z == 41.5f && manager.Calls == 1);
    puts("00a898d0_TEST PASS");
}