#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DANIMATEDMESHES_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DANIMATEDMESHES_H
// New shared header (agentNF77, round 7, row 0x00BD1F70 Render2DPrimitive; NF74's draft re-based): CEnginePrimitiveManager2DAnimatedMeshes.
// RTTI .?AVCEnginePrimitiveManager2DAnimatedMeshes@@ : CEnginePrimitiveManagerAnimatedMeshesBase : CEnginePrimitiveManagerMeshBase : ...
// : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil (ghidra_out/rtti_bases.tsv); no members of its own (Ego_r.pdb). Retail vtable
// slots (rebuild/manifest/rtti-vtables.tsv): 0 scalar deleting dtor 0x00BD86A0, 1 CreateInternalPrimitive 0x00BD6B90, 2 GetSupportedTypes
// 0x00BD28A0, 5 RenderSubPrimitive 0x00BD7A00, 10 Render2DPrimitive 0x00BD1F70. Methods: the Ego_r.pdb set
// (??0..@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, CreateInternalPrimitive / GetSupportedTypes / Render2DPrimitive / RenderSubPrimitive).
// Declarations only. Needs engine/CEnginePrimitiveManagerAnimatedMeshesBase.h's prerequisites.
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
class CEngineScene; template<class T> class CArray;
class CEnginePrimitiveManager2DAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase {
public:
    CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>& scenes);
    virtual ~CEnginePrimitiveManager2DAnimatedMeshes();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);                 // slot 1  0x00BD6B90
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;                                      // slot 2  0x00BD28A0
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub_primitive, ERenderPassType pass);              // slot 5  0x00BD7A00
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);   // slot 10 0x00BD1F70
};
#endif
