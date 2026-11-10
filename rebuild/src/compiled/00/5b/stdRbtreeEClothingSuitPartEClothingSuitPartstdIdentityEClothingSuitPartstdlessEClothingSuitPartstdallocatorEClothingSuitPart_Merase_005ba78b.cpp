// std::_Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::_M_erase(_Rb_tree_node<EClothingSuitPart>* x) -- free a subtree without rebalancing: recurse right, step left, free each node
// Leaf: ?_M_erase@?$_Rb_tree@W4EClothingSuitPart@@W41@U?$_Identity@W4EClothingSuitPart@@@std@@U?$less@W4EClothingSuitPart@@@3@V?$allocator@W4EClothingSuitPart@@@3@@std@@AAEXPAU?$_Rb_tree_node@W4EClothingSuitPart@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BA78B, thiscall `ret 4`; size-optimised TU (odd address). Loop on x: `push [x+0Ch] ;
//   call 0x005BA78B` (_M_erase(x->_M_right), recursive), y = x->_M_left (+8), free(x), x = y (STLport 4.6 _tree.c _M_erase).
// Identity (with Codex fix [7]): its only callers are itself (0x005BA79D) and _Rb_tree<EClothingSuitPart>::clear 0x005BABB8.
//   Private in STLport 4.6 (AAE, as the landed _M_erase spellings, e.g. _Rb_tree<VCDisplayModeDesc>). Was landed as a descriptive
//   __fastcall BuildTreeArray (catalog LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray, a label propagated across
//   the byte-identical _M_erase bodies of every trivially-destructible 4-byte-value tree).
// Callee: MSVCR71 free 0x00BFEA14.
#pragma optimize("s",on)
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_map.h"

namespace std {
template void _Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::_M_erase(_Rb_tree_node<EClothingSuitPart>* x);
} // namespace std
