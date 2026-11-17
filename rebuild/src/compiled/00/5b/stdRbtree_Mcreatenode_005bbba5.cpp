// std::_Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, std::_Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@JU?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Select1st@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@ABU?$pair@$$CBJVCEnginePrimitiveHandle@@@2@@Z
// Flags: /d2inlS15
// Inline-budget: /d2inlS15 -- STLport's inline _Construct inside _M_create_node: retail expands it in this size TU, plain VC7.1 /Os
//   keeps it a call (0x00431471 hand-proof); here retail calls the pair<long const, CEnginePrimitiveHandle> copy constructor 0x005BB37D, and the family value /d2inlS20 also expands the
//   pair copy, which retail keeps out of line (NF61 graded S17 / S18 / S20: DIFFER; S15: RELOCATION_MATCH).
// Header: overlay opt-in FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_COPY_DECL declares the out-of-line copy constructor 0x0043F450 (vtable
//   0x0122F598 store + counted Data copy), which the pair copy COMDAT calls; with the header's implicit copy VC7.1 expands the pair copy.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBBA5, 33 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10626): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x005BBCBE (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `_global::RbTree_CreateNode_sz0x1c_005bbba5`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_COPY_DECL
#include "engine/CEnginePrimitiveHandle.h"
namespace std {
template _Rb_tree_node<pair<long const, CEnginePrimitiveHandle> >* _Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::_M_create_node(const pair<long const, CEnginePrimitiveHandle>&);
}
