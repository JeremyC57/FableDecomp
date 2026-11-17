#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DSTATICMESHES_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DSTATICMESHES_H
// New header (NF74, round 7, row 0x00B9E950 Render2DPrimitive): CEnginePrimitiveManager2DStaticMeshes on the shared
// CEnginePrimitiveManagerMeshBase (Ego_r.pdb sizeof 792, no members of its own in ghidra_out/struct_layouts_egor.tsv). Members in the
// Ego_r.pdb spellings: ctor ??0..@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, the virtuals CreateInternalPrimitive / GetSupportedTypes /
// Render2DPrimitive / RenderSubPrimitive. Declarations only; no layout of its own.
#include "engine/CEnginePrimitiveManagerMeshBase.h"
class CEngineScene; template<class T> class CArray;
class CEnginePrimitiveManager2DStaticMeshes : public CEnginePrimitiveManagerMeshBase {
public:
    CEnginePrimitiveManager2DStaticMeshes(CArray<CEngineScene*>& scenes);
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);   // 0x00B9E950
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub_primitive, ERenderPassType pass);
};
#endif
