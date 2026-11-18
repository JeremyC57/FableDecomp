// std::map<EDamageAttribute, float>::clear() -- STLport _map.h `_M_t.clear()` with the tree clear expanded: when the tree holds nodes,
// erase them all (the out-of-line _M_erase 0x00450903 from the root) and reset the header to an empty tree
// Leaf: ?clear@?$map@W4EDamageAttribute@@MU?$less@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00869900, 51 bytes, a COMDAT in a speed-optimised TU (beside CHitParameters::ClearAllDamage
//   0x00868A40); no E8 / E9 / absolute reference to it in Fable.exe.
// Identity (NF83, round 7, q11012 binder): the body is STLport's tree clear over the map<EDamageAttribute, float> tree (its callee
//   0x00450903 is that tree's _M_erase, NF81). It cannot carry the tree clear's own name: 0x0045128D holds
//   ?clear@?$_Rb_tree@W4EDamageAttribute@@... (NF81, RM; bound so by the landed operator= 0x004614C0) and retail has no
//   identical-code folding, so a second COMDAT of one name cannot exist. STLport's map::clear (`_M_t.clear()`, inline) with the tree
//   clear expanded is the remaining library function with this text. DESCRIPTIVE-by-library: no PDB, string or caller names this
//   address (both PDBs are Dinkumware builds); the map spelling follows STLport 4.6 _map.h (4-argument map). Was the stand-in
//   _global::RbTree_Clear_00869900 (leaf ?CTCCarryable_OnKill@@YIXPAVCTCCarryable@@@Z, also defined by 0x006DC970) over a local
//   node model calling ?Detach_00450903@CTCCarryable@@QAEXI@Z.
// Port (NF83): one explicit instantiation of map::clear on stlport/fable_map.h with TREE_MERASE_NOINLINE (the _M_erase COMDAT called,
//   not expanded, as in 0x006DC020 / 0x006DC350).
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"

template void std::map<EDamageAttribute, float, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, float> > >::clear();
