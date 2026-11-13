#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CVertexShader.h"
#include "engine/CCountedPointer.h"
class C3DMesh2;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
class CEngineInternalPrimitiveOcclusionBase;
#endif
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager {
public:
    virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
    virtual ~CEnginePrimitiveManagerOcclusionBase();
public:
    CEnginePrimitiveManagerOcclusionBase(const CEnginePrimitiveManagerOcclusionBase&);
    CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene *>&);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
protected:
    // Opt-in (chunk 2060, row 0x00BA5260): Ego_r.pdb / FableWin.pdb
    // ?PrepareOcclusionTest@CEnginePrimitiveManagerOcclusionBase@@IAEXAAVCEngineInternalPrimitiveOcclusionBase@@W4ERenderPassType@@AAK@Z
    // (protected; retail 0x00B905A0, manifest label agrees). Declaration only.
    void PrepareOcclusionTest(CEngineInternalPrimitiveOcclusionBase& primitive, ERenderPassType pass, unsigned long& query);   // 0x00B905A0
#endif
private:
    CVertexShader VertexShader;
    CCountedPointer<const C3DMesh2> UnitBox;
    float SilhouetteTreshold;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerOcclusionBase)==0x20);
#endif
#endif
