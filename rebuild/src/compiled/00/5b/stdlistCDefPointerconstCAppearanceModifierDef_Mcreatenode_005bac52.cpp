// std::list<CDefPointer<const CAppearanceModifierDef> >::_M_create_node(const CDefPointer<const CAppearanceModifierDef>& x) -- STLport list node factory: allocate a 12-byte node and copy-construct the counted def pointer into it
// Leaf: ?_M_create_node@?$list@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@V?$allocator@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@std@@@std@@IAEPAU?$_List_node@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@2@ABV?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BAC52, 33 bytes, thiscall `ret 4` (`this` unused: the allocator is stateless);
//   size-optimised TU (odd address). `push 0Ch ; call malloc 0x00BFEA0E ; lea edx,[eax+8] ; test edx,edx` (the placement-new null
//   test on &node->_M_data), then the CDefPointer copy `Object = x.Object ; if (Object) inc [Object+4]`.
// Identity: STLport 4.6 _list.h `_Node* _M_create_node(const _Tp& __x)` (protected, IAE as in the landed 0x009F7AD0 / 0x00DA0200
//   spellings). Its only caller is list<CDefPointer<const CAppearanceModifierDef> >::insert 0x005BB0A7 (Codex fix [8]; called by
//   CTCInventoryClothing::CalculateArmorValues 0x005B5643 / 0x005B57BA, whose value type is the appearance-modifier def list of
//   CTCHeroAttachableAppearanceModifiers). Was landed as list<CDefPointer<const CCreatureGenerationFamilyDef> >::_M_create_node with a
//   local model (bare `_M_create_node` leaf): that T came from a manifest label propagated across byte-identical bodies (every
//   CDefPointer<T> instantiation compiles to these bytes), not from a caller.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CAppearanceModifierDef.h"
#include "stlport/fable_containers.h"

namespace std {
template _List_node<CDefPointer<const CAppearanceModifierDef> >*
list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::_M_create_node(
    const CDefPointer<const CAppearanceModifierDef>& x);
} // namespace std
