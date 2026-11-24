// CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then ControllerType, ControllerButton, MouseButton, ButtonGraphic, Position, Offset memberwise
// Leaf: ??4CAbilityButtonGuiDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046F43E, 69 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`, returns this.
// Identity (proven): CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef&); evidence: its only def caller is CAbilityButtonGuiDef::Copy 0x0046F49F (a `jmp` to it), slot 19 of vtable 0x0123610C (RTTI .?AVCAbilityButtonGuiDef@@), so `*this = *static_cast<const CAbilityButtonGuiDef*>(other)`;
//   RTTI bases CParentDefClassBase <- CDefClassBase; the copied offsets and widths are Ego_r.pdb CAbilityButtonGuiDef's members (retail = Ego_r - 4 after the shorter CParentDefClassBase); FableWin.pdb name set holds ??4CAbilityButtonGuiDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "operator=" is propagated (family J059: five byte-similar def copy assignments); the Copy thunk's vtable names CAbilityButtonGuiDef.
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0 (landed under that name).
// Shape: the implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax. It stays implicit
//   (both PDBs spell the implicit form; engine/CAbilityButtonGuiDef.h declares none), emitted here as a pick-any COMDAT.
// NF337 (2026-11-24, coordinator D-2 retry): onto the shared engine/CAbilityButtonGuiDef.h (was a local model); the forcing device is no
//   longer a definition of ?Copy@CAbilityButtonGuiDef@@UAEXPBVCDefClassBase@@@Z (0x0046F49F's own symbol, now that row's) but the
//   anonymous-namespace scaffolding helper below (the 0x00E95380 / nf86 / nf88 form).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CAbilityButtonGuiDef.h"
namespace {
// Fidelity: scaffolding, not retail: takes the address of the implicit CAbilityButtonGuiDef::operator= so this object emits it out of
//   line as a pick-any COMDAT (retail emits it beside Copy, a `jmp` to it); the helper's name is hashed per TU and unreferenced.
CAbilityButtonGuiDef& (CAbilityButtonGuiDef::*ReferenceAssign())(const CAbilityButtonGuiDef&)
{
    return &CAbilityButtonGuiDef::operator=;
}
}
