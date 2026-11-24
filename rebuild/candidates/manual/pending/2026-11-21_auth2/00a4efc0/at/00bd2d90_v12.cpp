// CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations() -- the frame's blended bone matrices of the mesh (bone space applied), built on the first request of the frame from the frame memory pool
// Leaf: ?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD2D90, 721 bytes, one `ret` (0x00BD3060).
// Name: Ego_r.pdb / FableWin.pdb ?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ.
// Helpers (all inline in retail): CEngine::GetFrameMemoryPool + CSimpleMemoryPool::AllocateType<CMatrix4x4> (FableWin), GetCurrentInterpolate,
//   C3DMeshInfo::GetAnimationManager, CMatrix4x4::AlignedPreMultiplyTransposed (FableWin; aligned copy + AlignedSetMultiplyTransposed, whose SSE
//   kernel is the shared retail-asm helper GF_SSE_MATRIX_MULTIPLY of engine/CMatrix4x4AlignedSSE.h; x87 arm = the Ego_d AlignedSetMultiply of
//   FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD, the body the out-of-line 0x00ADEFC0 pins), GetRenderInfo (&RenderInfo; Ego_d.pdb proc).
// Retail-asm: 0x00BD2EA7 `movaps xmm0,[edx]` .. 0x00BD2F88 `movaps [edi+30h],xmm4` (hand-written SSE kernel, pointers homed at [esp+20h..28h]).
// Locals (p989 re-port, no fitted names left): the Ego_d.pdb record of this function (debug length 592): `CEngineRenderInfoAnimatedMesh*
//   render_info` at function scope; in the cache-miss block `unsigned long no_bones`, `const C3DVector* bone_morphs`, `float interpolate`,
//   `const C3DMesh2& mesh`; the loop `unsigned long c_bone` { `CMatrix4x4& bone_transform` } -- exactly this text. VC7.1 orders the four
//   x87 products of each element by a count of the function's live symbols: the old fitted set (mesh_pointer / animation_manager /
//   transform) only matched the old ACCESSORS body; under the Ego_d body the Ego_d local set gives retail's order.
#include "fable_compiler.h"
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_GETANIMATIONMANAGER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DMESH2_BONECOUNT
#define FABLE_RETAIL_C3DMESHINFO_LOADSTATS
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD
#define FABLE_ALLOCATETYPE_ASSERT_COND false
#define FABLE_AT_V 12
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_GETFRAMEMEMORYPOOL
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_RENDERINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_GETBONETRANSFORMATIONS
#include "engine/CMatrix4x4.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CEngine.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationManager.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"

namespace NEngineInternal { extern CEngine* GPEngine; }                  // 0x01436EA4
extern unsigned long GNoBoneTransformationsCalculated;                   // 0x0143B95C (descriptive name: per-frame bone counter)

const CMatrix4x4* CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations()
{
    CEngineRenderInfoAnimatedMesh* render_info = GetRenderInfo();
    if (render_info->BoneTransforms == 0) {
        const C3DMesh2& mesh = *render_info->Mesh;
        unsigned long no_bones = mesh.GetBoneCount();
        render_info->BoneTransforms = NEngineInternal::GPEngine->GetFrameMemoryPool()->AllocateType<CMatrix4x4>(no_bones);
        float interpolate = GetCurrentInterpolate();
        const C3DVector* bone_morphs = GetBoneScalings(mesh);
        MeshInfo->GetAnimationManager().CalcBlendedMeshTransformations(Anim.StartState, Anim.EndState, interpolate, bone_morphs, mesh, render_info->BoneTransforms, no_bones, true);
        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone) {
            CMatrix4x4& bone_transform = render_info->BoneTransforms[c_bone];
            bone_transform.AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone]);
        }
        GNoBoneTransformationsCalculated += no_bones;
    }
    return render_info->BoneTransforms;
}