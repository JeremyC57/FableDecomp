// std::map<CCharString, std::vector<CCharString> >::~map() -- STLport map destructor (the tree's): clear() (the out-of-line recursive _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$map@VCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AEB70, 66 bytes, thiscall.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ;
//   call _M_erase 0x007AE0E0 ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ;
//   F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Callees: _Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >, ...>::_M_erase 0x007AE0E0, free 0x00BFEA14.
// Callers: none by call, jump or absolute reference (xref.py); an uncalled COMDAT, like its neighbour.
// Name fix (NF59, queue q10550): the landed row was the generated CRetailTree_erase007ae0e0 destructor (one of 108 bodies named by
//   their erase callee). The callee is this map's tree _M_erase (q10550; FableWin.pdb has ??1?$map@VCCharString@@V?$vector@VCCharString@@...
//   0x01E8D0F0 for the same map). The body is byte-identical for ~_Rb_tree and the implicit ~map; the map destructor is chosen because
//   the adjacent COMDAT 0x007AEB30 (landed) is the same map's uncalled map() constructor and 0x007AEBE0 its operator[] -- a map
//   member run, not a tree one; the landed
//   0x006F2490 map() / 0x006F24D0 ~map pair has the same layout, its ~_Rb_tree 0x006F1BA0 following the tree constructor instead. The local tree model is replaced by fable_string.h + stlport/fable_containers.h + stlport/fable_map.h
//   (FABLE_RETAIL_STLP_MAP); the helper below (not a retail function) makes VC7.1 emit the implicit destructor as its COMDAT, as the
//   landed ~map 0x006F24D0 does.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

void DestroyMapOfCCharStringVectorCCharString(std::map<CCharString, std::vector<CCharString> >* container) { container->~map(); }
