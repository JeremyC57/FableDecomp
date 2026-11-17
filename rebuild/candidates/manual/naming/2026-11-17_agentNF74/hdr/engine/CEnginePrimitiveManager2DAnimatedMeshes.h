#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DANIMATEDMESHES_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGER2DANIMATEDMESHES_H
// New header (NF74, round 7, row 0x00BD1F70 Render2DPrimitive): CEnginePrimitiveManager2DAnimatedMeshes on the shared
// CEnginePrimitiveManagerAnimatedMeshesBase (Ego_r.pdb sizeof 1680, no members of its own in ghidra_out/struct_layouts_egor.tsv). Members in the
// Ego_r.pdb spellings: ctor ??0..@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, the virtuals CreateInternalPrimitive / GetSupportedTypes /
// Render2DPrimitive / RenderSubPrimitive. Declarations only; no layout of its own.
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
class CEngineScene; template<class T> class CArray;
class CEnginePrimitiveManager2DAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase {
public:
    CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>& scenes);
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);   // 0x00BD1F70
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub_primitive, ERenderPassType pass);
};
#endif
