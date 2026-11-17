// std::_Rb_tree<EHeroAbility, std::pair<EHeroAbility const, SExpSpendUndo>, std::_Select1st<std::pair<EHeroAbility const, SExpSpendUndo> >, std::less<EHeroAbility>, std::allocator<std::pair<EHeroAbility const, SExpSpendUndo> > >::_M_find<EHeroAbility>(const EHeroAbility& key) const -- STLport 4.6.1 _tree.h member-template lookup: walk from the root keeping the last node
//   whose key is not less than `key`, then return the header unless that node's key equals it
// Leaf: ??$_M_find@W4EHeroAbility@@@?$_Rb_tree@W4EHeroAbility@@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroAbility@@@3@V?$allocator@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@3@@std@@ABEPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@1@ABW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057914C, size TU.
// NF64 (round 7, q10682): an explicit instantiation of the stlport/fable_map.h member-template _M_find (const, ABE) for the
//   map<EHeroAbility, SExpSpendUndo> tree of CTCInventoryExperience (Ego_r.pdb SExpSpendUndo through engine/CTCInventoryExperience.h
//   FABLE_RETAIL_SEXPSPENDUNDO_ONLY). The landed callers 0x005729D8 / 0x00576C4F and NF61's 0x005786B5 / 0x005785B3 bind this spelling.
//   Was a local STLport model whose leaf was the non-template prefix ?_M_find@?$_Rb_tree@. Descriptive-by-library (retail is STLport
//   4.6; both PDBs are Dinkumware builds).
// Was: std::_Rb_tree::_M_find(const key_type& k) const -- (map<EHeroAbility, SExpSpendUndo >) STLport exact-key lookup over a signed 32-bit key: walk from the root keeping the last node whose key is not less than k, then return it unless it is the header or k is less than its key (then the header, i.e. end())
#pragma optimize("s", on)
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "stlport/fable_map.h"
#include "engine/EHeroAbility.h"
#include "engine/CTCInventoryExperience.h"

template std::_Rb_tree_node<std::pair<EHeroAbility const, SExpSpendUndo> >* std::_Rb_tree<EHeroAbility, std::pair<EHeroAbility const, SExpSpendUndo>, std::_Select1st<std::pair<EHeroAbility const, SExpSpendUndo> >, std::less<EHeroAbility>, std::allocator<std::pair<EHeroAbility const, SExpSpendUndo> > >::_M_find<EHeroAbility>(const EHeroAbility&) const;
