// CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool) -- vtable slot 4:
//   forward the five arguments to the implementation (PImp) when there is one
// Leaf: ?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E72F0, 15 bytes: mov ecx,[ecx+8] ; test ecx,ecx ; je ; mov eax,[ecx] ; jmp dword ptr [eax+10h] ;
//   ret 14h (slot 4 of the implementation, a tail call with the caller's five stack arguments).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb
//   ?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z, slot 4 of vtable 0x0127094C
//   (the shared header declares it so). Was spelled MoveToPosition(int, int, int, int, int) over an `Inner` vtable view.
// Layout: the shared retail header (FABLE_RETAIL_BASE_OBJECT): PImp CCountedPointer<CScriptGameResourceObjectScriptedThingBase> +0x08.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector& position, float proximity, EScriptEntityMoveType moveType, bool firstFlag, bool secondFlag)
{
    CScriptGameResourceObjectScriptedThingBase* imp = PImp.Get();
    if (imp)
        imp->MoveToPosition(position, proximity, moveType, firstFlag, secondFlag);
}
