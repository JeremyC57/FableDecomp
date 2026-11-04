// NTCScriptedControl::CActionSummonerLightningAttackTarget::OnActivate() -- start the summoner's lightning attack by activating its turn-to-face sub-action
// Leaf: ?OnActivate@CActionSummonerLightningAttackTarget@NTCScriptedControl@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007122B0, 15 bytes: `mov eax,[ecx+4] ; mov edx,[ecx+1Ch] ; mov [ecx+20h],eax ; add ecx,1Ch ;
//   jmp [edx+8]` (the sub-action's SetScript inline, then its virtual OnActivate as a tail call).
// Identity (two sources): CActionSummonerLightningAttackTarget vtable 0x0126586C slot 2 = CActionBase slot 2 OnActivate (base 0x00711FD0;
//   slot 3 0x007122A0 is its OnAdd override, as the name set lists both); Ego_r.pdb / FableWin.pdb
//   ?OnActivate@CActionSummonerLightningAttackTarget@NTCScriptedControl@@UAEXXZ; Ego_r layout TurnToFaceAction
//   (CActionTurnToFacePosition) +0x1C, whose PTCScript +0x04 is the +0x20 store. Catalog "vfunc_2" was a placeholder.
// Callees: virtual CActionTurnToFacePosition::OnActivate (slot 2, inherited CActionBase 0x00711FD0).
// Header: engine/NTCScriptedControl_CActionSummonerLightningAttackTarget.h (new), engine/CTCScriptedControl.h
//   (FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#include "engine/ETCInterfaceType.h"
#include "engine/NTCScriptedControl_CActionSummonerLightningAttackTarget.h"
void NTCScriptedControl::CActionSummonerLightningAttackTarget::OnActivate()
{
    TurnToFaceAction.SetScript(PTCScript);
    TurnToFaceAction.OnActivate();
}