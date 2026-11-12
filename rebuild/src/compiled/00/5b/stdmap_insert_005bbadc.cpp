// std::map<EClothingSuitPart, long, std::less<EClothingSuitPart>, std::allocator<std::pair<EClothingSuitPart const, long> > >::insert(std::_Rb_tree_iterator<std::pair<EClothingSuitPart const, long> >, std::pair<EClothingSuitPart const, long> const&) -- STLport 4.6 insert (stlport/fable_map.h)
// Leaf: ?insert@?$map@W4EClothingSuitPart@@JU?$less@W4EClothingSuitPart@@@std@@V?$allocator@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EClothingSuitPart@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@2@@2@U32@ABU?$pair@$$CBW4EClothingSuitPart@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBADC, oracle length 29 bytes.
// Identity: (1) the landed caller 0x005BBBF3 (std::map<EClothingSuitPart, long, std::less<EClothingSuitPart>, std::allocator<std::pair<EClothingSuitPart const, long>>>::operator[]) binds this map's insert (its iterator spelled without the traits argument: a binder spelling to fix, see NOTES) (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<W4EClothingSuitPart@@J> instantiation (_Tmap_traits@W4EClothingSuitPart@@JU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/EClothingSuitPart.h"
typedef std::map<EClothingSuitPart, long> _Map_005bbadc;
template _Map_005bbadc::iterator _Map_005bbadc::insert(_Map_005bbadc::iterator, const _Map_005bbadc::value_type&);
