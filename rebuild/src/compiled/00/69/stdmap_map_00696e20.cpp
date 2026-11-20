// std::map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >::~map() -- STLport map destructor (the tree's): clear() (the out-of-line recursive _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$map@VCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@4@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696E20, 66 bytes, thiscall.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ;
//   call _M_erase 0x00696900 ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ;
//   F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Callees: _Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, ...>::_M_erase 0x00696900
//   (landed; declaration only here, FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY), free 0x00BFEA14.
// Callers: none by call, jump or absolute reference (xref.py); an uncalled COMDAT.
// Name fix (NF203, q12018): the landed row was the generated CRetailTree_erase00696900 destructor (one of 119 bodies named by their erase
//   callee). The callee is CCreatureActionBase::RegisteredActionEvents' tree _M_erase (q12018). The body is byte-identical for ~_Rb_tree and
//   the implicit ~map; this is the map's: the adjacent COMDAT 0x00696DE0 (landed) is the same map's map() constructor (map member run
//   map() / ~map, as 0x00696E70 / 0x00696EB0 for the EActionRegisteredType map), while the tree run (_M_erase 0x00696900, clear 0x00696AF0,
//   ~_Rb_tree 0x00696D10) comes first, as the landed 0x007AEB70 / 0x006F24D0 pairs. STL instance spelling descriptive-by-library
//   (stlport/fable_map.h; no PDB holds STL instances). The helper below (not a retail function) makes VC7.1 emit the implicit destructor
//   as its COMDAT, as 0x007AEB70 does.
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

void DestroyMapOfCCharStringActionEvent(std::map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >* container) { container->~map(); }
