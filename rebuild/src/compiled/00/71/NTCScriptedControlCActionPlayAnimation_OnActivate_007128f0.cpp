// NTCScriptedControl::CActionPlayAnimation::OnActivate() -- mark the scripted animation action active and clear the creature modes that block animation
// Leaf: ?OnActivate@CActionPlayAnimation@NTCScriptedControl@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007128F0, 15 bytes: `mov eax,[ecx+4] ; mov byte [ecx+8],1 ; mov ecx,[eax+4] ; jmp 0x0085F6A0`.
// Identity (two sources): CActionPlayAnimation vtable 0x0128D6F4 slot 2 = CActionBase slot 2 OnActivate (base 0x00711FD0 is
//   `Activated = true`, the byte stored here; the turn-to-face action inherits it); Ego_r.pdb / FableWin.pdb
//   ?OnActivate@CActionPlayAnimation@NTCScriptedControl@@UAEXXZ; the tail callee is the landed NCreatureMode::RemoveAnimationBlockingModes
//   0x0085F6A0, whose header notes its only callers are this and CActionPlayCombatAnimation::OnActivate. Catalog "vfunc_2" was a placeholder.
// Callees: ?RemoveAnimationBlockingModes@NCreatureMode@@YIXAAVCThing@@@Z 0x0085F6A0; CActionBase::OnActivate and GetParentThing inline.
// Header: engine/NTCScriptedControl_CActionPlayAnimation.h (new), engine/CTCScriptedControl.h (FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/NTCScriptedControl_CActionPlayAnimation.h"
namespace NCreatureMode { void __fastcall RemoveAnimationBlockingModes(CThing& thing); }   // 0x0085F6A0
void NTCScriptedControl::CActionPlayAnimation::OnActivate()
{
    CTCScriptedControl::CActionBase::OnActivate();
    NCreatureMode::RemoveAnimationBlockingModes(GetParentThing());
}