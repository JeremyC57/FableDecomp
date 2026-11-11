// CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const -- vtable slot 26: forward to the implementation (PImp), false without one
// Leaf: ?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7450, 15 bytes: mov ecx,[ecx+8] ; test ecx,ecx ; jne ; xor al,al ; ret ;
//   mov eax,[ecx] ; jmp dword ptr [eax+68h] (slot 26 of the implementation, a tail call).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ,
//   slot 26 of vtable 0x0127094C (engine/CScriptGameResourceObjectScriptedThingBase.h declares it so). Was the __fastcall stand-in
//   CScriptGameResourceObjectScriptedThingBase_IsPerformingScriptTask(CScriptGameResourceObjectScriptedThingBase*) over an `Inner` vtable view.
// Layout: the shared retail header (FABLE_RETAIL_BASE_OBJECT): PImp CCountedPointer<CScriptGameResourceObjectScriptedThingBase> +0x08.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const
{
    CScriptGameResourceObjectScriptedThingBase* imp = PImp.Get();
    if (imp == 0)
        return false;
    return imp->IsPerformingScriptTask();
}
