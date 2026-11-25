// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, CObjectAugmentationParticleSet>, ...>::_M_erase(_Rb_tree_node* x) -- STLport 4.6
// recursive tree erase: for each node down the left spine, erase the right subtree, destroy the value
// (pair<const EObjectAugmentationType, CObjectAugmentationParticleSet>) and free the node
// Leaf: ?_M_erase@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7772, 55 bytes, a COMDAT in a size-optimised TU. Byte twin (relocations aside) of 0x004E6E43
//   (the EDamageAttribute tree) and 0x004E6E0C.
// Identity (NF409): the _M_erase of the map<EObjectAugmentationType, CObjectAugmentationParticleSet> tree (Ego_r.pdb
//   CObjectAugmentationsDef::AugmentationParticles; FableWin.pdb names the map's ??0 / ??1 / ??4): (1) its non-recursive caller is that
//   tree's clear 0x004E8212 (table-bound ?clear@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@
//   VCObjectAugmentationParticleSet@@...), which passes the root (`push [[esi]+4]`), STLport's `_M_erase(_M_root())`; (2) its value
//   destructor 0x004E41A1 is the pair's scalar deleting destructor called with 0 (Ego_r.pdb names ??_G?$pair@$$CBW4EObjectAugmentationType@@
//   VCObjectAugmentationParticleSet@@@std@@QAEPAXI@Z), whose only callee is ??1CObjectAugmentationParticleSet 0x004E25F1.
//   Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the propagated label LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::
//   BuildTreeArray (a local CTextTreeNode / CInner model whose self ?BuildTreeArray@LTextTreeWalkThrough@@QAEXPAUCTextTreeNode@@@Z
//   was also the strong self of four other rows: 0x00451B4B, 0x00579DCE, 0x00593795, 0x005B1E23,
//   and whose value-destructor callee ?Destroy_004e41a1@CInner@@QAEXH@Z was a second name at 0x004E41A1).
// Port (NF409): one explicit instantiation of the stlport/fable_map.h _M_erase (the 0x004E6E43 recipe).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CObjectAugmentationParticleSet.h"
namespace std {
template void _Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, CObjectAugmentationParticleSet>, _Select1st<pair<const EObjectAugmentationType, CObjectAugmentationParticleSet> >, less<EObjectAugmentationType>, allocator<pair<const EObjectAugmentationType, CObjectAugmentationParticleSet> > >::_M_erase(_Rb_tree_node<pair<const EObjectAugmentationType, CObjectAugmentationParticleSet> >* x);
}
