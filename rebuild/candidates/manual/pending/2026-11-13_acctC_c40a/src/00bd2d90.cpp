// CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations() -- the frame's blended bone matrices of the mesh (bone space applied), built on the first request of the frame from the frame memory pool
// Leaf: ?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD2D90, 721 bytes, one `ret` (0x00BD3060).
// Name: Ego_r.pdb / FableWin.pdb ?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ.
// Helpers (all inline in retail): CEngine::GetFrameMemoryPool + CSimpleMemoryPool::AllocateType<CMatrix4x4> (FableWin), GetCurrentInterpolate,
//   C3DMeshInfo::GetAnimationManager, CMatrix4x4::AlignedPreMultiplyTransposed (FableWin; aligned copy + AlignedSetMultiplyTransposed, whose SSE
//   kernel is the shared retail-asm helper GF_SSE_MATRIX_MULTIPLY of engine/CMatrix4x4AlignedSSE.h; x87 arm = the accessor text of landed 0x00A9E1E0).
// Retail-asm: 0x00BD2EA7 `movaps xmm0,[edx]` .. 0x00BD2F88 `movaps [edi+30h],xmm4` (hand-written SSE kernel, pointers homed at [esp+20h..28h]).
// Fitted: the named locals mesh_pointer / animation_manager / transform. VC7.1 orders the four x87 products of each element by internal
//   symbol numbering, which every named local of the row shifts (96-variant sweep in NOTES.md); this set gives retail's 3102 3120 1302 2130.
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
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DMESH2_BONECOUNT
#define FABLE_RETAIL_C3DMESHINFO_LOADSTATS
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_GETFRAMEMEMORYPOOL
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
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
    if (RenderInfo.BoneTransforms == 0) {
        const C3DMesh2* mesh_pointer = RenderInfo.Mesh;
        const C3DMesh2& mesh = *mesh_pointer;
        unsigned long bone_count = mesh.GetBoneCount();
        RenderInfo.BoneTransforms = NEngineInternal::GPEngine->GetFrameMemoryPool()->AllocateType<CMatrix4x4>(bone_count);
        float interpolate = GetCurrentInterpolate();
        const C3DVector* bone_scalings = GetBoneScalings(mesh);
        C3DAnimationManager& animation_manager = MeshInfo->GetAnimationManager();
        animation_manager.CalcBlendedMeshTransformations(Anim.StartState, Anim.EndState, interpolate, bone_scalings, mesh,
                                                                       RenderInfo.BoneTransforms, bone_count, true);
        for (unsigned long i = 0; i < bone_count; ++i) {
            CMatrix4x4& transform = RenderInfo.BoneTransforms[i];
            transform.AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[i]);
        }
        GNoBoneTransformationsCalculated += bone_count;
    }
    return RenderInfo.BoneTransforms;
}
