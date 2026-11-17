// CTreeOwner_00630d90::FreeTree(SNode*) -- recursive free of a child-linked (+0xc) / sibling-linked (+0x8) node list, CCharString member at +0x14 destroyed first
// Naming (naming wave 22, ccs): the string type is the PDB CCharString (was CMember_0099eae0); name-only.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00630D90, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov esi,[esp+0Ch] ; test esi,esi ; mov ebx,ecx ; je E ; push edi ; L: mov eax,[esi+0xc] ; push eax ; mov ecx,ebx ; call FreeTree ; mov edi,[esi+0x8] ; lea ecx,[esi+0x14] ; call CCharString::~CCharString ; push esi ; call free ; add esp,4 ; test edi,edi ; mov esi,edi ; jne L ; pop edi ; E: pop esi ; pop ebx ; ret 4
// Walks a sibling list: recurses into each node's child list at +0xc, remembers the sibling at
// +0x8, destroys the node's CCharString member at +0x14 (destructor @ 0x0099EAE0), frees the
// node (free @ 0x00BFEA14) and continues with the sibling. The manifest label
// (`LTextTreeWalkThrough::BuildTreeArray`) is propagated; the owner is named descriptively.
// One of 12 bodies of this shape.
// NF50 (round 7): this is STLport 4.6 _tree.c _Rb_tree::_M_erase of map<unsigned long, CCharString> (nf44 entry 55): right link +0xc
// recursed, left link +0x8 kept, the 8-byte pair at +0x10 destroyed (its CCharString at +0x14), the node freed. Moved onto the shared
// stlport/fable_map.h as the explicit instantiation of that body, with the shared fable_string.h CCharString (destructor 0x0099EAE0
// out of line); the self call and the row's own symbol are the STLport _M_erase (descriptive-by-library).
// Leaf: ?_M_erase@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharString@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBKVCCharString@@@std@@@2@@Z
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<unsigned long, pair<const unsigned long, CCharString>, _Select1st<pair<const unsigned long, CCharString> >, less<unsigned long>, allocator<pair<const unsigned long, CCharString> > >::_M_erase(_Rb_tree_node<pair<const unsigned long, CCharString> >*);
}
