// std::_Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::clear() -- erase every node from the root down, then reset the header to the empty tree
// Leaf: ?clear@?$_Rb_tree@W4EClothingSuitPart@@W41@U?$_Identity@W4EClothingSuitPart@@@std@@U?$less@W4EClothingSuitPart@@@3@V?$allocator@W4EClothingSuitPart@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BABB8, 41 bytes, thiscall `ret`; size-optimised TU (odd address).
//   `cmp dword ptr [esi+4],0 ; je` (_M_node_count), `push [header+4] ; call 0x005BA78B` (_M_erase(root)), then header->_M_left =
//   header, _M_parent = 0, _M_right = header, _M_node_count = 0 (STLport 4.6 _tree.h clear(), the shared-header body).
// Identity (with Codex fix [7]): called by ~_Rb_tree<EClothingSuitPart> 0x005BB04F and by erase(first, last) 0x005BB102 (the
//   landed 0x005BB102 already binds this spelling). Was landed as a descriptive __fastcall OnKill (catalog
//   _global::RbTree_Clear_005babb8; manifest CTCCarryable::OnKill, propagated).
// Callee: _Rb_tree<EClothingSuitPart>::_M_erase 0x005BA78B (private, recursive, out of line).
#pragma optimize("s",on)
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_map.h"

namespace std {
template void _Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::clear();
} // namespace std
