// CRetailTree_erase00630d90::~CRetailTree_erase00630d90() -- std::map/set destructor: erase all nodes through 0x00630D90, reset the head, free it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00631420, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Erase ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ; F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Destructor of the retail red-black tree {head, size}: when the tree holds nodes the recursive
// erase @ 0x00630D90 (a __fastcall member taking the root, head->parent) frees them, then the
// head's left/parent/right are reset through `head` re-read per store and size is zeroed; finally
// the head node itself is released with free @ 0x00BFEA14 (null-checked). Only the erase callee
// distinguishes the instantiations, so the class is named by it. One of 108 bodies.
// Leaf: ??1CRetailTree_erase00630d90@@QAE@XZ
// NF50 (round 7): the erase call @ 0x00630D90 binds the STLport
// ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharString@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKVCCharString@@@std@@@2@@Z
// (nf44 entry 55, map<unsigned long, CCharString>, Ego_r.pdb CUIDef::ActionMap) through the shared stlport/fable_map.h: the stand-in
// now derives from that map, whose inline ~map / ~_Rb_tree (clear) / ~_Rb_tree_base give this body. Its own name stays a stand-in:
// the address has no caller and no data reference, so ~_Rb_tree vs. ~map vs. an owner's destructor is not provable (NF50 NOTES).
#include "fable_string.h"
#include "stlport/fable_map.h"

struct CRetailTree_erase00630d90 : std::map<unsigned long, CCharString> {   // stand-in: a map<unsigned long, CCharString> destructor
    ~CRetailTree_erase00630d90();
};

CRetailTree_erase00630d90::~CRetailTree_erase00630d90()
{
}
