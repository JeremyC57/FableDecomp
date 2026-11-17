// std::__destroy_aux<NPlayerTargeting::CTargetPoint*>(first, last, const __false_type&) -- STLport non-trivial range destroy: run each 28-byte CTargetPoint's destructor (its intelligent pointer's ~CBaseIntelligentPointer) in order
// Leaf: ??$__destroy_aux@PAVCTargetPoint@NPlayerTargeting@@@std@@YIXPAVCTargetPoint@NPlayerTargeting@@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DBE58, 27 bytes, __fastcall (first ecx, last edx, the tag pointer pushed: `ret 4`), stride 0x1C,
//   size-optimised TU.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (`mov ecx,esi ; call`: the CTargetPoint destructor expanded
//   to its PThing member's base destructor, a direct call).
// Callers: std::_Destroy<NPlayerTargeting::CTargetPoint*> 0x004DFBC6 (landed, NF57: binds this spelling) and
//   vector<NPlayerTargeting::CTargetPoint>::~vector 0x004E1AAF.
// Name fix (NF59, queue q10543; NF57 q10407): the landed row modelled a Dinkumware _Destroy_range over a made-up
//   pair<unsigned long, CAnimationEntry> with a made-up DestroyElement thunk; the element is NPlayerTargeting::CTargetPoint (Ego_r.pdb
//   28 bytes: PThing CIntelligentPointer<CThing> +0, Index +8, TargetingPosition +0xC, FrameRejected +0x18; engine/NPlayerTargeting.h),
//   the 0x68 / 0x74 vectors of CTCTargetingPlayer. Ported onto stlport/fable_containers.h (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL: the
//   /Gr __fastcall body), engine/NPlayerTargeting.h and the retail engine/CIntelligentPointer.h. Descriptive-by-library: no PDB carries
//   the STLport instance (both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#include "stlport/fable_containers.h"
#include "engine/NPlayerTargeting.h"

template void __fastcall std::__destroy_aux<NPlayerTargeting::CTargetPoint*>(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, const std::__false_type&);
