// CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the mesh morph set (original texture morphs, mesh parts) and free it
// Leaf: ?DeleteData@?$CCountedPointer@VCMeshMorphSet@CTCAppearanceMorphBase@@@@SIXPAX@Z
// Retail: Fable.exe @ 0x007222B0, 58 bytes, static __fastcall (data in ecx), `ret`.
// Identity (probable): Ego_r.pdb names `?DeleteData@?$CCountedPointer@VCMeshMorphSet@CTCAppearanceMorphBase@@@@SIXPAX@Z`
//   (with its Reset); the body is the `delete (T*)data` deleter shape (landed CCountedPointer<CCardinalSpline>::DeleteData
//   0x0075F830) stored as the count block's deleter at 0x00722340 (the Reset beside it), and its element destructor is
//   the out-of-line __destroy_aux over CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts 0x00722130. Layout (Ego_r 0x20 with
//   16-byte vectors; retail 0x18): CArray<CMeshMorphSetParts> MeshParts +0, CEngineTextureMorphSet OriginalMeshTextureMorphs +0xC.
// Callees: ~vector<CEngineTextureMorph> 0x005BC0D3 (FableWin.pdb; OriginalMeshTextureMorphs.Morphs), __destroy_aux 0x00722130, free 0x00BFEA14,
//   operator delete 0x00BFE9BC.
// NF62 (2026-11-17, round 7): ported onto the shared stlport/fable_containers.h vector + engine/CArray.h and the CTCAppearanceMorphBase /
//   CEngineTextureMorph headers (overlay opt-ins FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET, FABLE_RETAIL_CENGINETEXTUREMORPHSET);
//   the local STLport vector / allocator / CArray / CEngineTextureMorphSet / CTCAppearanceMorphBase models are gone and 0x005BC0D3 binds
//   ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ (was the stand-in ??1CEngineTextureMorphSet@@QAE@XZ).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CArray.h"
#include "engine/CEngineTextureMorph.h"
#include "engine/CTCAppearanceMorphBase.h"
#include "engine/CCountedPointer.h"

template class CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>;
