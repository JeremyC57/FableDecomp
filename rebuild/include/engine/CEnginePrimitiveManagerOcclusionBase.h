#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CVertexShader.h"
#include "engine/CCountedPointer.h"
class C3DMesh2;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_SILHOUETTE
class CEngineInternalPrimitiveOcclusionBase;
class CRGBColour;
class CMatrix3x4;
#endif
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_SILHOUETTE
    // Opt-in (chunk 2032, row 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette): Ego_r.pdb / FableWin.pdb protected
    // ?GetSilhouetteColour@CEnginePrimitiveManagerOcclusionBase@@IAE_NAAVCEngineInternalPrimitiveOcclusionBase@@AAVCRGBColour@@@Z (Ego_r
    // donor 0x00A4FCE0, just before IsClippedByNearPlane 0x00A4FD00 = retail 0x00B90090 before 0x00B900B0; the PC body is `xor al,al ;
    // ret 8`) and ?DrawAlphaBoundingBox@CEnginePrimitiveManagerOcclusionBase@@IAEXABVCMatrix3x4@@K@Z (landed 0x00B90CD0). Declarations.
protected:
    bool GetSilhouetteColour(CEngineInternalPrimitiveOcclusionBase& primitive, CRGBColour& colour);   // 0x00B90090
    void DrawAlphaBoundingBox(const CMatrix3x4& box, unsigned long colour);                             // 0x00B90CD0
#endif
private:
    CVertexShader VertexShader;
    CCountedPointer<const C3DMesh2> UnitBox;
    float SilhouetteTreshold;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerOcclusionBase)==0x20);
#endif
#endif
