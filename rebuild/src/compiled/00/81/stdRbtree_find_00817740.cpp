// _Rb_tree<EHitLocationCodelet, pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, _Select1st<pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, less<EHitLocationCodelet>, allocator<pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::_Rb_tree::find<K>(const K& key) -- STLport 4.6 member-template find (iterator(_M_find(key)), returned through the hidden pointer)
// Leaf: ??$find@W4EHitLocationCodelet@@@?$_Rb_tree@W4EHitLocationCodelet@@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Select1st@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@U?$less@W4EHitLocationCodelet@@@3@V?$allocator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@2@@1@ABW4EHitLocationCodelet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00817740.
// Identity: the tree of NHitLocations::CHitLocation::RegisteredHitLocationCodelets (0x013BAF2C) (Ego_d's map type, EGOD_FINDINGS section 1); descriptive-by-library (STLport 4.6 spelling from stlport/fable_map.h;
//   both PDBs are Dinkumware).
// NF72 (round 7, q10897): was a generic stand-in tree (descriptive address-suffixed class); re-authored as the explicit instantiation of fable_map.h FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (the STLport 4.6.1 _tree.h body),
//   the form of NF69's landed tree rows of the same maps (0x00818000 / 0x008B2DD0); the callers NF69 ported bind this symbol.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#include "stlport/fable_map.h"
class CThing;
class CHitParameters;
enum EHitLocationCodelet;
template std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::iterator std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::find<EHitLocationCodelet>(EHitLocationCodelet const&);
