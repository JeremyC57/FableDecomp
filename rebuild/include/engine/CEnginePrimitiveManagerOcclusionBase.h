#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CVertexShader.h"
#include "engine/CCountedPointer.h"
class C3DMesh2;
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager {
public:
    virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
    virtual ~CEnginePrimitiveManagerOcclusionBase();
public:
    CEnginePrimitiveManagerOcclusionBase(const CEnginePrimitiveManagerOcclusionBase&);
    CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene *>&);
private:
    CVertexShader VertexShader;
    CCountedPointer<const C3DMesh2> UnitBox;
    float SilhouetteTreshold;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerOcclusionBase)==0x20);
#endif
#endif
