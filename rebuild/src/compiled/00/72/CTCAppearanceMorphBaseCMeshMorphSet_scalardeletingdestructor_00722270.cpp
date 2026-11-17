// CTCAppearanceMorphBase::CMeshMorphSet::`scalar deleting destructor'(unsigned int flags) -- destroys the mesh morph set (original texture morphs, then the mesh-part array) and frees it when bit 0 of flags is set
// Leaf: ??_GCMeshMorphSet@CTCAppearanceMorphBase@@QAEPAXI@Z
// Retail: Fable.exe @ 0x00722270, 63 bytes, thiscall `ret 4`; no vtable store (a non-polymorphic class).
// Identity (probable): the body is member-for-member the CMeshMorphSet destruction the landed
//   CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>::DeleteData 0x007222B0 (next row) inlines: ~vector<CEngineTextureMorph>
//   0x005BC0D3 (FableWin.pdb; the Morphs array of OriginalMeshTextureMorphs +0x0c), then the MeshParts vector (+0x00) through the
//   out-of-line __destroy_aux 0x00722130 over CMeshMorphSetParts and free 0x00BFEA14, then operator delete 0x00BFE9BC. Ego_r.pdb has
//   ??1CMeshMorphSet@CTCAppearanceMorphBase@@QAE@XZ (a user destructor); VC7.1 emits the non-virtual ??_G on a `delete` (GOTCHAS
//   wave 24m). No caller or reference survives in retail, so the class rests on the layout and the callees alone.
// Shape: the __false_type tag temporary of _Destroy is homed in the dead flags argument slot (`lea eax,[esp+8]`); that
//   needs the same-TU noinline __destroy_aux body (FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL).
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

// A `delete` of the class makes VC7.1 emit its scalar deleting destructor (the graded leaf).
void DeleteMeshMorphSet_00722270(CTCAppearanceMorphBase::CMeshMorphSet* set) { delete set; }
