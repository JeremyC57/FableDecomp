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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_RENDERUNTEXTURED
    // Retail opt-in (wf8, row 0x00B903D0): Ego_r.pdb / FableWin.pdb
    // ?RenderUntexturedStaticMeshPrimitive@CEnginePrimitiveManagerOcclusionBase@@AAEXPAVC3DPrimitive2@@ABVCMatrix3x4@@@Z (private).
private:
    void RenderUntexturedStaticMeshPrimitive(class C3DPrimitive2* primitive, const class CMatrix3x4& transform);   // 0x00B903D0
#endif
private:
    CVertexShader VertexShader;
    CCountedPointer<const C3DMesh2> UnitBox;
    float SilhouetteTreshold;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerOcclusionBase)==0x20);
#endif
#endif
