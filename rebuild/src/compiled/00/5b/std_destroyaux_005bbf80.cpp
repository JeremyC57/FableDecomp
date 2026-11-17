// std::__destroy_aux<CEngineTextureMorph*>(first, last, const __false_type&) -- STLport range destructor: destroy each 0x14-byte texture morph in [first, last)
// Flags: /Os
// Leaf: ??$__destroy_aux@PAVCEngineTextureMorph@@@std@@YIXPAVCEngineTextureMorph@@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBF80, size-optimised TU, /Gr `__fastcall` (first in ecx, last in edx, the tag
//   reference on the stack): `push esi ; push edi ; mov edi,edx ; mov esi,ecx ; jmp ; L: push 0 ; mov ecx,esi ; call 0x005BBBCE ;
//   add esi,14h ; cmp esi,edi ; jne L ; pop edi ; pop esi ; ret 4`.
// Identity (NF56, round 7, queue q10469): FableWin.pdb 0x01E0CC40 names its only caller ~vector<CEngineTextureMorph> 0x005BC0D3 (NF54,
//   landed: binds this address as ??$__destroy_aux@PAVCEngineTextureMorph@@@std@@YIXPAVCEngineTextureMorph@@0ABU__false_type@0@@Z,
//   FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL). Was the stand-in Destroy_range_flash over a local 0x14-byte CFlash model. Now the
//   explicit instantiation of the shared stlport/fable_containers.h __fastcall __destroy_aux (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL)
//   over engine/CEngineTextureMorph.h; the per-element call is the element's destructor helper 0x005BBBCE (flag 0).
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#include "stlport/fable_containers.h"
#include "engine/CEngineTextureMorph.h"

template void __fastcall std::__destroy_aux<CEngineTextureMorph*>(CEngineTextureMorph* first, CEngineTextureMorph* last, const std::__false_type&);
