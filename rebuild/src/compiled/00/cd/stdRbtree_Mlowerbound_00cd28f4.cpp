// std::_Rb_tree<int, std::pair<const int, CCharString>, ...>::_M_lower_bound(const int& k) const -- STLport lower bound: walk to the first node whose key is not less than k; return it, or the header when none
// Leaf: ?_M_lower_bound@?$_Rb_tree@HU?$pair@$$CBHVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBHVCCharString@@@std@@@2@U?$less@H@2@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@2@@std@@ABEPAU_Rb_tree_node_base@2@ABH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD28F4, 37 bytes, thiscall, `ret 4`; size-optimised TU (unaligned address).
// Identity: called by map<int, CCharString>::operator[] 0x00CD3A16 (agentNF69 q10857, ported on fable_map.h with
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE, RM PASS) and the tree helper 0x00CD2DDC; signed key (`jl`). Key int, not long:
//   docs/journal/2026-11/EGOD_FINDINGS.md section 4b (Ego_r.pdb + FableWin.pdb key the cutscene macro map by int). Returns _Base_ptr
//   (_Rb_tree_node_base*) as STLport 4.6 _tree.h declares it. Descriptive-by-library (STLport spelling; both PDBs are Dinkumware).
// NF72 (round 7, q10904): was the long tree returning _Rb_tree_node<V>* on a local STLport model; re-authored as the explicit instantiation on
//   the shared stlport/fable_map.h (TREE_BOUNDS_OUTOFLINE + TREE_BOUNDS_BODY_PASS22, agentNF42's 0x0042AD6C form) with fable_string.h CCharString.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree_node_base* _Rb_tree<int, pair<const int, CCharString>, _Select1st<pair<const int, CCharString> >, less<int>,
    allocator<pair<const int, CCharString> > >::_M_lower_bound(const int&) const;
}
