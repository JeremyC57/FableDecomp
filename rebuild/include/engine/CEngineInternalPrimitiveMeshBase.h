#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMESHBASE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMESHBASE_H
// Retail opt-in (Codex pass18): original full hierarchy, fields and native sizes, Ego_r/FableWin TPI.
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class C3DMeshInfo;
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
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveMeshBase)==112);
#endif
