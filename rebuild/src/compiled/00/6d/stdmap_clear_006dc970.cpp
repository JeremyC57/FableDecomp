// std::map<long, CComboMultiplierData>::clear() -- STLport _map.h `_M_t.clear()` with the tree clear expanded: when the tree holds nodes,
// erase them all (the out-of-line _M_erase 0x004E6E0C from the root) and reset the header to an empty tree
// Leaf: ?clear@?$map@JVCComboMultiplierData@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC970, 51 bytes, a COMDAT in a speed-optimised TU (the CHeroExperienceDef serialiser TU beside
//   0x006DC350 / 0x006DCFA0); no E8 / E9 / absolute reference to it in Fable.exe.
// Identity (NF83, round 7, q11013 binder): the body is STLport's tree clear over the map<long, CComboMultiplierData> tree (its callee
//   0x004E6E0C is that tree's _M_erase, NF81). It cannot carry the tree clear's own name: 0x004E7720 holds
//   ?clear@?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@... (NF78, RM) and retail has no identical-code folding.
//   STLport's map::clear (`_M_t.clear()`, inline) with the tree clear expanded is the remaining library function with this text.
//   DESCRIPTIVE-by-library: no PDB, string or caller names this address (both PDBs are Dinkumware builds); the map spelling follows
//   STLport 4.6 _map.h (4-argument map). Was the stand-in _global::RbTree_Clear_006dc970 (leaf ?CTCCarryable_OnKill@@YIXPAVCTCCarryable@@@Z,
//   also defined by 0x00869900) over a local node model calling ?Detach_004e6e0c@CTCCarryable@@QAEXI@Z.
// Port (NF83): one explicit instantiation of map::clear on stlport/fable_map.h + engine/CComboMultiplierData.h with TREE_MERASE_NOINLINE
//   (the 0x00869900 form).
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"

template void std::map<long, CComboMultiplierData, std::less<long>, std::allocator<std::pair<const long, CComboMultiplierData> > >::clear();
