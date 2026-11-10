// std::list<CDefPointer<const CAppearanceModifierDef> >::insert(iterator position, const CDefPointer<const CAppearanceModifierDef>& x) -- link a new node holding a counted copy of x before position; returns its iterator
// Leaf: ?insert@?$list@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@V?$allocator@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@std@@@std@@QAE?AU?$_List_iterator@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@U?$_Nonconst_traits@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@std@@@2@U32@ABV?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BB0A7, 37 bytes, thiscall `ret 0Ch` (hidden result pointer, iterator, value reference);
//   size-optimised TU (odd address). `push [esp+0Ch] ; call 0x005BAC52 (_M_create_node, ecx = this passed through)`, then the
//   STLport 4.6 _list.h link: tmp->_M_next = pos, tmp->_M_prev = pos->_M_prev, prev->_M_next = tmp, pos->_M_prev = tmp, *result = tmp.
// Identity (Codex fix [8]): STLport 4.6 _list.h `iterator insert(iterator __position, const _Tp& __x)`; callers are
//   CTCInventoryClothing::CalculateArmorValues (0x005B5643 / 0x005B57BA; the landed 0x005B4EBA binds this spelling) and
//   list::push_back 0x005BB3EF (`insert(end(), x)`). Was landed as the descriptive free __stdcall ListNode_InsertBefore_005bb0a7.
// Callee: list<CDefPointer<const CAppearanceModifierDef> >::_M_create_node 0x005BAC52 (protected, out of line here as in retail:
//   `push 0Ch ; call malloc ; lea edx,[eax+8]` -- the placement-new null test -- then the CDefPointer copy, `inc [ecx+4]`, `ret 4`;
//   was bound as AllocateInsertNode_005bb0a7; the catalog's CCreatureGenerationFamilyDef label for it is propagated, see NAME_FIXES).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CAppearanceModifierDef.h"
#include "stlport/fable_containers.h"

namespace std {
template <> _List_node<CDefPointer<const CAppearanceModifierDef> >*
list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::_M_create_node(
    const CDefPointer<const CAppearanceModifierDef>& x);                                                  // 0x005BAC52
template list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::iterator
list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::insert(
    iterator position, const CDefPointer<const CAppearanceModifierDef>& x);
} // namespace std
