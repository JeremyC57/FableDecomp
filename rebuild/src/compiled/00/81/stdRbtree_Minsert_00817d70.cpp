// std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> const&, std::_Rb_tree_node_base*) -- STLport 4.6 _M_insert (stlport/fable_map.h)
// Leaf: ?_M_insert@?$_Rb_tree@W4EHitLocationCodelet@@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Select1st@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@U?$less@W4EHitLocationCodelet@@@3@V?$allocator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00817D70, oracle length 191 bytes.
// Identity: the tree of NHitLocations::CHitLocation::RegisteredHitLocationCodelets (0x013BAF2C),
//   a private static map<EHitLocationCodelet, void (__fastcall*)(CThing&, const CHitParameters&)> (Ego_d publics + LF_STMEMBER; Ego_r.pdb / FableWin.pdb
//   `dynamic initializer for 'NHitLocations::CHitLocation::RegisteredHitLocationCodelets'`; docs/journal/2026-11/EGOD_FINDINGS.md
//   section 1).
//   Its insert_unique 0x00818000 (called by CHitLocation::StatRegisterClass 0x008162F0) calls this address, the STLport 4.6 out-of-line _M_insert (0x18-byte
//   node: four header words + key + handler). Spelled from stlport/fable_map.h, not from Ego_d's VC10 Dinkumware _Tree.
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. agentNF69 (q10825): was the generated CRetailTree stand-in _unattributed::Insert_CRetailTree_00817d70.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
class CThing;
class CHitParameters;
enum EHitLocationCodelet;
template std::_Rb_tree_iterator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Nonconst_traits<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > > std::_Rb_tree<EHitLocationCodelet, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)>, std::_Select1st<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> >, std::less<EHitLocationCodelet>, std::allocator<std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> > >::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, std::pair<EHitLocationCodelet const, void (__fastcall*)(CThing&, CHitParameters const&)> const&, std::_Rb_tree_node_base*);
