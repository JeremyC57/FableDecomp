// std::map<CComponent*, pair<CWideString, long> >::operator[](const key_type& k) -- STLport subscript on the trade screen's per-component count labels: lower_bound, insert (k, pair<CWideString, long>()) when absent, return the (text, count) pair
// Leaf: ??A?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@QAEAAU?$pair@VCWideString@@J@1@ABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3100, 103 bytes, `ret 4`.
// Map: (line 1 short-spells the NUISystem::CComponent* key and std::pair value) CTCInventoryTrade NumBuyableThings / NumSellableThings / NumWantedThings (+0x1B0 / +0x1BC / +0x1C8, Ego_r.pdb
// `map<NUISystem::CComponent *, std::pair<CWideString, long> >`; the landed ctor 0x0059EB0C builds them with 0x005B2E6C).
// Callers: 38 sites in the CTCInventoryTrade TU (0x005A0863 .. 0x005AFDF5), e.g. 0x005A0863 on NumSellableThings
// (`[ebx+1BCh]`), which writes the returned pair's count (`mov [eax+4],ecx`).
// Callees: _M_lower_bound(k) @ 0x005B1DFE (node pointer; the catalog "RBTree_FindNode_Uint3"), CWideString() @ 0x0099AED0
// and ~CWideString @ 0x0099B510, pair<NUISystem::CComponent* const, pair<CWideString, long> >::pair(const K&, const T&)
// @ 0x005B17C6 (out of line), map::insert(iterator, const value_type&) @ 0x005B2EC4 (the hinted tree insert).
// Label: the catalog "StdMap_LowerBound" is descriptive: the body is STLport 4.x `map::operator[]` verbatim, the
// 0x005812DF template with the key a pointer: less<CComponent*> inline (`mov eax,[ebx] ; cmp eax,[esi+10h] ; jae`), the
// `_Tp()` default (pair's inline default ctor: CWideString() then `and [ebp-4],0`) at the top of the frame, the
// value_type temporary (12 bytes) below it, the iterator argument rebuilt in its slot and the insert result in the dead
// `k` slot; the value is node + 0x14.
// Shape: frame-pointer size TU: `optimize("s", on)`; `_Tp()` as a named local of the insert block
// (the 0x005812DF form).
// NF42 (round 5): the local CWideString / three-parameter map / one-parameter iterator model (a truncated mangling) replaced by the
// shared fable_string.h and stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[], FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE), so the
// row and its callees carry the full STLport spelling the 38 callers bind; descriptive-by-library (STLport 4.6.2).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template pair<CWideString, long>& map<NUISystem::CComponent*, pair<CWideString, long> >::operator[](NUISystem::CComponent* const&);
}
