// std::vector<CEngineTextureMorph>::_M_insert_overflow(CEngineTextureMorph* position, const CEngineTextureMorph& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow-and-insert for the 20-byte texture morph records
// Leaf: ?_M_insert_overflow@?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@IAEXPAVCEngineTextureMorph@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB390, length 218 bytes (+ int3 padding), thiscall `ret 14h`. Callers:
// CEngineTextureMorphSet::AddEngineMorph 0x006CA8C0 (push_back) and 0x006CB470 (catalog
// "vector<pair<int,CDefPtr>>::emplace_back", another push_back of the same vector). The manifest label
// "Array_Insert_range" is descriptive.
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) @ 0x006CB0D0
// (`__fastcall`, x2), vector<CCompositeBlendDesc>'s copy constructor @ 0x005BB346 (out of line), __uninitialized_fill_n(
// first, n, x, const __false_type&) @ 0x006CB170 (`__fastcall`), _M_clear() @ 0x006CB340 (landed
// std::vector<CEngineTextureMorph>::_M_clear: frees each record's Blends buffer, then the old buffer).
// Layout: CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc> Blends +8;
// 20 bytes in retail, the STLport vector being 12); STLport vector {_M_start, _M_finish, _M_end_of_storage}.
// Shape: STLport 4.6 _vector.h _M_insert_overflow (stlport/fable_containers.h, protected member) with the TU's out-of-line __fastcall
//   __uninitialized_copy / __uninitialized_fill_n (FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N), as emitted by the retail TU-mate
//   AddEngineMorph 0x006CA8C0 (Account B, hand4f_b5: that source grades RELOCATION_MATCH at this address); explicit instantiation here.
// Name fix (NF96 part 2, q11454): the old row defined ?_M_insert_overflow@?$vector@VCEngineTextureMorph@@U?$allocator@...@@QAEX... (struct
//   allocator key, public) on local CRGBColour / CCompositeBlendDesc / std::vector / CEngineTextureMorph models and bound _M_clear 0x006CB340
//   (U allocator, QAE) and the two helpers by non-template spellings; now on engine/CEngineTextureMorph.h + stlport/fable_containers.h
//   (STLport 4.6: _M_insert_overflow / _M_clear protected, class allocator, the helpers function templates; the landed 0x006CA8C0 binds
//   this spelling).
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#include "engine/CEngineTextureMorph.h"

template void std::vector<CEngineTextureMorph>::_M_insert_overflow(CEngineTextureMorph*, const CEngineTextureMorph&, const std::__false_type&,
    unsigned int, bool);
