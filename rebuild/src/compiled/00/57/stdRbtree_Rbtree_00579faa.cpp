// std::_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, SExpSpendUndo>, _Select1st<..>, less<EHeroTrainableStatType>,
//   allocator<..> >::~_Rb_tree() -- clear every node (out-of-line _Rb_tree::clear 0x00579EA3), then free the header node
//   (_Rb_tree_base::~_Rb_tree_base: `_M_header.deallocate(_M_header._M_data, 1)`, the null-tested free): the destructor of
//   CTCInventoryExperience::UndoableStatBuys (map<EHeroTrainableStatType, SExpSpendUndo>, +0x140).
// Leaf: ??1?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579FAA, 23 bytes: `push esi ; mov esi,ecx ; call 0x00579EA3 ; mov esi,[esi] ; test esi,esi ;
//   je ; push esi ; call free ; pop ecx` (size-optimised TU, as the 0x005BB04F set<EClothingSuitPart> tree destructor).
// Name fix (NF195, 2026-11-20, q4774): was the descriptive stand-in std::list_T_00579faa::~list_T_00579faa (Leaf ??1list_T_00579faa@std@@),
//   whose clear() bound 0x00579EA3 as ?clear@list_T_00579faa@std@@QAEXXZ. The body is the STLport map tree destructor: it calls
//   0x00579EA3 (landed at nf189 as _Rb_tree<EHeroTrainableStatType,pair<const EHeroTrainableStatType,SExpSpendUndo>,..>::clear) and
//   frees the header; the only landed referencer, ~CTCInventoryExperience 0x0057846F, already binds this ~_Rb_tree spelling at
//   0x00579FAA. Now the 0x005BB04F recipe on the shared headers (STLport spelling: descriptive-by-library, no PDB holds STL instances).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "engine/EHeroTrainableStatType.h"
#include "engine/CTCInventoryExperience.h"

namespace std {
template _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, SExpSpendUndo>, _Select1st<pair<EHeroTrainableStatType const, SExpSpendUndo> >, less<EHeroTrainableStatType>, allocator<pair<EHeroTrainableStatType const, SExpSpendUndo> > >::~_Rb_tree();
} // namespace std
