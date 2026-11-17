// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<long, long> >, std::_Select1st<pair<NUISystem::CComponent* const, std::pair<long, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<long, long> > > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read.
// TU: /Gr (fastcall default): retail calls this instance's _Construct out of line as a __fastcall COMDAT (0x005B1B20, landed as
//   ??$_Construct@...@std@@YIX..., catalog flags /O1 /Oy- /Gr) with ecx = &node->_M_value_field, edx = &x; the header's plain-inline
//   _Construct takes the TU default convention, so /Gr gives that call and its spelling (no inline-budget flag in this row)
// Leaf: ?_M_create_node@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@4@@std@@IAEPAU?$_Rb_tree_node@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@ABU?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@2@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B200C, 29 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10621): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x005B2337 (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `NEditGui::CRegionDialog::GetTreeHandleFromMapIndex`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template _Rb_tree_node<pair<NUISystem::CComponent* const, std::pair<long, long> > >* _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<long, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<long, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<long, long> > > >::_M_create_node(const pair<NUISystem::CComponent* const, std::pair<long, long> >&);
}
