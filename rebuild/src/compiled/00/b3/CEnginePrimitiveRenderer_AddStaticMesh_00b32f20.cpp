// CEnginePrimitiveRenderer::AddStaticMesh(...) -- forward the eleven arguments to the static-meshes manager at +0x280 and return its by-value result (the hidden return pointer is passed on and returned)
// Leaf: ?AddStaticMesh@CEnginePrimitiveRenderer@@QAE?AVCEnginePrimitiveHandle@@PBVC3DMeshInfo@@0ABVCMatrix3x4@@MABVCEngineFadeDistance@@JABVCRGBColour@@JM_NK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32F20, length 78 bytes.
// Callees: CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh @ 0x00BA0FC0 (on the manager; the same twelve stack words). Shape: the hidden pointer (the first
// stack word) is kept in esi and returned; the arguments are re-pushed last-first in eax/edx pairs with the manager load scheduled in the middle; `ret 30h`.
// NF52 (round 7): moved onto the shared headers so the callee binds Ego_r/FableWin
//   ?AddStaticMesh@CEnginePrimitiveManagerStaticMeshesBase@@QAE?AVCEnginePrimitiveHandle@@PBVC3DMeshInfo@@0ABVCMatrix3x4@@MABVCEngineFadeDistance@@JABVCRGBColour@@JM_NK@Z
//   and the row is the PDB ?AddStaticMesh@CEnginePrimitiveRenderer@@... (engine/CEnginePrimitiveRenderer.h opt-in _ADDSTATICMESH): the manager is
//   reached through FableWin.pdb's inline GetStaticMeshManager() (the CCountedPointer Data word at +0x280, opt-in _STATICMESHMANAGER, as the
//   landed CEnginePrimitiveRenderUtil::AddGeneratedObjects 0x00BF18B0 does). Was local SHandle_00b32f20 / CEnginePrimitiveManagerStaticMeshesBase /
//   CEnginePrimitiveRenderer models with `long` parameters.
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_STATICMESHMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEnginePrimitiveRenderer.h"

CEnginePrimitiveHandle CEnginePrimitiveRenderer::AddStaticMesh(const C3DMeshInfo* mesh_info, const C3DMeshInfo* shadow_mesh_info,
                                                               const CMatrix3x4& transform, float scale, const CEngineFadeDistance& fade_dist,
                                                               long alpha, const CRGBColour& tint_colour, long alpha_ref, float alpha_mip_bias,
                                                               bool persistent, unsigned long epf_flags)
{
    return GetStaticMeshManager().AddStaticMesh(mesh_info, shadow_mesh_info, transform, scale, fade_dist, alpha, tint_colour, alpha_ref,
                                                alpha_mip_bias, persistent, epf_flags);
}
