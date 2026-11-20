// CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& speaker, const char* text_group, ETextGroupSelectionMethod method, bool first_flag, bool second_flag, bool third_flag) -- vtable slot 13: forward to the implementation (PImp) when there is one
// Leaf: ?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7390: mov ecx,[ecx+8] ; test ecx,ecx ; je ; mov eax,[ecx] ; jmp dword ptr [eax+34h] (slot 13 of the
//   implementation, a tail call) ; ret.
// Name (NF120, PRIORITY q4623): Ego_r.pdb + FableWin.pdb ?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z, slot 13 of vtable 0x0127094C (engine/CScriptGameResourceObjectScriptedThingBase.h
//   declares it so; the landed Codex pass 13 callers bind this spelling). Was the non-virtual Speak(int x6) (QAEXHHHHHH) over an `Inner` vtable view.
// Layout: the shared retail header (FABLE_RETAIL_BASE_OBJECT): PImp CCountedPointer<CScriptGameResourceObjectScriptedThingBase> +0x08
//   (the recipe of the landed slot rows 0x007E7450 IsPerformingScriptTask / 0x007E72F0 MoveToPosition).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& speaker, const char* text_group, ETextGroupSelectionMethod method, bool first_flag, bool second_flag, bool third_flag)
{
    CScriptGameResourceObjectScriptedThingBase* imp = PImp.Get();
    if (imp)
        imp->Speak(speaker, text_group, method, first_flag, second_flag, third_flag);
}
