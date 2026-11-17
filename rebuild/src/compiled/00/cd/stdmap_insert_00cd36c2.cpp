// std::map<int, CCharString>::insert(iterator position, const value_type& x) -- STLport 4.6 _map.h: forward to the tree's hinted insert_unique (0x00CD34C5), the iterator result through the hidden pointer
// Leaf: ?insert@?$map@HVCCharString@@U?$less@H@std@@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBHVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBHVCCharString@@@std@@@2@@2@U32@ABU?$pair@$$CBHVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD36C2, 29 bytes, size TU, thiscall `ret 0Ch` (hidden result, position by value, &x).
// Retail bytes:  push [x] ; mov edx,[position] ; push ecx ; mov eax,esp ; push [result] ; mov [eax],edx ; call insert_unique ;
//   mov eax,[result] ; ret 0Ch -- the by-value iterator (user copy constructor) copied into a reserved slot.
// Identity (NF61, round 7, q10586): the out-of-line map::insert(iterator, const value_type&) COMDAT of the map<int, CCharString> whose
//   hinted insert_unique 0x00CD34C5 NF58 landed (instance proven there: PDB PlayCutsceneSound(map<int, CCharString>&); the node copy
//   0x00CD2C5B); its only caller is that map's operator[] 0x00CD3A16 (the `map::insert(i, value_type(k, T()))` of STLport's subscript),
//   which binds the long-keyed spelling of this member (NAME_FIXES). Explicit instantiation of stlport/fable_map.h map::insert.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds). Was the stand-in
//   _unattributed::Forward_CForwarder_00cd36c2 (the manifest label "CPersistTraits<C2DVector>::TransferIn" was propagated).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_map.h"

namespace std {
template map<int, CCharString>::iterator map<int, CCharString>::insert(map<int, CCharString>::iterator, const map<int, CCharString>::value_type&);
}
