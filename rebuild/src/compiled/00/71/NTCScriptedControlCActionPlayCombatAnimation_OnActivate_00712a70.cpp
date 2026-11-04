// NTCScriptedControl::CActionPlayCombatAnimation::OnActivate() -- mark the scripted combat-animation action active and clear the creature modes that block animation
// Leaf: ?OnActivate@CActionPlayCombatAnimation@NTCScriptedControl@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00712A70, 15 bytes: `mov eax,[ecx+4] ; mov byte [ecx+8],1 ; mov ecx,[eax+4] ; jmp 0x0085F6A0`.
// Identity (two sources): CActionPlayCombatAnimation vtable 0x0128D728 slot 2 = CActionBase slot 2 OnActivate (base 0x00711FD0 is
//   `Activated = true`, the byte stored here; the turn-to-face action inherits it); Ego_r.pdb / FableWin.pdb
//   ?OnActivate@CActionPlayCombatAnimation@NTCScriptedControl@@UAEXXZ; the tail callee is the landed NCreatureMode::RemoveAnimationBlockingModes
//   0x0085F6A0, whose header notes its only callers are this and CActionPlayAnimation::OnActivate 0x007128F0. Catalog "vfunc_2" was a placeholder.
// Callees: ?RemoveAnimationBlockingModes@NCreatureMode@@YIXAAVCThing@@@Z 0x0085F6A0; CActionBase::OnActivate and GetParentThing inline.
// Header: engine/NTCScriptedControl_CActionPlayCombatAnimation.h (new), engine/CTCScriptedControl.h (FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/NTCScriptedControl_CActionPlayCombatAnimation.h"
namespace NCreatureMode { void __fastcall RemoveAnimationBlockingModes(CThing& thing); }   // 0x0085F6A0
void NTCScriptedControl::CActionPlayCombatAnimation::OnActivate()
{
    CTCScriptedControl::CActionBase::OnActivate();
    NCreatureMode::RemoveAnimationBlockingModes(GetParentThing());
}