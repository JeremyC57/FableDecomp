#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSTATICMESHES_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSTATICMESHES_H
// Overlay header (chunk 2011): RTTI .?AVCEnginePrimitiveManagerStaticMeshes@@ : CEnginePrimitiveManagerStaticMeshesBase (+0, the rest
// of the chain CEnginePrimitiveManagerMeshBase .. CBaseClass, ghidra_out/rtti_bases.tsv). Own members not modelled (declaration of
// the class only: the rows reach the base's AddStaticMesh through CEnginePrimitiveRenderer::GetStaticMeshManager()).
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
class CEnginePrimitiveManagerStaticMeshes : public CEnginePrimitiveManagerStaticMeshesBase {
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHES_CTOR
    // Retail opt-in (wfE7, row 0x00B33B50): ??0CEnginePrimitiveManagerStaticMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (retail 0x00BA1A70,
    // `new` of 0x318 in the CEnginePrimitiveRenderer ctor; Ego_r / FableWin). Declaration only; own members opaque.
public:
    CEnginePrimitiveManagerStaticMeshes(CArray<CEngineScene*>& scenes);   // 0x00BA1A70
private:
    unsigned char Unmodeled_StaticMeshes[0x318 - sizeof(CEnginePrimitiveManagerStaticMeshesBase)];
#endif
};
#endif
