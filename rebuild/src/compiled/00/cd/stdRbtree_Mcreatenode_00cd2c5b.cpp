// std::_Rb_tree<int, pair<int const, CCharString>, std::_Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@HU?$pair@$$CBHVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBHVCCharString@@@std@@@2@U?$less@H@2@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBHVCCharString@@@std@@@2@ABU?$pair@$$CBHVCCharString@@@2@@Z
// Flags: /d2inlS15
// Inline-budget: /d2inlS15 -- STLport's inline _Construct inside _M_create_node: retail expands it in this size TU, plain VC7.1 /Os
//   keeps it a call (0x00431471 hand-proof); here retail calls the pair<int const, CCharString> copy constructor 0x00CD2395, and the family value /d2inlS20 also expands the
//   pair copy, which retail keeps out of line (NF61 graded S20: DIFFER; S15: RELOCATION_MATCH).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2C5B, 33 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10635): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x00CD300B (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `_global::RbTree_CreateNode_sz0x18_00cd2c5b`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace std {
template _Rb_tree_node<pair<int const, CCharString> >* _Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::_M_create_node(const pair<int const, CCharString>&);
}
