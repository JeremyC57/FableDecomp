#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSPRITETRAILS_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSPRITETRAILS_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CVertexShader.h"
#include "engine/CEngineVertexBufferPool.h"
class CEngineInternalPrimitiveSpriteTrail;
class CEnginePrimitiveManagerSpriteTrails:public CEnginePrimitiveManager {
public:
 CEnginePrimitiveManagerSpriteTrails(CArray<CEngineScene*>&);
 virtual ~CEnginePrimitiveManagerSpriteTrails();
 virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);
 virtual void GetSupportedTypes(CEnginePrimitiveTypeList&)const;
 virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);
 virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
 void RenderSpriteTrail(const CEngineInternalPrimitiveSpriteTrail&,bool);
private:
 CVertexShader VertexShaderZSortedUnlit;
 CEngineVertexBufferPool VertexBufferPool;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerSpriteTrails)==48);
#endif
