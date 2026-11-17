// std::_Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, std::_Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@KU?$pair@$$CBKK@std@@U?$_Select1st@U?$pair@$$CBKK@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBKK@std@@@2@ABU?$pair@$$CBKK@2@@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 -- STLport's inline _Construct (placement new of the pair copy) inside _M_create_node: retail expands
//   it in this size TU (`lea ..,[..+10h] ; test ; je`, then the value copy), plain VC7.1 /Os keeps it a call. NF61 hand-proof:
//   0x00431471 calls `_Construct` under plain /Os, /O1 and /d2inlT200, expands it under /d2inlS15 / S25 / S40 (/O2 expands it
//   but cleans malloc's argument with `add esp,4`, retail `pop ecx`); 0x00466255 (CCharString value) still calls the pair copy
//   constructor under /d2inlS15 and matches under S20 / S25 / S40. S20 is the smallest value that matches the whole family.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431471, 34 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10590): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x004313E9 (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `_global::RbTree_CreateNode_sz0x18_00431471`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree_node<pair<unsigned long const, unsigned long> >* _Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::_M_create_node(const pair<unsigned long const, unsigned long>&);
}
