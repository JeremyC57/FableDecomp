// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, ...>::clear() -- STLport tree clear: when the tree holds nodes, the out-of-line recursive _M_erase from the root, then the header's left / parent / right relinked (header re-read per store) and the count zeroed
// Leaf: ?clear@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696AF0, 51 bytes, thiscall, `ret`.
// Callee: _Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, ...>::_M_erase 0x00696900
//   (landed; declaration only here, FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY).
// Callers: none by call, jump or absolute reference (xref.py); an uncalled COMDAT.
// Name fix (NF203, q12018): the landed row was a July stand-in (CTCCarryable_OnKill, a header-less local CTCCarryable whose
//   "Detach_00696900" bound the _M_erase 0x00696900; catalog _global::RbTree_Clear_00696af0). The body is STLport 4.6 _tree.h
//   _Rb_tree::clear (stlport/fable_map.h, the default inline body) over CCreatureActionBase::RegisteredActionEvents' tree: its callee is that
//   tree's _M_erase, and it sits in the tree run (_M_erase 0x00696900 / 0x00696970, clear 0x00696AF0 / 0x00696B30, ~_Rb_tree 0x00696D10 /
//   0x00696D60) before the two map runs. STL instance spelling descriptive-by-library (no PDB holds STL instances).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
namespace NCreatureAction { class CActionEventBase; }

template void std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > > >::clear();
