// std::_Rb_tree_iterator<std::pair<const long, long>, std::_Const_traits<std::pair<const long, long> > >::operator++() -- STLport 4.6
// map<long, long> const_iterator pre-increment: _Rb_global<bool>::_M_increment (0x004292C0, out of line here) on the node, return *this.
// Leaf: ??E?$_Rb_tree_iterator@U?$pair@$$CBJJ@std@@U?$_Const_traits@U?$pair@$$CBJJ@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B16FF (size TU): push esi; mov esi,ecx; push [esi]; call 0x004292C0; mov [esi],eax; pop ecx;
//   mov eax,esi; pop esi; ret (was the placeholder _global::RbTreeIterator_PreIncrement_005b16ff, leaf ?M@S@@QAEPAU1@XZ).
// Naming (agentNF32, 2026-11-12): DESCRIPTIVE-BY-LIBRARY. Retail is STLport 4.6; Ego_r.pdb / FableWin.pdb are Dinkumware builds, so no
//   PDB carries this spelling. Evidence: the body is the stlport/fable_map.h _Rb_tree_iterator::operator++ (one call of the cdecl
//   _Rb_global<bool>::_M_increment, the result stored back to _M_node, `this` returned); the landed CTCWife::FindAppropriateGift
//   0x006CF530 walks a map<long, long> with a const_iterator and binds this address under this exact symbol
//   (rebuild/oracles/reloc_bindings.tsv). No template argument beyond that call site is used.
#pragma optimize("s", on)
#include "stlport/fable_map.h"

template std::_Rb_tree_iterator<std::pair<const long, long>, std::_Const_traits<std::pair<const long, long> > >&
    std::_Rb_tree_iterator<std::pair<const long, long>, std::_Const_traits<std::pair<const long, long> > >::operator++();
