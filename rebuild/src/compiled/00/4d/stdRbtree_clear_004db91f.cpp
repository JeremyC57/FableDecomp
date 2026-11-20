// std::_Rb_tree<long, std::pair<const long, long>, std::_Select1st<...>, std::less<long>, std::allocator<...> >::clear() -- STLport map<long, long> clear: when the tree has nodes, erase from the root and reset the header to the empty state
// Leaf: ?clear@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAEXXZ
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB91F (COMDAT packed after the preceding function), thiscall, 38 bytes; calls _M_erase 0x004DA2AC.
// Identity: STLport 4.6 _tree.h clear() of map<long, long> (no PDB copy: both PDBs are Dinkumware; descriptive-by-library): the body
//   (node count +4, header parent/left/right reset) and its _M_erase callee; the landed 0x004E52F0 calls it under this spelling.
// NF108 (2026-11-19, q4185): re-authored on the shared stlport/fable_map.h as the explicit instantiation (was the AutoTinyOwnedList::OnKill
//   stand-in, catalog "_global::RbTree_Clear_004db91f", whose __stdcall AutoTinyFreeListAllocation_004da2ac bound the _M_erase address).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_map.h"

typedef std::map<long, long> TLongLongMap;
template void TLongLongMap::_Rep_type::clear();
