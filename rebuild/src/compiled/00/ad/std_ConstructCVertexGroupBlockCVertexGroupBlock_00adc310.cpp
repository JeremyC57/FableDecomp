// std::_Construct<CVertexGroupBlock, CVertexGroupBlock>(p, value) -- STLport 4.6 placement copy-construction of a vertex-group block (group array + group -> index map)
// Leaf: ??$_Construct@VCVertexGroupBlock@@V1@@std@@YIXPAVCVertexGroupBlock@@ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADC310, 31 bytes (`__fastcall`: p in ecx, the value in edx; `ret`).
// Identity: STLport 4.6 _construct.h _Construct(p, v) = `new (p) T(v)` over CVertexGroupBlock: the placement-new null test, then the
//   implicit copy constructor: CArray<unsigned long> Groups (vector<unsigned long> copy 0x00535470) on +0 and map<unsigned long, long>
//   GroupToIndexMap (map copy 0x00A96E40) on +0xC: Ego_r.pdb CVertexGroupBlock {Groups CArray<unsigned long>, GroupToIndexMap
//   map<unsigned long,long>} (engine/CVertexGroupBlock.h, retail 0x18 bytes). Both PDBs carry the Dinkumware construct helpers for
//   CVertexGroupBlock (`_Cons_val<allocator<CVertexGroupBlock>, CVertexGroupBlock, const CVertexGroupBlock&>`), and Ego_r holds no
//   pair whose first member is a 12-byte vector<unsigned long>, so the catalog's pair<CCharString const, CArray<unsigned long> > label
//   (propagated from the byte-identical shape 0x009D3CA0) is wrong. Its TU-mates are the CVertexGroupBlock STLport helpers
//   (__destroy_aux 0x00ADC190, __uninitialized_copy 0x00ADC330, both __fastcall). Descriptive-by-library. No retail code calls or
//   references it (no E8/E9 site, no absolute reference). The 0x007AE0C0 recipe.
// NF258 (2026-11-22, L37): moved off the July row-local Pair / CArrayUL / CCharString models (`?Cons_val@@YIXPAUPair@@0@Z`, binding the
//   stand-ins `?ctor@CCharString@@QAEXPAV1@@Z` (0x00535470) and `?ctor@CArrayUL@@QAEXPAU1@@Z` (0x00A96E40)) onto engine/CVertexGroupBlock.h
//   (the 0x00AD9770 member opt-in), engine/CArray.h, stlport/fable_map.h and stlport/fable_containers.h (FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL);
//   explicit instantiation. The vector copy constructor is declaration-only and the map copy constructor out of line (as 0x00AD9770), so
//   the copies bind ??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z and ??0?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAE@ABV01@@Z.
#define FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY   // NF178b opt-in: ~_Rb_tree declared only, so the implicit member destructors the
                                                // CVertexGroupBlock copy pulls in emit no unreferenced _M_erase / clear pick-any copies
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"
namespace std {
template void __fastcall _Construct<CVertexGroupBlock, CVertexGroupBlock>(CVertexGroupBlock*, const CVertexGroupBlock&);
}
