// CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString& name, bool a1, bool a2, bool a3, bool a4, bool a5, bool a6, bool a7) -- vtable slot 18: forward to the implementation (PImp) when there is one
// Leaf: ?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E73D0: mov ecx,[ecx+8] ; test ecx,ecx ; je ; mov eax,[ecx] ; jmp dword ptr [eax+48h] (slot 18 of the
//   implementation, a tail call) ; ret.
// Name (NF120, PRIORITY q4624): Ego_r.pdb + FableWin.pdb ?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z, slot 18 of vtable 0x0127094C (engine/CScriptGameResourceObjectScriptedThingBase.h
//   declares it so; the landed Codex pass 13 callers bind this spelling). Was the non-virtual PlayAnimation(int x8) (QAEXHHHHHHHH) over an `Inner` vtable view.
// Layout: the shared retail header (FABLE_RETAIL_BASE_OBJECT): PImp CCountedPointer<CScriptGameResourceObjectScriptedThingBase> +0x08
//   (the recipe of the landed slot rows 0x007E7450 IsPerformingScriptTask / 0x007E72F0 MoveToPosition).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString& name, bool a1, bool a2, bool a3, bool a4, bool a5, bool a6, bool a7)
{
    CScriptGameResourceObjectScriptedThingBase* imp = PImp.Get();
    if (imp)
        imp->PlayAnimation(name, a1, a2, a3, a4, a5, a6, a7);
}
