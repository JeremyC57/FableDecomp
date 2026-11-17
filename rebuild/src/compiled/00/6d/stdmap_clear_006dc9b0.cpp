// std::map<EDamageAttribute, CHeroStatIncreaseData>::clear() -- STLport _map.h `_M_t.clear()` with the tree clear expanded: when the tree
// holds nodes, erase them all (the out-of-line _M_erase 0x004E6E43 from the root) and reset the header to an empty tree
// Leaf: ?clear@?$map@W4EDamageAttribute@@VCHeroStatIncreaseData@@U?$less@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@4@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC9B0, 51 bytes, a COMDAT in a speed-optimised TU (the CHeroExperienceDef serialiser TU beside
//   0x006DC450 / 0x006DD1C0); no E8 / E9 / absolute reference to it in Fable.exe.
// Identity (NF90, q11179 binder): the body is STLport's tree clear over the map<EDamageAttribute, CHeroStatIncreaseData> tree (its callee
//   0x004E6E43 is that tree's _M_erase, NF90). It cannot carry the tree clear's own name: 0x004E7749 holds that spelling (NF90; retail has
//   no identical-code folding). STLport's map::clear (`_M_t.clear()`, inline) with the tree clear expanded is the remaining library
//   function with this text: the byte twin of 0x006DC970 (NF83, map<long, CComboMultiplierData>::clear), which sits beside it.
//   DESCRIPTIVE-by-library: no PDB, string or caller names this address (both PDBs are Dinkumware builds). Was the stand-in
//   _global::RbTree_Clear_006dc9b0 (leaf ?CTCCarryable_OnKill@@YIXPAVCTCCarryable@@@Z) over a local node model calling
//   ?Detach_004e6e43@CTCCarryable@@QAEXI@Z.
// Port (NF90): one explicit instantiation of map::clear on stlport/fable_map.h + engine/CHeroStatIncreaseData.h with TREE_MERASE_NOINLINE
//   (the 0x006DC970 form).
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"

template void std::map<EDamageAttribute, CHeroStatIncreaseData, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, CHeroStatIncreaseData> > >::clear();
