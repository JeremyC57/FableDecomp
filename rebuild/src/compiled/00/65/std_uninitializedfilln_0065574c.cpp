// std::uninitialized_fill_n(CIntelligentPointer<CThing>* first, unsigned int n, const CIntelligentPointer<CThing>& x) -- STLport 4.6 dispatch: forward to the non-POD __uninitialized_fill_n with a __false_type tag
// Leaf: ??$uninitialized_fill_n@PAV?$CIntelligentPointer@VCThing@@@@IV1@@std@@YIPAV?$CIntelligentPointer@VCThing@@@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065574C, 19 bytes: `push ebp ; mov ebp,esp ; lea eax,[ebp+0Bh] ; push eax ; push [ebp+8] ;
//   call 0x004AA190 ; pop ebp ; ret 4` -- __fastcall (first ecx and n edx passed straight through, x on the stack), the tag
//   temporary homed in the dead x argument slot, size-optimised TU (ebp frame).
// Identity (agentNF54 name fix; was the made-up extern "C" stdcall FableUmove_0065574c over FableUmove_construct_0065574c, catalogued
//   as the propagated "vector<EWeaponClass>::_Umove<EWeaponClass*>"): STLport 4.6 stl/_uninitialized.h
//   `uninitialized_fill_n(first, n, x) { return __uninitialized_fill_n(first, n, x, _IS_POD_ITER(first, T)); }` for
//   CIntelligentPointer<CThing>: the callee 0x004AA190 is the landed
//   ??$__uninitialized_fill_n@PAV?$CIntelligentPointer@VCThing@@@@IV1@@std@@YIPAV?$CIntelligentPointer@VCThing@@@@PAV1@IABV1@ABU__false_type@0@@Z
//   (the copy loop storing the CIntelligentPointer<CThing> vtable 0x01238C6C), whose STLport caller is exactly this forwarder with
//   the same three template arguments. Descriptive by library: neither PDB names this instance (both are Dinkumware builds).
// Shape: the header's out-of-line __uninitialized_fill_n (FABLE_RETAIL_STLP_UNINIT_OUTOFLINE: visible body, kept out of line) so
//   VC7.1 knows the empty tag is never read and homes it in a dead argument slot.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_UNINIT_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "stlport/fable_containers.h"
class CThing;
#include "engine/CIntelligentPointer.h"

template CIntelligentPointer<CThing>* std::uninitialized_fill_n(CIntelligentPointer<CThing>* first, unsigned int n,
    const CIntelligentPointer<CThing>& x);
