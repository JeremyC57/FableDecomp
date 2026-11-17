// std::_Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::_M_copy(x, p) -- STLport 4.6 _tree.c structural copy: clone x (_M_create_node 0x004E597B, colour copied,
//   children cleared) under parent p, recurse into each right subtree, walk the left spine iteratively; returns the cloned top
// Leaf: ?_M_copy@?$_Rb_tree@VCDefString@@U?$pair@$$CBVCDefString@@J@std@@U?$_Select1st@U?$pair@$$CBVCDefString@@J@std@@@3@V?$CCompareDefString@J@@V?$allocator@U?$pair@$$CBVCDefString@@J@std@@@3@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBVCDefString@@J@std@@@2@PAU32@0@Z
// Flags: /Oa
// Inline-budget: FABLE_FORCEINLINE _M_clone_node (overlay opt-in FABLE_RETAIL_STLP_TREE_MCOPY_BODY) -- retail expands it at both
//   clone sites (call _M_create_node, then the colour / child stores in place); VC7.1 /Os calls it out of line, also under
//   /d2inlS20 (NF61 hand-proof on 0x00432AA8: 115 v 144 bytes both ways).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E58EB, 144 bytes, frame-pointer size TU (`optimize("y",off)` + `("s",on)`, catalog /Oa),
//   thiscall `ret 8`; `this` parks in the `push ecx` frame word for the two _M_create_node calls.
// NF61 (round 7, q10610): the private STLport member (AAE), an explicit instantiation of stlport/fable_map.h _M_copy (overlay opt-in
//   FABLE_RETAIL_STLP_TREE_MCOPY_BODY: STLport's _M_clone_node + _M_copy text, exceptions off) for the instance whose _M_create_node
//   0x004E597B it calls (re-authored by NF61 on the spelling NF58's _M_insert 0x004E5BA7 binds; types proven there from the Ego_r.pdb member
//   and the node size); the value-type declarations are that row's. Was the stand-in `CRetailTree_004e597b::Copy` (which bound the stand-in
//   `CRetailTree_004e597b::Buynode`). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND
#include "engine/CDefString.h"
template<class T> class CCompareDefString {   // Ego_r.pdb / FableWin.pdb ??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z; no shared header
public:
    bool operator()(const CDefString& a, const CDefString& b) const { return a.TablePos < b.TablePos; }
};
namespace std {
template _Rb_tree_node<pair<CDefString const, long> >* _Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::_M_copy(_Rb_tree_node<pair<CDefString const, long> >*, _Rb_tree_node<pair<CDefString const, long> >*);
}
