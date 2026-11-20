// std::map<CCharString, CCharString>::~map() -- STLport map destructor (the tree's): clear() (the out-of-line recursive _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC310, 66 bytes, thiscall.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ;
//   call _M_erase 0x009AA830 ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ;
//   F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Identity (q6375 / q6379): Ego_r.pdb (0x00AF9BB0) and FableWin.pdb (0x02FCE5E0) ??1?$map@VCCharString@@V1@...@QAE@XZ, out of line in
//   both; 34 retail call sites reach 0x009AC310 (map<CCharString,CCharString> members of NScript scripts, e.g. csargs), and 9 landed
//   callers on stlport/fable_map.h (0x00D33730, 0x00E55CE0, ...) bind this spelling. The byte-identical 0x009AB770 has no caller (the
//   tree's own destructor). Callee: the tree's _M_erase 0x009AA830 (?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@std@@...,
//   the landed self row 0x009AA830), free 0x00BFEA14.
// NF132: was the generated CRetailTree_erase009aa830 destructor (one of 108 bodies named by their erase callee) over a local tree model;
//   now stlport/fable_map.h (FABLE_RETAIL_STLP_MAP) with the 0x007AEB70 recipe: the helper below (not a retail function) makes VC7.1
//   emit the implicit destructor as its COMDAT.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

namespace {
// Fidelity: scaffolding, not retail: calls the implicit std::map<CCharString,CCharString> destructor so this object emits it out of line as a pick-any COMDAT (GOTCHAS
// nf86 / nf88). Internal to this TU; never called.
void DestroyMapOfCCharStringCCharString(std::map<CCharString, CCharString>* container) { container->~map(); }
}
