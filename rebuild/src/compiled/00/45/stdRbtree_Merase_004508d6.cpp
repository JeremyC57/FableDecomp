// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, float>, ...>::_M_erase(_Rb_tree_node* x) -- STLport 4.6
//   recursive subtree free: recurse on the right child (+0xC), free the node, walk to the left child (+8)
// Leaf: ?_M_erase@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004508D6, 45 bytes, a COMDAT in a size-optimised TU.
// Identity (NF88, q11151): the _M_erase of the map<EObjectAugmentationType, float> tree (CArmourDef::AugmentationResponse). Its only
//   caller besides itself is that tree's clear 0x00451240 (callers.py), which already binds 0x004508D6 under exactly this spelling
//   (NF86 explicit instantiation on stlport/fable_map.h). Byte shape = the EDamageAttribute twin 0x00450903 and the
//   EHeroExperienceStatCategory twin 0x004DA252. Descriptive by library (STLport 4.6; both PDBs are Dinkumware builds). Was the stand-in
//   LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray (leaf ?BuildTreeArray@@YIXPAUAutoTinyTreeWalker@@UAutoTinyTreeNodeArgument@@@Z).
// Port (NF88): one explicit instantiation on stlport/fable_map.h (the 0x004DA252 recipe).
#pragma optimize("s",on)
#include "engine/EObjectAugmentationType.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, float>, _Select1st<pair<const EObjectAugmentationType, float> >, less<EObjectAugmentationType>, allocator<pair<const EObjectAugmentationType, float> > >::_M_erase(_Rb_tree_node<pair<const EObjectAugmentationType, float> >* x);
}
