// CGameComponent::CGameComponent(CGame& game) -- base constructor of every game component: not quitting, not running, the owning game
// Leaf: ??0CGameComponent@@QAE@AAVCGame@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051D8E0, 49 bytes, `ret 4`: CBaseClassNonCopyable's ctor 0x0099A2F0 (out of line), the
//   CDeviceResetCallback vtable 0x0122DE50 (its implicit ctor), the flags +8 / +9 and the game reference +0xC, then both CGameComponent vtables
//   0x0122ECB0 / 0x0122ECA4 (sunk below the member stores by the scheduler).
// Header port (NF233, 2026-11-22, q12638): engine/CGameComponent.h (Quit +8, Running +9, CGame& Game +0xC; Ego_r.pdb). Self leaf: Ego_r.pdb +
//   FableWin.pdb + Ego_d.pdb ??0CGameComponent@@QAE@AAVCGame@@@Z, which every binder (0x0042EA8F, 0x00418DCA, 0x00496070) now binds; was
//   ??0CGameComponent@@QAE@PAVCGame@@@Z (binders also PAUCMainGameComponentParent). Vtables now the PDB names ??_7CGameComponent@@6BCBaseClassNonCopyable@@@,
//   ...@@6BCDeviceResetCallback@@@ (was 6BCGameListener@@@) and ??_7CDeviceResetCallback@@6B@ (FableWin.pdb; was the stand-in ??_7CGameListener@@6B@).
//   FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE (NF233): the pick-any ??_GCGameComponent this object emits (43 B) then equals its owner 0x00415940,
//   which expands ~CGameComponent; HEAD's copy (30 B, a call to an ownerless ~CGameComponent) differed.
#define FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE
#include "engine/CGameComponent.h"

CGameComponent::CGameComponent(CGame& game)
    : Quit(false), Running(false), Game(game)
{
}
