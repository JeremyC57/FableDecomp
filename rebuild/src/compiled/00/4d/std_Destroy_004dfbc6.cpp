// std::_Destroy(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last) -- STLport range destroy: dispatch on the element type's destructor trait to the out-of-line __destroy_aux loop
// Leaf: ??$_Destroy@PAVCTargetPoint@NPlayerTargeting@@@std@@YIXPAVCTargetPoint@NPlayerTargeting@@0@Z
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DFBC6, 13 bytes (size-optimised TU, unaligned start), __fastcall (first ecx, last edx passed
//   straight on), plain `ret`: push ecx ; lea eax,[esp+3] ; push eax ; call 0x004DBE58 ; pop ecx ; ret.
// Identity (agentNF57 name fix; was the descriptive free function CComponent_GetCurrentState over NUISystem_GetState, a propagated
//   NUISystem::CComponent::GetCurrentState label -- that member is 0x00533B30): STLport 4.6.2 `_Destroy(first, last)` ->
//   `__destroy(first, last, value_type)` -> `__destroy_aux(first, last, __false_type())`. Sources: (1) its only retail caller is the
//   landed vector<NPlayerTargeting::CTargetPoint>::operator= 0x008085C0 (call 0x00808693); (2) its callee 0x004DBE58 destroys a range
//   of 0x1C-byte elements whose first member is an intelligent pointer (catalog ForEachElem0x1c__CBaseIntelligentPointer), which is
//   NPlayerTargeting::CTargetPoint (Ego_r.pdb 28 bytes: PThing CIntelligentPointer<CThing> +0, Index +8, TargetingPosition +0xC,
//   FrameRejected +0x18; engine/NPlayerTargeting.h). Descriptive-by-library: no PDB carries STLport (both are Dinkumware builds).
// Shape: as the twin 0x005BC0C6 (std::_Destroy<CEngineTextureMorph*>): the header's STLport range _Destroy explicitly instantiated,
//   __fastcall (FABLE_RETAIL_STLP_PASS22_FLARE_ABI), __destroy_aux the out-of-line declaration only (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL).
//   The element type is only named (the range wrapper never touches an element), so the class is forward-declared.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
namespace NPlayerTargeting { class CTargetPoint; }    // engine/NPlayerTargeting.h

template void FABLE_FASTCALL std::_Destroy<NPlayerTargeting::CTargetPoint*>(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last);
