#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMESHBASE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMESHBASE_H
// Retail opt-in (Codex pass18): original full hierarchy, fields and native sizes, Ego_r/FableWin TPI.
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class C3DMeshInfo;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_COLLECTSTATS
#include "stlport/fable_map.h"
#include "rebuild_abi.h"
#endif
class CEngineRenderInfoMeshBase;
class CEngineInternalPrimitiveMorphInfo;
class CEnginePrimitiveSimulationInfo;
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveOcclusionBase {
public:
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
 CEngineInternalPrimitiveMeshBase(long type,unsigned long mask):CEngineInternalPrimitiveOcclusionBase(type,mask),MeshInfo(0){}
#endif
 virtual ~CEngineInternalPrimitiveMeshBase();
 virtual const CEngineRenderInfoMeshBase* PeekRenderInfoMeshBase()const=0;
 virtual bool GetTransformFrameStart(CMatrix3x4&)const;
 virtual bool GetTransformFrameEnd(CMatrix3x4&)const;
 virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();
 float GetBoundingSphereRadiusWorldSpace()const;
 const C3DMeshInfo* MeshInfo;
 const C3DMeshInfo* ShadowMeshInfo;
 float Scale;
 CRGBColour TintColour;
 bool BulletTimeAffectsCloth;
 CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;
 CIVCountedPointer<CEngineMeshEffect> MeshEffect;
 CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_COLLECTSTATS
 // Retail opt-in (chunk 1021, row 0x00BEC680): the static Ego_r.pdb overload
 // ?CollectStats@CEngineInternalPrimitiveMeshBase@@SIXABVC3DMeshInfo@@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@K@Z
 // (0x00AA4A90 = retail 0x00BEC680; the virtual UBEX overload, Ego_r 0x00AA4CB0 = retail 0x00BEC8B0, forwards MeshInfo to it, and
 // CEngineInternalPrimitiveRepeatedStaticMesh::CollectStats 0x00B98DE0 calls it too). A declaration only: no layout change.
 static void FABLE_FASTCALL CollectStats(const C3DMeshInfo& mesh_info,std::map<unsigned long,unsigned long>& stats,EEnginePrimitiveStatIndex stat_index,unsigned long flags);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveMeshBase)==112);
#endif
