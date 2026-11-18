// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, ...>::_M_erase(_Rb_tree_node* x) -- STLport 4.6 recursive tree erase:
// for each node down the left spine, erase the right subtree and free the node (the pair<const EDamageAttribute, float> value is trivial)
// Leaf: ?_M_erase@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EDamageAttribute@@M@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450903, a COMDAT in a size-optimised TU.
// Identity (NF81, round 7, q10986): the _M_erase of the map<EDamageAttribute, float> tree (Ego_r.pdb CComboMultiplierData::ComboMultiplier,
//   CHitParameters damage maps): NF78's 0x006DCFA0 binds it so for the inner-map destructors (RM), and the landed 0x006DC350 / 0x00868A40
//   call it for that map. Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the propagated label
//   LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray (a local TreeNode model).
// Port (NF81): one explicit instantiation of the stlport/fable_map.h _M_erase (the 0x005BA78B recipe).
#pragma optimize("s",on)
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, _Select1st<pair<const EDamageAttribute, float> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, float> > >::_M_erase(_Rb_tree_node<pair<const EDamageAttribute, float> >* x);
}
