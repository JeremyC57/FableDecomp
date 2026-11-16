// std::map<NUISystem::CComponent*, std::pair<long,long> >::operator[](NUISystem::CComponent* const& k) -- STLport subscript (size TU):
// lower_bound, insert (k, (0, 0)) when absent, return the mapped pair
// Leaf: ??A?$map@PAVCComponent@NUISystem@@U?$pair@JJ@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@4@@std@@QAEAAU?$pair@JJ@1@ABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B322B, 74 bytes, `ret 4`, frame-pointer size TU.
// Identity: Ego_r.pdb CTCInventoryTrade::ItemsToBuy / ItemsToSell are map<NUISystem::CComponent*, pair<long, long> >; callers
//   CTCInventoryTrade::SellItemPC 0x005AECC0 / 0x005B01FE, BuyItem 0x005AF64E, BuyItemPC 0x005B0AAC bind this full STLport spelling.
// Callees: the tree's _M_lower_bound @ 0x005B1D38, map::insert(iterator, const value_type&) @ 0x005B31CF.
// NF42 (round 5): the local three-parameter map / one-parameter iterator model (which emitted a truncated mangling) replaced by the
// shared stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[] body, FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE _M_lower_bound);
// descriptive-by-library (STLport 4.6.2; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template pair<long, long>& map<NUISystem::CComponent*, pair<long, long> >::operator[](NUISystem::CComponent* const&);
}
