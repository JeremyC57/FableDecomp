// CEngineTextureMorph::`scalar deleting destructor'(unsigned int) -- free the Blends vector's storage at +8, then the morph itself when asked
// Leaf: ??_GCEngineTextureMorph@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBBCE, length 37 bytes (a /Os module: no alignment padding, `pop ecx`
//   argument cleanup).
// Callees: free @ 0x00BFEA14 (the Blends buffer: STLport ~vector<CCompositeBlendDesc>, trivial elements), operator delete @ 0x00BFE9BC.
// Callers: std::__destroy_aux<CEngineTextureMorph*> 0x005BBF80 (landed, NF56: `push 0 ; mov ecx,esi ; call` per 0x14-byte element)
//   and a one-element destroy at 0x005BBDE9; the owners are the vector<CEngineTextureMorph> of ~vector 0x005BC0D3 (landed).
// Name fix (NF59, queue q10536; NF56 porting 0x005BBF80): the landed row modelled a descriptive CKeyedArrayRecord { key, value,
//   CArray<long> }; NF56 proved the element is CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, Blends +8,
//   sizeof 0x14; engine/CEngineTextureMorph.h), and VC7.1 emits STLport's per-element `p->~T()` as a call of the compiler-generated
//   non-virtual scalar deleting destructor with flag 0. The record model is replaced by the shared header.
// Shape: no vtable store (non-polymorphic); VC7.1 emits the `??_G` in a TU that `delete`s the class (the helper below, which is
//   not a retail function), with the member vector's inline destructor (`if (first) free(first)`) expanded into it.
#pragma optimize("s", on)
#include "engine/CEngineTextureMorph.h"

void DeleteEngineTextureMorph(CEngineTextureMorph* morph)
{
    delete morph;
}
