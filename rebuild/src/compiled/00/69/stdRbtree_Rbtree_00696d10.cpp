// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, ...>::~_Rb_tree() -- STLport tree destructor: clear() (the out-of-line recursive _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696D10, 66 bytes, thiscall.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ;
//   call _M_erase 0x00696900 ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ;
//   F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Callees: _Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, ...>::_M_erase 0x00696900
//   (landed; declaration only here, FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY), free 0x00BFEA14.
// Callers: none by call, jump or absolute reference (xref.py); an uncalled COMDAT.
// Name fix (NF203, q12018): the landed row was the generated CRetailTree_erase00696900 destructor (one of 119 bodies named by their erase
//   callee). The callee is CCreatureActionBase::RegisteredActionEvents' tree _M_erase (q12018). The body is byte-identical for ~_Rb_tree and
//   the implicit ~map; this is the tree's: it sits in the tree run (_M_erase 0x00696900, clear 0x00696AF0, then 0x00696D10 / 0x00696D60 for
//   the two trees), before the map run (map() 0x00696DE0, ~map 0x00696E20), as the landed ~_Rb_tree 0x006F1BA0 / ~map 0x006F24D0 pairs.
//   STL instance spelling descriptive-by-library (stlport/fable_map.h; no PDB holds STL instances). Explicit instantiation, as 0x006F1BA0.
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

template std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > > >::~_Rb_tree();
