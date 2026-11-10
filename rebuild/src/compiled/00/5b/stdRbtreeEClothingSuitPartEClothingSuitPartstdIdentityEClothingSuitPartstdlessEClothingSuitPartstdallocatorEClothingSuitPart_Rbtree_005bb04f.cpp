// std::_Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::~_Rb_tree() -- clear every node, then free the header node (the set<EClothingSuitPart> destructor's body)
// Leaf: ??1?$_Rb_tree@W4EClothingSuitPart@@W41@U?$_Identity@W4EClothingSuitPart@@@std@@U?$less@W4EClothingSuitPart@@@3@V?$allocator@W4EClothingSuitPart@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BB04F, 23 bytes, thiscall `ret`; size-optimised TU (odd address).
//   `push esi ; mov esi,ecx ; call 0x005BABB8 (clear) ; mov esi,[esi] ; test esi,esi ; je ; push esi ; call free ; pop ecx`.
// Identity (Codex fix [7]): STLport 4.6 _tree.h `~_Rb_tree() { clear(); }` with the base `~_Rb_tree_base()` expanded
//   (`_M_header.deallocate(_M_header._M_data, 1)`, the null-tested free). Its callers are the set<EClothingSuitPart> locals of
//   CTCInventoryClothing::GetCurrentTotalArmorValue 0x005B4A95 and CalculateArmorValues 0x005B4EBA (the empty ~set is expanded
//   there; the unreferenced 5-byte `jmp` at 0x005BB3CC is the out-of-line ~set); the landed 0x005B4EBA binds this spelling.
//   Was landed as a descriptive __fastcall OnReadFinished_3 (catalog _global::StdList_Dtor_005bb04f; manifest CActiveFile, propagated).
// Callees: _Rb_tree<EClothingSuitPart>::clear 0x005BABB8 (out of line, FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE), MSVCR71 free 0x00BFEA14.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::~_Rb_tree();
} // namespace std
