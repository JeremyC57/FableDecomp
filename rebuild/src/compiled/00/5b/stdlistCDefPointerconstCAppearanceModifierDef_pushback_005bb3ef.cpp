// std::list<CDefPointer<const CAppearanceModifierDef> >::push_back(const CDefPointer<const CAppearanceModifierDef>& x) -- append a counted copy of x (insert(end(), x))
// Leaf: ?push_back@?$list@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@V?$allocator@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@std@@@std@@QAEXABV?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BB3EF, 26 bytes, thiscall `ret 4`; size-optimised TU with frame pointers (push ebp / mov
//   ebp,esp): `push [ebp+8] ; mov edx,[ecx] ; push ecx ; mov eax,esp ; mov [eax],edx` (end() built in place in the argument slot),
//   `lea eax,[ebp+8] ; push eax` (the discarded result in x's own slot), `call 0x005BB0A7`.
// Identity (Codex fix [9]): STLport 4.6 _list.h `void push_back(const _Tp& __x) { insert(end(), __x); }`; an unreferenced COMDAT
//   (no caller in retail -- CalculateArmorValues expands push_back and calls insert directly). Was landed as the descriptive
//   ListWrapper_005bb3ef::ListPushFrontWrapper_005bb3ef, binding ?InsertBefore@ListWrapper_005bb3ef.. at 0x005BB0A7.
// Callee: list<CDefPointer<const CAppearanceModifierDef> >::insert(iterator, const T&) 0x005BB0A7 (out of line, as in retail).
#pragma optimize("s",on)
#pragma optimize("y",off)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CAppearanceModifierDef.h"
#include "stlport/fable_containers.h"

namespace std {
template <> list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::iterator
list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::insert(
    iterator position, const CDefPointer<const CAppearanceModifierDef>& x);                              // 0x005BB0A7
template void list<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >::push_back(
    const CDefPointer<const CAppearanceModifierDef>& x);
} // namespace std
