// CScriptGameResourceObjectScriptedThingBase::ClearAllActionsIncludingLoopingAnimations() -- vtable slot 22: forward to the implementation (PImp) when there is one
// Leaf: ?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7410: mov ecx,[ecx+8] ; test ecx,ecx ; je ; mov eax,[ecx] ; jmp dword ptr [eax+58h] (slot 22 of the
//   implementation, a tail call) ; ret.
// Name (NF120, PRIORITY q4625): Ego_r.pdb + FableWin.pdb ?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ, slot 22 of vtable 0x0127094C (engine/CScriptGameResourceObjectScriptedThingBase.h
//   declares it so; the landed Codex pass 13 callers bind this spelling). Was the __fastcall stand-in CScriptGameResourceObjectScriptedThingBase_ClearAllActionsIncludingLoopingAnimations(..*) over an `Inner` vtable view.
// Layout: the shared retail header (FABLE_RETAIL_BASE_OBJECT): PImp CCountedPointer<CScriptGameResourceObjectScriptedThingBase> +0x08
//   (the recipe of the landed slot rows 0x007E7450 IsPerformingScriptTask / 0x007E72F0 MoveToPosition).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

void CScriptGameResourceObjectScriptedThingBase::ClearAllActionsIncludingLoopingAnimations()
{
    CScriptGameResourceObjectScriptedThingBase* imp = PImp.Get();
    if (imp)
        imp->ClearAllActionsIncludingLoopingAnimations();
}
