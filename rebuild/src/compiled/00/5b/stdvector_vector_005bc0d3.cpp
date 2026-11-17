// std::vector<CEngineTextureMorph>::~vector() -- destroy the elements through the out-of-line __destroy_aux, then free the block
// Flags: /Os
// Leaf: ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC0D3 (size-optimised TU, unaligned start), thiscall.
// Retail shape: push the tag address, edx = finish, ecx = start, call 0x005BBF80 (the __fastcall range destructor), then
//   `if (start) free(start)` (0x00BFEA14).
// Identity (agentNF54 name fix; was the stand-in free function CActiveFile_OnReadFinished, catalog _global::StdList_Dtor_005bc0d3):
//   FableWin.pdb 0x01E0CC40 ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ. The elements are
//   0x14 bytes (engine/CEngineTextureMorph.h: OriginalTextureIndex, MorphID, Blends vector; Ego_r.pdb 24 bytes with its 16-byte
//   Dinkumware vector) and 0x005BBF80 destroys each through the element destructor 0x005BBBCE; its callers are the texture-morph
//   owners (CEnginePrimitiveMorphedAnimatingMesh::~ 0x00709B10, the CTCAppearanceMorphBase::CMeshMorphSet destructors 0x00722270 /
//   0x007222B0, NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::OnAppearanceDraw 0x0077B350), whose Ego_r.pdb members
//   TextureMorphs / PTextureMorphs are vector<CEngineTextureMorph>.
// Shape: the header's STLport ~vector, explicitly instantiated, compiled /Os (the flag of the neighbouring same-TU rows: retail
//   pops the free argument with `pop ecx`). The range destructor is the /Gr out-of-line COMDAT (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL,
//   declaration only); the _Destroy -> __destroy dispatch above it is expanded (NF54 overlay opt-in
//   FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE: /Os alone calls _Destroy out of line).
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#include "engine/CEngineTextureMorph.h"

template std::vector<CEngineTextureMorph>::~vector();
