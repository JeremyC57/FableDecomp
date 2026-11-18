// std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::insert_unique(std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@W4EHitLocationCodelet@@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Select1st@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@U?$less@W4EHitLocationCodelet@@@3@V?$allocator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00818000, oracle length 180 bytes.
// Identity: the tree of NHitLocations::CHitLocation::RegisteredHitLocationCodelets (0x013BAF2C),
//   a private static map<EHitLocationCodelet, void (__fastcall*)(CThing&, const CHitParameters&)> (Ego_d publics + LF_STMEMBER; Ego_r.pdb / FableWin.pdb
//   `dynamic initializer for 'NHitLocations::CHitLocation::RegisteredHitLocationCodelets'`; docs/journal/2026-11/EGOD_FINDINGS.md
//   section 1).
//   CHitLocation::StatRegisterClass 0x008162F0 inserts its handlers through this address (map::insert inline); it calls the tree's out-of-line
//   _M_insert 0x00817D70. Spelled from stlport/fable_map.h, not from Ego_d's VC10 Dinkumware _Tree.
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. agentNF69 (q10826): was the generated CRetailTree stand-in _unattributed::InsertUnique_CRetailTree_00818000.
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CThing;
class CHitParameters;
enum EHitLocationCodelet;
template std::pair<std::_Rb_tree_iterator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Nonconst_traits<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >, bool> std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::insert_unique(std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> const&);
