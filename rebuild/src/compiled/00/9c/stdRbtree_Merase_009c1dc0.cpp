// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- STLport 4.6 _M_erase: erase without rebalancing -- recurse right, destroy the value, free the node, continue left (std::set<CDisplayModeDesc >)
// Leaf: ?_M_erase@?$_Rb_tree@VCDisplayModeDesc@@V1@U?$_Identity@VCDisplayModeDesc@@@std@@U?$less@VCDisplayModeDesc@@@3@V?$allocator@VCDisplayModeDesc@@@3@@std@@AAEXPAU?$_Rb_tree_node@VCDisplayModeDesc@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C1DC0, oracle length 51 bytes.
// Name (agentNF23, name-fix queue land1011da, agentFAM07 proof): agentFAM07: the _M_erase of the tree whose constructor 0x009C2B00 was renamed this pass (its destructor, the next COMDAT, calls 0x009C1DC0; proven type in RENAMES.tsv); STLport 4.6 stl/_tree.h declares _Rb_tree::_M_erase private (the landed 0x00529CE0 template text), so VC7.1 spells it AAE
//   Was the stand-in ?FreeTree_CTreeOwner_009c1dc0@CTreeOwner_009c1dc0@@QAEXPAUSNode@@@Z (catalog _unattributed).
// Identity (proven, agentFAM07): CDisplayManager::EnumerateDisplayModes 0x009C02D0 builds a local set of display modes and calls this tree's _M_erase 0x009C1DC0,
//   landed and bound as _Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity<..>, ...>; ??0 / ??1 of set<CDisplayModeDesc> are in the
//   PDB name sets; the ctor is the COMDAT right before the set's destructor 0x009C2B40; node 0x20 = 0x10 + CDisplayModeDesc 0x10.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
// Model (agentNF24 port of agentNF23's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CDisplayModeDesc.h replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CDisplayModeDesc.h"

template void std::_Rb_tree<CDisplayModeDesc, CDisplayModeDesc, std::_Identity<CDisplayModeDesc>, std::less<CDisplayModeDesc>, std::allocator<CDisplayModeDesc> >::_M_erase(std::_Rb_tree_node<CDisplayModeDesc>*);
