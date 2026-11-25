// std::_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, NUISystem::CComponent*>, _Select1st<..>, less<EHeroTrainableStatType>,
//   allocator<..> >::~_Rb_tree() -- clear every node (out-of-line _Rb_tree::clear 0x005793E8), then free the header node
//   (_Rb_tree_base::~_Rb_tree_base: `_M_header.deallocate(_M_header._M_data, 1)`, the null-tested free): the destructor of
//   CTCInventoryExperience::UpgradableStats (map<EHeroTrainableStatType, NUISystem::CComponent*>, +0x224).
// Leaf: ??1?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@PAVCComponent@NUISystem@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005796AB, 23 bytes: `push esi ; mov esi,ecx ; call 0x005793E8 ; mov esi,[esi] ; test esi,esi ;
//   je ; push esi ; call free ; pop ecx` (size-optimised TU, as the sibling 0x00579FAA).
// Identity (NF399, T3 nf396; ~_Rb_tree v ~map by reference sites, NF389): retail reaches this address twice (E8 and E9 scan of all
//   .text): (1) `call 0x005796AB` at 0x005784E8 in ~CTCInventoryExperience 0x0057846F, the UpgradableStats member (+0x224) in its
//   reverse destroy order; the table already binds this spelling here with that witness; (2) `jmp 0x005796AB` at 0x00579887, an
//   uncatalogued, unreferenced 5-byte body right after the 25-byte map constructor 0x0057986E. A map's implicit destructor is only
//   the destruction of its _M_t member, so a 5-byte tail jump is the shape of ~map (inferred: the address has no row and no
//   reference); its target is then the tree destructor. The TU repeats the pattern for its other trees (0x0057985B `jmp 0x00579616`,
//   0x0057A101 `jmp 0x00579FAA`, both after a map constructor; 0x00579FAA landed as ~_Rb_tree by NF195). The body calls clear
//   0x005793E8 (landed nf396_b2 as this tree's _Rb_tree::clear). Both PDBs are Dinkumware builds: the spelling is the one the
//   repo STLport emits (descriptive-by-library).
// NF399: was the descriptive stand-in std::list_T_005796ab::~list_T_005796ab (Leaf ??1list_T_005796ab@std@@), whose clear() was
//   ?clear@list_T_005796ab@std@@QAEXXZ (unbound). Now the 0x00579FAA recipe on the shared header (clear declared only).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EHeroTrainableStatType.h"
namespace NUISystem { class CComponent; }
namespace std {
template _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, NUISystem::CComponent*>, _Select1st<pair<EHeroTrainableStatType const, NUISystem::CComponent*> >, less<EHeroTrainableStatType>, allocator<pair<EHeroTrainableStatType const, NUISystem::CComponent*> > >::~_Rb_tree();
} // namespace std
