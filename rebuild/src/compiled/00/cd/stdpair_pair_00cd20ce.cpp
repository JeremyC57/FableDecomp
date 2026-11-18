// std::pair::pair(const first_type& a, const second_type& b) -- (pair<const int, CCharString >) STLport member-wise constructor: the 4-byte first copied inline, the second through its out-of-line copy constructor; returns this
// Leaf: ??0?$pair@$$CBHVCCharString@@@std@@QAE@ABHABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD20CE, 29 bytes, thiscall `ret 8`.
// Identity (probable): std::pair<const long, CCharString>::pair(const first_type&, const second_type&); evidence: probable (the key's spelling): its only caller is the landed map<long, CCharString>::operator[] 0x00CD3A16 (0x00CD3A47), used twice (0x00CC0032, 0x00CC00AA) on a local map of the cutscene-macro runner 0x00CBFB7D keyed by GFStringToInt's result (FableWin.pdb ?GFStringToInt@@YIJPBD@Z returns long) with a signed (`jl`) lower_bound 0x00CD28F4; the value is a CCharString (copy constructor 0x0099EC30).
// Label: the manifest's "_Pair_base<NUISystem::CComponent*_const,std::pair<CWideString,long>_>" is propagated (family H046: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callee: 0x0099EC30 (the second member's copy constructor, on `lea ecx,[esi+4]`).
// Shape: STLport 4.6 _pair.h `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`: the first word is loaded
//   through its reference before the second reference is pushed (as the landed pair<long, CCharString> 0x00695250).
// NF72 (round 7, q10903): key type int, not long -- the caller operator[] 0x00CD3A16 is map<int, CCharString> (agentNF69, q10857:
//   docs/journal/2026-11/EGOD_FINDINGS.md section 4b; Ego_r.pdb + FableWin.pdb key the cutscene macro map by int). Re-authored as the explicit
//   instantiation on the shared stlport/fable_containers.h pair and fable_string.h CCharString (FABLE_RETAIL_CCHARSTRING; copy ctor 0x0099EC30)
//   instead of the local pair / CCharString models.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
template std::pair<const int, CCharString>::pair(const int&, const CCharString&);
