// std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::erase(const CCharString& key) -- STLport 4.6 set<CCharString> erase by key: take the equal_range, count it with distance, erase that range, return the count
// Leaf: ?erase@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAEIABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BB270, 92 bytes, thiscall `ret 4`.
// Identity: set<CCharString> -- Ego_r.pdb CFaction::AlliedFactions / EnemyFactions and CQuestManager::
//   QuestsWithResourcesLoadedThatAreNotActivated are set<CCharString>; its callers are CFaction::SetAsAlliedWithFaction 0x006B94E0,
//   SetAsNeutralToFaction 0x006B9510 and SetAsEnemyOfFaction 0x006B9530 (erase from those sets) and CQuestManager::ActivateQuests
//   0x004B3CE0, which already binds this exact spelling (NF80). Its callees are the same tree's _M_lower_bound 0x004B9D00 /
//   _M_upper_bound 0x004B9D50 (set<CCharString>, NF80 / NF82). The catalog's map<CCharString, long> was a stand-in.
// Callees: _M_upper_bound 0x004B9D50 and _M_lower_bound 0x004B9D00 (equal_range's pair arguments, evaluated right to left),
//   __distance(first, last, const input_iterator_tag&) 0x004B84A0 (cdecl, out of line), erase(iterator, iterator) 0x004B9C10.
// Shape (family K111): the landed _Rb_tree::erase(const key_type&) 0x0042CDE0 (family G053) in a speed TU with equal_range inline: the
//   distance call pops 8 bytes and its third slot is reused for erase's first iterator; __distance's body is visible but out of line
//   (stlport/fable_containers.h FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE), so VC7.1 keeps both iterators in registers across the call.
// NF82 (q10993 / q10994): moved off the row-local CCharString / pair / iterator / tree models onto stlport/fable_map.h
//   (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: erase(const K&) over the out-of-line private bounds, explicit instantiation) and
//   fable_string.h; its own symbol and its four callee bindings move from the map<CCharString, long> stand-in to set<CCharString>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
template size_t std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::erase(const CCharString&);
