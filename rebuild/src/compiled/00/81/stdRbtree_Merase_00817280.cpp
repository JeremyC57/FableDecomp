// _Rb_tree<EHitLocationCodelet, pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, _Select1st<pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, less<EHitLocationCodelet>, allocator<pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::_M_erase(_Rb_tree_node<V>* x) -- STLport 4.6 _tree.c: erase the subtree without rebalancing (recursive on the right, iterative on the left)
// Leaf: ?_M_erase@?$_Rb_tree@W4EHitLocationCodelet@@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Select1st@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@U?$less@W4EHitLocationCodelet@@@3@V?$allocator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00817280.
// Identity: the tree of NHitLocations::CHitLocation::RegisteredHitLocationCodelets (0x013BAF2C) (Ego_d's map type, EGOD_FINDINGS section 1); descriptive-by-library (STLport 4.6 spelling from stlport/fable_map.h;
//   both PDBs are Dinkumware).
// NF72 (round 7, q10889): was a generic stand-in tree (descriptive address-suffixed class); re-authored as the explicit instantiation of the out-of-line _Rb_tree member (fable_map.h),
//   the form of NF69's landed tree rows of the same maps (0x00818000 / 0x008B2DD0); the callers NF69 ported bind this symbol.
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CThing;
class CHitParameters;
enum EHitLocationCodelet;
template void std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::_M_erase(std::_Rb_tree_node<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >*);
