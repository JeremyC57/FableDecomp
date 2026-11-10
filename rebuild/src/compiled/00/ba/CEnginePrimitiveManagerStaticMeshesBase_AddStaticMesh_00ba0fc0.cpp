// CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh -- add a static mesh primitive and its resource handle.
// Retail: 0x00BA0FC0, 333 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?AddStaticMesh@CEnginePrimitiveManagerStaticMeshesBase@@QAE?AVCEnginePrimitiveHandle@@PBVC3DMeshInfo@@0ABVCMatrix3x4@@MABVCEngineFadeDistance@@JABVCRGBColour@@JM_NK@Z
// Flags: /d2inlT200
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
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
CEnginePrimitiveHandle CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh(const C3DMeshInfo* mesh_info,const C3DMeshInfo* shadow_mesh_info,
 const CMatrix3x4& transform,float scale,const CEngineFadeDistance& fade_dist,long alpha,const CRGBColour& tint_colour,
 long alpha_ref,float alpha_mip_bias,bool persistent,unsigned long epf_flags)
{
 CEngineInternalPrimitiveStaticMesh* primitive=new CEngineInternalPrimitiveStaticMesh;
 primitive->MeshInfo=mesh_info;
 primitive->ShadowMeshInfo=shadow_mesh_info;
 primitive->ObjectTransform=transform;
 primitive->Scale=scale;
 primitive->FadeDistance=fade_dist;
 primitive->EnginePrimitiveFlags=epf_flags;
 primitive->Alpha=(unsigned char)alpha;
 primitive->TintColour=tint_colour;
 primitive->AlphaRef=(unsigned char)alpha_ref;
 primitive->AlphaMipBias=alpha_mip_bias;
 primitive->SetNonPersistent(true);
 primitive->SetNonPersistent(!persistent);
 CEnginePrimitiveHandle handle;
 handle.SetData(primitive);
 bool non_moving=(epf_flags&1)!=0;
 AddPrimitive(*primitive,non_moving);
 return handle;
}