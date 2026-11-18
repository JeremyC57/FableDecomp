#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DSTATICMESHES_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DSTATICMESHES_H
// New shared header (agentNF77, round 7, row 0x00B9E950 Render2DPrimitive; NF74's draft re-based): CEnginePrimitiveManager2DStaticMeshes.
// RTTI .?AVCEnginePrimitiveManager2DStaticMeshes@@ : CEnginePrimitiveManagerStaticMeshesBase : CEnginePrimitiveManagerMeshBase : ...
// : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil (ghidra_out/rtti_bases.tsv); no members of its own (Ego_r.pdb). Retail vtable
// slots (rebuild/manifest/rtti-vtables.tsv): 0 scalar deleting dtor 0x00BA92B0, 1 CreateInternalPrimitive 0x00B9E8C0, 2 GetSupportedTypes
// 0x00BA7770, 5 RenderSubPrimitive 0x00BA77B0, 10 Render2DPrimitive 0x00B9E950. Methods: Ego_r + FableWin (??1: FableWin only)
// (??0..@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, ??1 UAE, CreateInternalPrimitive / GetSupportedTypes / Render2DPrimitive / RenderSubPrimitive).
// Declarations only. Needs the retail engine/CEnginePrimitiveManagerStaticMeshesBase.h (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS).
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
class CEngineScene; template<class T> class CArray;
class CEnginePrimitiveManager2DStaticMeshes : public CEnginePrimitiveManagerStaticMeshesBase {
public:
    CEnginePrimitiveManager2DStaticMeshes(CArray<CEngineScene*>& scenes);
    virtual ~CEnginePrimitiveManager2DStaticMeshes();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);                 // slot 1  0x00B9E8C0
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;                                      // slot 2  0x00BA7770
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub_primitive, ERenderPassType pass);              // slot 5  0x00BA77B0
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);   // slot 10 0x00B9E950
};
#endif
