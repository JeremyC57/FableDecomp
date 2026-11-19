// std::_List_base<NPlayerGui::CDrawDamage::SDamageValue>::~_List_base() -- STLport list teardown: clear() frees every node and
//   re-links the sentinel to itself, then the sentinel is freed
// Leaf: ??1?$_List_base@VSDamageValue@CDrawDamage@NPlayerGui@@V?$allocator@VSDamageValue@CDrawDamage@NPlayerGui@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043FCF0, thiscall, no arguments.
// Identity (NF93 q11305, egod e14 claim): the body is STLport ~_List_base (clear, re-link the header, free the header); it is called on
//   CDrawDamage::DamageValues (+0x28, std::list<SDamageValue>, Ego_r.pdb) from ~CDrawDamage 0x00441C70, the scalar deleting destructor
//   0x00442D50 and CCountedPointer<CDrawDamage>::DeleteData 0x00443840, which already binds this spelling (reloc_bindings.tsv).
//   Descriptive-by-library (STLport; the PDBs are Dinkumware builds). Was the made-up local List::Destroy model.
// Callees: free 0x00BFEA14 (the node allocator's deallocate).
// Shape: stlport/fable_containers.h's default ~_List_base { clear(); deallocate(node, 1); } emitted through an explicit instantiation.
#include "engine/NPlayerGui_CDrawDamage.h"

template std::_List_base<NPlayerGui::CDrawDamage::SDamageValue, std::allocator<NPlayerGui::CDrawDamage::SDamageValue> >::~_List_base();
