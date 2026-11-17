// std::_Rb_tree<EQuakeLength, pair<EQuakeLength const, float>, std::_Select1st<pair<EQuakeLength const, float> >, less<EQuakeLength >, allocator<pair<EQuakeLength const, float> > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@W4EQuakeLength@@U?$pair@$$CBW4EQuakeLength@@M@std@@U?$_Select1st@U?$pair@$$CBW4EQuakeLength@@M@std@@@3@U?$less@W4EQuakeLength@@@3@V?$allocator@U?$pair@$$CBW4EQuakeLength@@M@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EQuakeLength@@M@std@@@2@ABU?$pair@$$CBW4EQuakeLength@@M@2@@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 -- STLport's inline _Construct (placement new of the pair copy) inside _M_create_node: retail expands
//   it in this size TU (`lea ..,[..+10h] ; test ; je`, then the value copy), plain VC7.1 /Os keeps it a call. NF61 hand-proof:
//   0x00431471 calls `_Construct` under plain /Os, /O1 and /d2inlT200, expands it under /d2inlS15 / S25 / S40 (/O2 expands it
//   but cleans malloc's argument with `add esp,4`, retail `pop ecx`); 0x00466255 (CCharString value) still calls the pair copy
//   constructor under /d2inlS15 and matches under S20 / S25 / S40. S20 is the smallest value that matches the whole family.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6748, 34 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10613): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x004E6C39 (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `_global::RbTree_CreateNode_sz0x18_004e6748`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#include "engine/NCreatureActionEvents.h"
namespace std {
template _Rb_tree_node<pair<EQuakeLength const, float> >* _Rb_tree<EQuakeLength, pair<EQuakeLength const, float>, _Select1st<pair<EQuakeLength const, float> >, less<EQuakeLength >, allocator<pair<EQuakeLength const, float> > >::_M_create_node(const pair<EQuakeLength const, float>&);
}
