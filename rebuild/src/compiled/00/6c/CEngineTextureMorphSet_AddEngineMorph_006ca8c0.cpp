// CEngineTextureMorphSet::AddEngineMorph(long original_texture) -- append an engine texture morph for the original texture (morph 0, no blends) and return it
// Leaf: ?AddEngineMorph@CEngineTextureMorphSet@@IAEPAVCEngineTextureMorph@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CA8C0, 138 bytes, thiscall `ret 4`; Ego_r.pdb / FableWin.pdb
//   ?AddEngineMorph@CEngineTextureMorphSet@@IAEPAVCEngineTextureMorph@@J@Z (protected; tc_appearance_base.obj, Ego_r #4 / FableWin #3;
//   Ego_d record: this, original_texture). Its one caller is the landed AddTextureMorph 0x006CA950. The older descriptive
//   CTextureMorphs::AddMorph(unsigned long) was a local-model name (NAME_FIXES.tsv).
// Callees: vector<CCompositeBlendDesc>'s copy constructor 0x005BB346 (the in-place arm's record copy), vector<CEngineTextureMorph>::
//   _M_insert_overflow 0x006CB390, free 0x00BFEA14 (the temporary's Blends after the overflow arm).
// Shape: Lionhead's CArray push_back (FableWin tc_appearance_base.obj #21 CArray<CEngineTextureMorph>::push_back) of a
//   CEngineTextureMorph(original_texture, 0) temporary (the PDB (long, long) constructor, FableWin #4), then &back().
// Same-TU callee visibility (GOTCHAS "Same-TU callee visibility"): retail's TU compiled the overflow path's helpers itself and kept them
//   out of line -- __uninitialized_copy<CEngineTextureMorph*> 0x006CB0D0 and __uninitialized_fill_n 0x006CB170, both __fastcall (called
//   from 0x006CB390 at 0x006CB3EA / 0x006CB445 and 0x006CB429). FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N give that shape; with them
//   VC7.1 keeps the temporary's MorphID in edi for the in-place copy and drops its destructor on that arm only (reloading Blends after
//   the overflow call, 0x006CA92B), as retail does. Without UNINIT_COPY the destructor is dropped on both arms (116 bytes); with the
//   overflow declaration-only the temporary is reloaded and both tails merge (128). The overflow, uninit-copy, fill_n and _M_clear
//   bodies of this source grade RELOCATION_MATCH at their own addresses (0x006CB390, 0x006CB0D0, 0x006CB170, 0x006CB340).
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDENGINEMORPH
#include "engine/CEngineTextureMorph.h"

CEngineTextureMorph* CEngineTextureMorphSet::AddEngineMorph(long original_texture)
{
    Morphs.push_back(CEngineTextureMorph(original_texture, 0));
    return &Morphs.back();
}