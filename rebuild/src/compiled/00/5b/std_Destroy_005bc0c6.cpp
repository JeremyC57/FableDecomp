// std::_Destroy(CEngineTextureMorph* first, CEngineTextureMorph* last) -- STLport range destroy: dispatch on the element type's destructor trait to the out-of-line __destroy_aux loop
// Leaf: ??$_Destroy@PAVCEngineTextureMorph@@@std@@YIXPAVCEngineTextureMorph@@0@Z
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BC0C6, 13 bytes (size-optimised TU, unaligned start), __fastcall (first ecx, last edx passed
//   straight on), plain `ret`: push ecx ; lea eax,[esp+3] ; push eax ; call 0x005BBF80 ; pop ecx ; ret.
// Identity (agentNF57 name fix; was the descriptive free function CComponent_GetCurrentState over NUISystem_GetState_005bbf80, a
//   propagated NUISystem::CComponent::GetCurrentState label -- that member is 0x00533B30): STLport 4.6.2 `_Destroy(first, last)` ->
//   `__destroy(first, last, value_type)` -> `__destroy_aux(first, last, __false_type())`. Sources: (1) its only retail caller is the
//   landed vector<CEngineTextureMorph>::operator= 0x00728830 (call 0x007288FE), which binds ??$_Destroy@PAVCEngineTextureMorph@@@std@@...;
//   (2) its callee 0x005BBF80 is the /Gr range destructor the landed ~vector<CEngineTextureMorph> 0x005BC0D3 binds as
//   ??$__destroy_aux@PAVCEngineTextureMorph@@@std@@YIXPAVCEngineTextureMorph@@0ABU__false_type@0@@Z, stepping 0x14 bytes (the STLport
//   CEngineTextureMorph: two longs + a 12-byte vector). Descriptive-by-library: no PDB carries STLport (both are Dinkumware builds).
// Shape: the header's STLport range _Destroy explicitly instantiated, __fastcall as in the /Gr Lionhead TUs (FABLE_RETAIL_STLP_PASS22_FLARE_ABI
//   spells _Destroy / __destroy / __destroy_aux FABLE_FASTCALL), with __destroy_aux the out-of-line declaration only
//   (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL, as 0x005BC0D3); the empty tag (user constructor) sits in the `push ecx` frame dword.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#include "engine/CEngineTextureMorph.h"

template void FABLE_FASTCALL std::_Destroy<CEngineTextureMorph*>(CEngineTextureMorph* first, CEngineTextureMorph* last);
